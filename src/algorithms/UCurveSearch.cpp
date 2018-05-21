// UCurveSearch.cpp automatically generated by bin/add_new_algorithm.pl
// in Fri Jan 13 10:06:14 2017.

//
// UCurveSearch.cpp -- implementation of the class "UCurveSearch".
//
//    This file is part of the featsel program
//    Copyright (C) 2016  Marcelo S. Reis
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

#include "UCurveSearch.h"


UCurveSearch::UCurveSearch ()
{
  lower_restriction = new Collection ();
  upper_restriction = new Collection ();
  list_of_visited_subsets = new Collection ();
  cost_function = NULL;
}


UCurveSearch::~UCurveSearch ()
{
  if (lower_restriction != NULL)
    delete lower_restriction;
  if (upper_restriction != NULL)
    delete upper_restriction;
  if (list_of_visited_subsets != NULL)
    delete list_of_visited_subsets;
}


void UCurveSearch::get_minima_list (unsigned int max_size_of_minima_list)
{
  timeval begin_program, end_program;
  gettimeofday (& begin_program, NULL);

  //
  unsigned int direction;
  Collection * L = new Collection ();
  bool search_space_is_empty = false;
  ElementSubset * X, Y ("", set);
  UCurveToolBox::Node * M;
  do
  {
    // at each iteration it is called either minimal_element or maximal_element
    direction = rand () % 2;
    if (direction == 0)
    {
      X = UCurveToolBox::get_minimal_subset (lower_restriction,
       upper_restriction, set);
      if (X != NULL)
      {
        // if X is upper covered by the upper restrictions then
        // we may add X into the collection of lower restrictions; once X
        // is minimal, there is no risk of losing an element from the search space!
        if (upper_restriction->upper_covers (X))
          UCurveToolBox::update_lower_restriction (lower_restriction,
           X);
        else
        {
          M = UCurveToolBox::create_node (X);
          M->vertex->cost = cost_function->cost (M->vertex);
          // X is minimal, thus there is no lower adjacent
          UCurveToolBox::update_lower_restriction (lower_restriction,
           X);
          M->lower_flag->set_empty_subset ();
          UCurveToolBox::DFS (M, L, lower_restriction, 
            upper_restriction, cost_function);
        }
        delete X;
      }
      else
      {
        search_space_is_empty = true;
      }
    }
    else
    {
      X = UCurveToolBox::get_maximal_subset (lower_restriction,
       upper_restriction, set);
      if (X != NULL)
      {
        if (lower_restriction->lower_covers (X))
          UCurveToolBox::update_upper_restriction (upper_restriction,
           X);
        else
        {
          M = UCurveToolBox::create_node (X);
          M->vertex->cost = cost_function->cost (M->vertex);
          // X is maximal, thus there is no upper adjacent
          UCurveToolBox::update_upper_restriction (upper_restriction, 
            X);
          M->upper_flag->set_empty_subset ();
          UCurveToolBox::DFS (M, L, lower_restriction, 
            upper_restriction, cost_function);
        }
        delete X;
      }
      else
      {
        search_space_is_empty = true;
      }
    }

    while (L->size() > 0)
    {
      X = L->remove_last_subset ();
      list_of_minima.push_back (X);
      if (store_visited_subsets)
        list_of_visited_subsets->add_subset (X);
    }
    // Reminder: this function DOES NOT compute again the
    // cost function, instead it uses the cost value stored
    // into the ElementSubsets.
    //
    clean_list_of_minima (max_size_of_minima_list);

  } while ((!search_space_is_empty) &&
           (!cost_function->has_reached_threshold ()));

  delete L;
  //

  number_of_visited_subsets =
  cost_function->get_number_of_calls_of_cost_function ();
  clean_list_of_minima (max_size_of_minima_list);

  gettimeofday (& end_program, NULL);
  elapsed_time_of_the_algorithm = diff_us (end_program, begin_program);
}


