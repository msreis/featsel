//
// SFS.cpp -- implementation of the class "SFS"
// ("Sequential Forward Selection").
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

#include "SFS.h"


SFS::SFS ()
{
  list_of_visited_subsets = new Collection ();
  cost_function = NULL;
}


SFS::~SFS ()
{
  if (list_of_visited_subsets != NULL)
  delete list_of_visited_subsets;
}


void SFS::get_minima_list (unsigned int max_size_of_minima_list)
{
  timeval begin_program, end_program;
  gettimeofday (& begin_program, NULL);

  ElementSubset   X ("X", set), * Y;
  unsigned int i, k;
  float current_minima;

  X.set_empty_subset ();
  current_minima = X.cost = cost_function->cost (&X);
  if (store_visited_subsets)
    list_of_visited_subsets->add_subset (&X);
  Y = new ElementSubset ("", set);
  Y->copy (&X);
	list_of_minima.push_back (Y);

  do
  {
    k = set->get_set_cardinality ();

    for (i = 0; i < set->get_set_cardinality (); i++)
    {
      if (! X.has_element (i))
      {
        X.add_element (i);
        X.cost = cost_function->cost (&X);

        if (cost_function->has_reached_threshold () )
          break;

        if (current_minima >= X.cost)
	{
          k = i;
          current_minima = X.cost;
        }

        if (store_visited_subsets)
          list_of_visited_subsets->add_subset (&X);

        X.remove_element (i);
      }
    }

    // If k = |S| then there is no more element to add into X in order
    // to decrease the value of c(X)
    //
    if (k < set->get_set_cardinality ())
    {
      X.add_element (k);
      X.cost = current_minima;
      Y = new ElementSubset ("", set);
      Y->copy (&X);
      list_of_minima.push_back (Y);
    }

  }
  while ((X.get_subset_cardinality () < set->get_set_cardinality ()) &&
         (k < set->get_set_cardinality ()));

  number_of_visited_subsets =
  cost_function->get_number_of_calls_of_cost_function ();

  clean_list_of_minima (max_size_of_minima_list);

  gettimeofday (& end_program, NULL);
  elapsed_time_of_the_algorithm = diff_us (end_program, begin_program);

}


