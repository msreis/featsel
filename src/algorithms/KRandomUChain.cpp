// KRandomUChain.cpp automatically generated by bin/add_new_algorithm.pl
// in Wed Feb 14 12:58:52 2018.

//
// KRandomUChain.cpp -- implementation of the class "KRandomUChain".
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

#include "KRandomUChain.h"


KRandomUChain::KRandomUChain ()
{
  list_of_visited_subsets = new Collection ();
  cost_function = NULL;
  k_repetitions = K_REPETITIONS;
}


KRandomUChain::~KRandomUChain ()
{
  if (list_of_visited_subsets != NULL)
  delete list_of_visited_subsets;
}


void KRandomUChain::get_minima_list (unsigned int max_size_of_minima_list)
{
  timeval begin_program, end_program;
  gettimeofday (& begin_program, NULL);

  for (unsigned int i = 0; i < k_repetitions; i++)
  {
    ElementSubset X ("", set), * Y;
    double last_cost, current_cost;
    
    // We start with the complete subset in order to
    //randomize the chosen elements.
    X.set_complete_subset ();

    Y = new ElementSubset ("", set);
    Y->copy (&X);
    Y->set_complement_subset ();
    Y->cost = cost_function->cost (Y);
    list_of_minima.push_back (Y);

    current_cost = Y->cost;
    
    if (store_visited_subsets)
        list_of_visited_subsets->add_subset (Y);
    do
    {
      X.remove_random_element ();
      Y = new ElementSubset ("", set);
      Y->copy (&X);
      Y->set_complement_subset ();
      Y->cost = cost_function->cost (Y);
      list_of_minima.push_back (Y);

      last_cost = current_cost;
      current_cost = Y->cost;

      if (store_visited_subsets)
        list_of_visited_subsets->add_subset (Y);
    }
    while (!X.is_empty () && current_cost < last_cost &&
           !cost_function->has_reached_threshold ());
  }

  number_of_visited_subsets =
  cost_function->get_number_of_calls_of_cost_function ();
  clean_list_of_minima (max_size_of_minima_list);

  gettimeofday (& end_program, NULL);
  elapsed_time_of_the_algorithm = diff_us (end_program, begin_program);

}


