// CHCGA.cpp automatically generated by bin/add_new_algorithm.pl
// in Fri May  4 11:19:48 2018.

//
// CHCGA.cpp -- implementation of the class "CHCGA".
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

#include "CHCGA.h"


CHCGA::CHCGA ()
{
  list_of_visited_subsets = new Collection ();
  cost_function = NULL;
}


CHCGA::~CHCGA ()
{
  if (list_of_visited_subsets != NULL)
  delete list_of_visited_subsets;
}


void CHCGA::get_minima_list (unsigned int max_size_of_minima_list)
{
  timeval begin_program, end_program;
  gettimeofday (& begin_program, NULL);
  unsigned int cataclysms = 20;
  list<ElementSubset *> population;
  list<ElementSubset *> offspring;
  list<ElementSubset *>::iterator it;

  //
  CHCGAPopulation pop (set, cost_function);
  pop.start_population (50);
  population = pop.get_population ();
  for (it = population.begin (); it != population.end (); it++)
  {
    list_of_visited_subsets->add_subset (*it);
    list_of_minima.push_back (new ElementSubset (*it));
  }

  if (!cost_function->has_reached_threshold ())
  {
    while (cataclysms > 0 && !cost_function->has_reached_threshold ())
    {
      offspring = pop.recombine ();
      if (pop.fittest_survival (offspring))
        cataclysms--;

      population = pop.get_population ();
      for (it = population.begin (); it != population.end (); it++)
      {
        list_of_visited_subsets->add_subset (*it);
        list_of_minima.push_back (new ElementSubset (*it));
      } 
    }
  }

  number_of_visited_subsets =
  cost_function->get_number_of_calls_of_cost_function ();

  gettimeofday (& end_program, NULL);
  elapsed_time_of_the_algorithm = diff_us (end_program, begin_program);
  clean_list_of_minima (max_size_of_minima_list);

}


