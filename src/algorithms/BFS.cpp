//
// BFS.cpp -- implementation of the class "BFS".
//
//    This file is part of the featsel program
//    Copyright (C) 2017  Marcelo S. Reis
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

# include "BFS.h"

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


unsigned int BFS::get_top (Queue * OPEN)
{
  return OPEN->best_index;
}


void BFS::insert_subset (Queue * OPEN, ElementSubset *X)
{
  unsigned int worst_index = 0;

  for (unsigned int i = 1; i < OPEN->maximum_size; i++)
    if (OPEN->queue[i]->cost > OPEN->queue[worst_index]->cost)
      worst_index = i;

  if (OPEN->maximum_size > OPEN->current_size)
    OPEN->current_size++;

  // As in Weka, we will just update the Queue if X->cost is better than
  // the cost of the worst element! 
  //
  if (X->cost < OPEN->queue[worst_index]->cost)
  {
    OPEN->queue[worst_index]->copy (X);

    if (OPEN->queue[OPEN->best_index]->cost > OPEN->queue[worst_index]->cost)
      OPEN->best_index = worst_index;
  }
}


void BFS::remove_subset (Queue * OPEN, unsigned int index)
{
  OPEN->queue[index]->cost = INFTY;
  OPEN->current_size--;
}


void BFS::get_minima_list (unsigned int max_size_of_minima_list)
{
  timeval begin_program, end_program;
  gettimeofday (& begin_program, NULL);

  unsigned int current_number_of_expansions = 0;

  ElementSubset empty_set ("", set);
  ElementSubset * BEST_SUBSET = new ElementSubset ("", set);

  double BEST = empty_set.cost = cost_function->cost (&empty_set);

  BEST_SUBSET->copy (&empty_set);

  Queue OPEN;

  OPEN.queue = new ElementSubset * [k];
  for (unsigned int i = 0; i < k; i++)
  {
    OPEN.queue[i] = new ElementSubset ("", set);
    OPEN.queue[i]->cost = INFTY;
  }
  OPEN.maximum_size = k;
  OPEN.current_size = 1;
  OPEN.best_index = 0;

  std::set <string> CLOSED;
  unsigned int insert_count = 0;

  CLOSED.insert (empty_set.print_subset ());
  insert_count++;

  if (store_visited_subsets)
    list_of_visited_subsets->add_subset (&empty_set);

  ElementSubset v ("", set);

  do
  {
    bool added = false;
  
    // Get the subset from OPEN with maximal c(X).
    //
    unsigned int top = get_top (&OPEN);

    v.copy (OPEN.queue[top]);

    remove_subset (&OPEN, top);

    for (unsigned int i = 0; i < set->get_set_cardinality (); i++)
    {
      if (cost_function->has_reached_threshold ())
        break;

      if (! v.has_element (i))
      {
        v.add_element (i);

        if (CLOSED.find (v.print_subset ()) == CLOSED.end ())
        {
          v.cost = cost_function->cost (&v);

          // Equivalent to "m_cacheSize * m_numAttribs" in Weka.
          //
          if (insert_count == (HASH_CACHE_SIZE * set->get_set_cardinality ()))
          {
            CLOSED.clear ();
            insert_count = 0;
          }

          CLOSED.insert (v.print_subset ());
          insert_count++;

          insert_subset (&OPEN, &v);

          if (store_visited_subsets)
            list_of_visited_subsets->add_subset (&v);
        }

        v.remove_element (i);
      }

      if ((BEST - v.cost) > epsilon)
      {
        BEST = v.cost;
        BEST_SUBSET->copy (&v);
        BEST_SUBSET->cost = BEST;
        current_number_of_expansions = 1;
        added = true;
      }
    }

    if (! added)
      current_number_of_expansions++;
  }
  while ((current_number_of_expansions < k) && 
         (OPEN.current_size > 0) &&
         (!cost_function->has_reached_threshold ()));

  for (unsigned int i = 0; i < k; i++)
    delete OPEN.queue[i];
  delete [] OPEN.queue;

  list_of_minima.push_back (BEST_SUBSET);

  number_of_visited_subsets =
                         cost_function->get_number_of_calls_of_cost_function ();

  clean_list_of_minima (max_size_of_minima_list);

  gettimeofday (& end_program, NULL);
  elapsed_time_of_the_algorithm = diff_us (end_program, begin_program);

}


