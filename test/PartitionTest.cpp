//
// PartitionTest.cpp -- implementation of the namespace "PartitionTest".
//
//    This file is part of the featsel program
//    Copyright (C) 2016  Marcelo S. Reis, Gustavo Estrela de Matos
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

#include "PartitionTest.h"

namespace PartitionTest
{

  bool it_should_create_the_fixed_elm_set ()
  {
    unsigned int set_size = 12;
    unsigned int nof_fixed = 0;
    ElementSet original_set ("", set_size, 100);
    bool * is_fixed = new bool[set_size];
    for (unsigned int i = 0; i < set_size; i++)
      if (i % 3)
      {
        is_fixed[i] = true;
        nof_fixed++;
      }
      else
        is_fixed[i] = false;
    Partition partition (&original_set, is_fixed);
    delete[] is_fixed;
    ElementSet * fixed_elm_set;
    fixed_elm_set = partition.get_fixed_elm_set ();
    unsigned int fixed_set_size = fixed_elm_set->get_set_cardinality ();
    if (nof_fixed != fixed_set_size)
      return false;
    return true;
  }


  bool it_should_create_the_unfixed_elm_set ()
  {
    unsigned int set_size = 12;
    unsigned int unfixed_set_size;
    unsigned int nof_unfixed = 0;
    ElementSet original_set ("", set_size, 100);
    bool * is_fixed = new bool[set_size];
    for (unsigned int i = 0; i < set_size; i++)
      if (i % 3)
        is_fixed[i] = true;
      else
      {
        is_fixed[i] = false;
        nof_unfixed++;
      }
    Partition partition (&original_set, is_fixed);
    delete[] is_fixed;
    ElementSet * unfixed_elm_set;
    unfixed_elm_set = partition.get_unfixed_elm_set ();
    unfixed_set_size = unfixed_elm_set->get_set_cardinality ();
    if (nof_unfixed != unfixed_set_size)
      return false;
    return true;
  }

} // end namespace
