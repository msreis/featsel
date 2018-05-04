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

}