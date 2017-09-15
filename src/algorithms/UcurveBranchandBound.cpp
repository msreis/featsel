//
// UcurveBranchandBound.cpp -- implementation of the class
// "UcurveBranchandBound".
//
//    This file is part of the featsel program
//    Copyright (C) 2015  Marcelo S. Reis
//
//    This program is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with this program.  If not, see <http://www.gnu.org/licenses/>.
//

#include "UcurveBranchandBound.h"

UcurveBranchandBound::UcurveBranchandBound ()
{
  list_of_visited_subsets = new Collection ();
  cost_function = NULL;
}


UcurveBranchandBound::~UcurveBranchandBound ()
{
  if (list_of_visited_subsets != NULL)
    delete list_of_visited_subsets;
}


void UcurveBranchandBound::get_minima_list
(unsigned int max_size_of_minima_list)
{
  timeval begin_program, end_program;
  gettimeofday (& begin_program, NULL);

  ElementSubset * A, * B;
  unsigned int i, n;
  double bound, current_cost, last_cost;
  list <ElementSubset *> Stack;
  list <unsigned int> Leftmost_free_elem;
  list <double> Last_cost_stack;

  A = new ElementSubset ("A", set);
  Stack.push_back (A);
  Leftmost_free_elem.push_back (0);
  Last_cost_stack.push_back (INFTY);
  bound = INFTY;

  while ((! Stack.empty ()) && (! cost_function->has_reached_threshold ()))
  {
    A = Stack.back ();
    Stack.pop_back ();
    n = Leftmost_free_elem.back ();
    Leftmost_free_elem.pop_back ();
    last_cost = Last_cost_stack.back ();
    Last_cost_stack.pop_back ();

    current_cost = A->cost = cost_function->cost (A);

    if (((A->get_subset_cardinality () == 0) || (current_cost <= last_cost)) &&
        (A->get_subset_cardinality () <= MAX_SUBSET_SIZE) )
    {
      // The bound helps to keep the list of minimum small
      //
      if (current_cost <= bound)
      {
        bound = current_cost;
        B = new ElementSubset ("", set);
        B->copy (A);
        list_of_minima.push_back (B);
      }

      if (store_visited_subsets)
        list_of_visited_subsets->add_subset (A);

      for (i = n; i < set->get_set_cardinality (); i++)
      {
        A->add_element (i);
        B = new ElementSubset ("", set);
        B->copy (A);
        Stack.push_back (B);
        Leftmost_free_elem.push_back (i+1);
        Last_cost_stack.push_back (current_cost);
        A->remove_element (i);
      }
    } // if
    delete A;
  } // while

  number_of_visited_subsets =
  cost_function->get_number_of_calls_of_cost_function ();

  clean_list_of_minima (max_size_of_minima_list);

  gettimeofday (& end_program, NULL);
  elapsed_time_of_the_algorithm =
  diff_us (end_program, begin_program);

}
