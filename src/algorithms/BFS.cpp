//
// BFS.cpp -- implementation of the class "BFS".
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

#include "BFS.h"


BFS::BFS ()
{
  list_of_visited_subsets = new Collection ();
  cost_function = NULL;
  k = MAXIMUM_NUMBER_OF_EXPANSIONS;
  epsilon = ACCURACY;
}


BFS::~BFS ()
{
  if (list_of_visited_subsets != NULL)
  delete list_of_visited_subsets;
}


void BFS::get_minima_list (unsigned int max_size_of_minima_list)
{
  timeval begin_program, end_program;
  gettimeofday (& begin_program, NULL);

  unsigned int number_of_expansions_without_improvement = 0;

  map <string, ElementSubset *> OPEN, CLOSED; 
  map <string, ElementSubset *>::iterator it; 

  ElementSubset * X = new ElementSubset ("X", set);    

  float BEST = X->cost = cost_function->cost (X); // X == empty set.  

  OPEN.insert (pair<string, ElementSubset *>(X->print_subset (), X));

  do
  {
    ElementSubset * v = NULL;
    float maximal_subset_value = FLT_MAX;

    // Get the subset from OPEN with maximal c(X).
    //
    for (it = OPEN.begin (); it != OPEN.end (); it++)
    {
      if (it->second->cost <= maximal_subset_value)
      {
        v = it->second;
        maximal_subset_value = v->cost;
      }
    }

    OPEN.erase (v->print_subset ());

    CLOSED.insert (pair<string, ElementSubset *>(v->print_subset (), v));

    if (store_visited_subsets)
      list_of_visited_subsets->add_subset (v);

    if (v->cost <= (BEST + epsilon))
    {
      BEST = v->cost;
      number_of_expansions_without_improvement = 0;

      for (unsigned int i = 0; i < set->get_set_cardinality (); i++)
      {
        if (v->has_element (i))
        {
          v->remove_element (i);
          if ((OPEN.find (v->print_subset ()) == OPEN.end ()) && 
              (CLOSED.find (v->print_subset ()) == CLOSED.end ()))
          {
            X = new ElementSubset ("X", set);
            X->copy (v);
            X->cost = cost_function->cost (X);
            OPEN.insert (pair<string, ElementSubset *>(X->print_subset (), X));
          }
          v->add_element (i);
        }
        else
        {
          v->add_element (i);

          if ((OPEN.find (v->print_subset ()) == OPEN.end ()) && 
              (CLOSED.find (v->print_subset ()) == CLOSED.end ()))
          {
            X = new ElementSubset ("X", set);
            X->copy (v);
            X->cost = cost_function->cost (X);
            OPEN.insert (pair<string, ElementSubset *>(X->print_subset (), X));
          }
          v->remove_element (i);
        }
      }   // for (unsigned int i = 0 ...
    }
    else
    {
      number_of_expansions_without_improvement++;
    }
  }
  while ((number_of_expansions_without_improvement < k) && (OPEN.size () > 0));

  for (it = CLOSED.begin (); it != CLOSED.end (); it++)
  {
    list_of_minima.push_back (it->second);
  }

  number_of_visited_subsets = 
                         cost_function->get_number_of_calls_of_cost_function ();
  clean_list_of_minima (max_size_of_minima_list);

  gettimeofday (& end_program, NULL);
  elapsed_time_of_the_algorithm = diff_us (end_program, begin_program);

}


