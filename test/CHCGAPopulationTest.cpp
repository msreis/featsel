//
// CHCGAPopulationTest.cpp -- implementation of the namespace "CHCGAPopulationTest".
//
//    This file is part of the featsel program
//    Copyright (C) 2018 Marcelo S. Reis, Gustavo Estrela
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

#include "CHCGAPopulationTest.h"

namespace CHCGAPopulationTest
{

  bool it_should_create_a_starting_population ()
  {
    ElementSet elm_set ("", 3, 100);
    SubsetSum c (&elm_set);
    CHCGAPopulation pop (&elm_set, &c);
    pop.start_population (100);

    if (pop.get_size () == 100)
      return true;
    else
      return false;
  }

  bool it_should_combine_individuals ()
  {
    bool answ = true;
    ElementSet elm_set ("", 4, 100);
    SubsetSum c (&elm_set);
    CHCGAPopulation pop (&elm_set, &c);
    ElementSubset father ("", &elm_set);
    ElementSubset mother ("", &elm_set);

    mother.add_element (0);
    mother.add_element (1);
    mother.add_element (2);
    mother.add_element (3);

    ElementSubset * child = pop.combine_individuals (&mother, &father);
    answ = child->get_subset_cardinality () == 2;
    delete child;

    mother.remove_element (0);
    mother.remove_element (2);
    father.add_element (1);
    father.add_element (3);
    child = pop.combine_individuals (&mother, &father);
    answ = answ && child->get_subset_cardinality () == 2;
    delete child;

    return answ;
  }

  bool only_the_fittest_should_survive ()
  {
    bool answ = true;
    ElementSet elm_set ("", 4, 1000);
    SubsetSum c (&elm_set);
    CHCGAPopulation * pop = new CHCGAPopulation (&elm_set, &c);
    list<ElementSubset *> pop_indviduals; 
    list<ElementSubset *> pop_offspring;

    // 10 is big enough so we don't have a cataclysm on these
    // two first generations
    pop->start_population (10);
    pop_offspring = pop->recombine ();
    while (pop_offspring.size () < 1)
    {
      delete pop;
      pop = new CHCGAPopulation (&elm_set, &c);
      pop->start_population (10);
      pop_offspring = pop->recombine ();
    }
    
    ElementSubset * worst_fit = pop_offspring.back ();
    worst_fit->cost = c.cost (worst_fit);

    list<ElementSubset *>::iterator offspring_it = 
      pop_offspring.begin ();
    list<ElementSubset *>::iterator offspring_it_end = 
      pop_offspring.end ();
    while (offspring_it != offspring_it_end)
    {
      ElementSubset * child = (*offspring_it);
      child->cost = c.cost (child);
      if (worst_fit->cost < child->cost)
      {
        worst_fit = child;
      }
      offspring_it++;
    }
    pop_indviduals = pop->get_population ();
    
    list<ElementSubset *>::iterator pop_it = pop_indviduals.begin ();
    list<ElementSubset *>::iterator pop_it_end = pop_indviduals.end ();
    while (pop_it != pop_it_end)
    {
      if (worst_fit->cost < (*pop_it)->cost)
        worst_fit = *pop_it;
      pop_it++;
    }
    worst_fit = new ElementSubset (worst_fit);
    pop->fittest_survival (pop_offspring);

    pop_indviduals = pop->get_population ();
    pop_it = pop_indviduals.begin ();
    pop_it_end = pop_indviduals.end ();
    while (pop_it != pop_it_end)
    {
      if ((*pop_it)->cost > worst_fit->cost)
      {
        answ = false;
      }
      pop_it++;
    }

    delete worst_fit;
    return answ;
  }

}