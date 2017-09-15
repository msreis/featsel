//
// UCSR.cpp -- implementation of the class "UCSR".
//
//    This file is part of the featsel program
//    Copyright (C) 2016 Gustavo E. Matos
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


#include "UCSR.h"

UCSR::UCSR ()
{
  list_of_visited_subsets = new Collection ();
  cost_function = NULL;
    
  elapsed_time_of_all_calls_of_the_minima_exhausting = 0;
  elapsed_time_consulting_restrictions = 0;
  elapsed_time_reducing_restrictions = 0;
  elapsed_time_updating_restrictions = 0;
  number_of_calls_of_minimum_exhausting = 0;
  number_of_restrictions_updates = 0;
  number_of_restrictions_consults = 0;
}


UCSR::~UCSR ()
{
    if (restrictions != NULL)
        delete restrictions;
    if (list_of_visited_subsets != NULL)
        delete list_of_visited_subsets;
}


void UCSR::get_minima_list (unsigned int max_size_of_minima_list)
{
    restrictions = new ROBDD (set);
    timeval begin_exhausting, end_exhausting, begin_program, end_program;
    gettimeofday (& begin_program, NULL);

    Collection * L = new Collection ();
    bool search_space_is_empty = false;
    ElementSubset * X, Y ("", set);
    UCSRToolBox::Node * M;

    srand ( (unsigned) time (NULL) );

    do
    {
        X = restrictions->get_random_zero_evaluated_element ();
        if (X != NULL)
        {
            gettimeofday (& begin_exhausting, NULL);
            M = UCSRToolBox::create_node (X);
            M->vertex->cost = cost_function->cost (M->vertex);

            DFS (M, L, restrictions, cost_function);
            number_of_calls_of_minimum_exhausting++;

            gettimeofday (& end_exhausting, NULL);
            elapsed_time_of_all_calls_of_the_minima_exhausting +=
            diff_us (end_exhausting, begin_exhausting);
            delete X;
        }
        else
            search_space_is_empty = true;

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

    } while ( (! search_space_is_empty) &&
              (! cost_function->has_reached_threshold () ) );

    delete L;

    number_of_visited_subsets =  cost_function->get_number_of_calls_of_cost_function ();

    // TODO: include these methods into ROBDD class!
    //
    // number_of_restrictions_consults = restrictions->get_nof_consults ();
    // number_of_restrictions_updates = restrictions->get_nof_updates ();
    // elapsed_time_consulting_restrictions = restrictions->get_time_consulting ();
    // elapsed_time_updating_restrictions = restrictions->get_time_updating ();
    // elapsed_time_reducing_restrictions = restrictions->get_time_reducing ();

    gettimeofday (& end_program, NULL);
    elapsed_time_of_the_algorithm = diff_us (end_program, begin_program);
}
