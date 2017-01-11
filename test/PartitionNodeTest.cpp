//
// PartitionTest.cpp -- implementation of the namespace "PartitionTest".
//
//    This file is part of the featsel program
//    Copyright (C) 2016 Marcelo S. Reis, Gustavo Estrela de Matos
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

#include "PartitionNodeTest.h"

namespace PartitionNodeTest
{
  
  bool it_map_subsets_correctly ()
  {
    bool answ;
    unsigned int set_size = 12;
    ElementSet original_set ("", set_size, 100);
    bool * is_fixed = new bool[set_size];
    for (unsigned int i = 0; i < set_size; i++)
      is_fixed[i] = false;
    is_fixed[0] = true;
    is_fixed[2] = true;
    is_fixed[3] = true;
    is_fixed[5] = true;
    is_fixed[7] = true;
    Partition partition (&original_set, is_fixed);
    ElementSet * fixed_set = partition.get_fixed_elm_set ();
    ElementSubset part_subset ("", fixed_set);
    part_subset.add_element (0);
    part_subset.add_element (2);
    part_subset.add_element (4);
    PartitionNode P (&partition, &part_subset);
    // P        = 1X01X0X1XXXXX 
    // expected = 1001100101000
    // input    =  0  1 0 01000
    ElementSubset expected_subset ("", &original_set);
    expected_subset.add_element (0);
    expected_subset.add_element (3);
    expected_subset.add_element (4);
    expected_subset.add_element (7);
    expected_subset.add_element (9);
    ElementSet * unfixed_set = partition.get_unfixed_elm_set ();
    ElementSubset input_subset ("", unfixed_set);
    input_subset.add_element (1);
    input_subset.add_element (4);
    ElementSubset * answ_subset;
    answ_subset = P.get_original_subset (&input_subset);
    if (!answ_subset->is_equal (&expected_subset))
      answ = false;
    else
      answ = true;
    delete answ_subset;
    delete[] is_fixed;
    return answ;
  }


  bool it_should_check_upper_adjacent_parts ()
  {
    bool answ = true;
    ElementSet original_set ("", 10, 100);
    bool fixed[10];
    for (unsigned int i = 0; i < 10; i++)
      fixed[i] = i < 5; // we are fixing the first 5 elements    
    Partition * part = new Partition (&original_set, fixed);
    ElementSet * fixed_set = part->get_fixed_elm_set ();
    ElementSubset p_subset ("", fixed_set);
    ElementSubset q1_subset ("", fixed_set);
    ElementSubset q2_subset ("", fixed_set);
    p_subset.add_element (1);
    p_subset.add_element (3);
    q1_subset.add_element (1);
    q1_subset.add_element (3);
    q1_subset.add_element (4);
    q2_subset.add_element (2);
    PartitionNode * P = new PartitionNode (part, &p_subset);
    PartitionNode * Q1 = new PartitionNode (part, &q1_subset);
    PartitionNode * Q2 = new PartitionNode (part, &q2_subset);
    if (!Q1->is_upper_adjacent (P))
      answ = false;
    if (Q2->is_upper_adjacent (P))
      answ = false;
    delete P;
    delete Q1;
    delete Q2;
    delete part;
    return answ;
  }


  bool it_should_return_the_least_subset ()
  {
    bool answ = true;
    ElementSet original_set ("", 10, 100);
    bool fixed[10];
    for (unsigned int i = 0; i < 10; i++)
            fixed[i] = i < 5; // we are fixing the first 5 elements
    Partition * partition;
    partition = new Partition (&original_set, fixed);
    ElementSet * fixed_set = partition->get_fixed_elm_set ();
    ElementSubset p_subset ("", fixed_set);
    p_subset.add_element (0);
    p_subset.add_element (2);
    p_subset.add_element (3);
    PartitionNode P (partition, &p_subset);
    ElementSubset expected_least ("", &original_set);
    expected_least.add_element (0);
    expected_least.add_element (2);
    expected_least.add_element (3);
    ElementSubset * answ_minimal = P.get_least_subset ();
    if (!answ_minimal->is_equal (&expected_least))
      answ = false;
    delete partition;
    delete answ_minimal;
    return answ;
  }


  bool it_should_return_the_greatest_subset ()
  {
    bool answ = true;
    ElementSet original_set ("", 10, 100);
    bool fixed[10];
    for (unsigned int i = 0; i < 10; i++)
      fixed[i] = i < 5; // we are fixing the first 5 elements
    Partition * partition;
    partition = new Partition (&original_set, fixed);
    ElementSet * fixed_set = partition->get_fixed_elm_set ();
    ElementSubset p_subset ("", fixed_set);
    p_subset.add_element (0);
    p_subset.add_element (2);
    p_subset.add_element (3);
    PartitionNode P (partition, &p_subset);
    ElementSubset expected_greatest ("", &original_set);
    expected_greatest.add_element (0);
    expected_greatest.add_element (2);
    expected_greatest.add_element (3);
    expected_greatest.add_element (5);
    expected_greatest.add_element (6);
    expected_greatest.add_element (7);
    expected_greatest.add_element (8);
    expected_greatest.add_element (9);
    ElementSubset * answ_greatest = P.get_greatest_subset ();
    if (!answ_greatest->is_equal (&expected_greatest))
      answ = false;
    delete partition;
    delete answ_greatest;
    return answ;
  }

} // end namespace
