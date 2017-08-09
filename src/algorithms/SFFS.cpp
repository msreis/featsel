//
// SFFS.cpp -- implementation of the class "SFFS"
// ("Sequential Forward Floating Selection").
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

#include "SFFS.h"


SFFS::SFFS ()
{
  list_of_visited_subsets = new Collection ();
  cost_function = NULL;
}


SFFS::~SFFS ()
{
  if (list_of_visited_subsets != NULL)
    delete list_of_visited_subsets;
}


void SFFS::get_minima_list (unsigned int max_size_of_minima_list)
{
  timeval begin_program, end_program;
  gettimeofday (& begin_program, NULL);

  ElementSubset X ("X", set), Y ("Y", set), * Z;
  unsigned int k, best, worst,
               number_of_exclusions; // Max number of exclusions during Steps
	                             // 2 and 3 of a given iteration
  float * J;

  J = new (nothrow) float [set->get_set_cardinality() + 1];
  if (J == NULL)
    cout << endl << "SFFS: error allocating array J!" << endl;

  X.set_empty_subset ();
  Y.set_empty_subset ();

  // Add the empty set
  //
  k = 0;
  J[k] = X.cost = cost_function->cost (&X);
  Z = new ElementSubset ("", set);
  Z->copy (&X);
  list_of_minima.push_back (Z);

  if (store_visited_subsets)
    list_of_visited_subsets->add_subset (&X);

  best = get_best_element (&X);
  while ((best < set->get_set_cardinality ()) &&
         (!cost_function->has_reached_threshold ()) )
  {
    // Update the current best subset
    //
    X.add_element (best);
    k++;
    J[k] = X.cost = cost_function->cost (&X);

    if (cost_function->has_reached_threshold ())
      break;

    Z = new ElementSubset ("", set);
    Z->copy (&X);
    list_of_minima.push_back (Z);
    if (store_visited_subsets)
      list_of_visited_subsets->add_subset (&X);

    // Try to remove subsets (conditional exclusion)
    // in order to restart the inclusion from a better smaller subset
    //
    worst = get_worst_element (&X);
    if (cost_function->has_reached_threshold ())
      break;

    Y.copy (&X);
    Y.remove_element (worst);

    Y.cost = cost_function->cost (&Y);
    if (cost_function->has_reached_threshold ())
      break;

    // If the best feature is also the worst, go to Step 1;
    // otherwise initializes Step 2
    //
    if (J[k-1] <= Y.cost)
    {
      // J[k-1] == c(X - {best})
      // if J[k-1] < Y.cost then the best feature is NOT also a worst one!

      // Step 2
      //
      number_of_exclusions = 0;
      while ((k > 2) && (Y.cost > J[k-1]) &&
             (number_of_exclusions <= PARAMETER_DELTA)) // Step 3
      {
        X.remove_element (worst);
        k--; number_of_exclusions++;
        J[k] = X.cost = Y.cost;
        worst = get_worst_element (&X);
        if (cost_function->has_reached_threshold ())
          break;
        Y.remove_element (worst);
        Y.cost = cost_function->cost (&Y);
        if (cost_function->has_reached_threshold ())
          break;
        if (store_visited_subsets)
          list_of_visited_subsets->add_subset (&X);
      }
    } // if

    if (cost_function->has_reached_threshold ())
      break;

    best = get_best_element (&X); // To the next iteration

  } // while

  delete [] J;

  number_of_visited_subsets =
  cost_function->get_number_of_calls_of_cost_function ();

  clean_list_of_minima (max_size_of_minima_list);

  gettimeofday (& end_program, NULL);
  elapsed_time_of_the_algorithm = diff_us (end_program, begin_program);

}


// Class subroutines
//

unsigned int SFFS::get_best_element (ElementSubset * X)
{
  unsigned int i, j = set->get_set_cardinality ();
  ElementSubset Y ("Y", set);
  float current_minimum = cost_function->cost (X);
  Y.copy (X);

  for (i = 0; i < set->get_set_cardinality (); i++)
  {
    if (cost_function->has_reached_threshold ())
      return set->get_set_cardinality ();

    if (! Y.has_element (i))
    {
      Y.add_element (i);
      if (store_visited_subsets)
        list_of_visited_subsets->add_subset (&Y);
      Y.cost = cost_function->cost (&Y);

      if (current_minimum >= Y.cost)
      {
        j = i;
        current_minimum = Y.cost;
      }
      Y.remove_element (i);
    }
  }
  return j;
}


unsigned int SFFS::get_worst_element (ElementSubset * X)
{
  unsigned int i, j = set->get_set_cardinality ();
  ElementSubset Y ("Y", set);

  float current_maximum = cost_function->cost (X);
  Y.copy (X);

  for (i = 0; i < set->get_set_cardinality (); i++)
  {
    if (cost_function->has_reached_threshold ())
      return set->get_set_cardinality ();

    if (Y.has_element (i))
    {
      Y.remove_element (i);
      if (store_visited_subsets)
        list_of_visited_subsets->add_subset (&Y);
      Y.cost = cost_function->cost (&Y);
      if (current_maximum <= Y.cost)
      {
        j = i;
        current_maximum = Y.cost;
      }
      Y.add_element (i);
    }
  }
  return j;
}
