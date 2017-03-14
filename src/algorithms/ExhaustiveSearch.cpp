//
// ExhaustiveSearch.cpp -- implementation of the class "ExhaustiveSearch".
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

#include "ExhaustiveSearch.h"


ExhaustiveSearch::ExhaustiveSearch ()
{
  list_of_visited_subsets = new Collection ();
  cost_function = NULL;
}


ExhaustiveSearch::~ExhaustiveSearch ()
{
  if (list_of_visited_subsets != NULL)
    delete list_of_visited_subsets;

}


void ExhaustiveSearch::get_minima_list (unsigned int max_size_of_minima_list)
{
  timeval begin_program, end_program;
  gettimeofday (& begin_program, NULL);

  ElementSubset   X ("X", set), * Y;
  int i;
  double n = (double) set->get_set_cardinality ();

  X.set_empty_subset (); // X starts with empty set

  do  // Amortized time per iteration is O(1) + O(f(n))
  {
    i = 0;

    while ((i < n) && (X.has_element (i)))
    {
      X.remove_element (i);
      i++;
    }

    if (i < n)
      X.add_element (i);

    if (store_visited_subsets)
     list_of_visited_subsets->add_subset (&X);

    X.cost = cost_function->cost (&X);  // O(f(n))
    Y = new ElementSubset ("", set);
    Y->copy (&X);
    list_of_minima.push_back (Y);
  }
  while ( (i < n) && (!cost_function->has_reached_threshold ()) );

  clean_list_of_minima (max_size_of_minima_list);

  // Exhaustive search, if implemented keeping just an element of minimum cost,
  // needs to compute the cost function 2^|S| times.
  //
  number_of_visited_subsets =
  cost_function->get_number_of_calls_of_cost_function ();

  gettimeofday (& end_program, NULL);
  elapsed_time_of_the_algorithm = diff_us (end_program, begin_program);

}

