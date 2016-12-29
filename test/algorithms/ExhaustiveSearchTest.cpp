//
// ExhaustiveSearchTest.cpp -- implementation of the namespace "ExhaustiveSearchTest".
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

#include "ExhaustiveSearchTest.h"

namespace ExhaustiveSearchTest
{

  bool a_lattice_with_constant_cost_should_be_totally_exausted ()
  {
  ElementSet a_set1 ("S1", 3, 1);    // |S1| = 3
  ElementSet a_set2 ("S2", 10, 1);   // |S2| = 10
  ElementSet a_set3 ("S3", 1, 1);    // |S3| = 1
  SubsetSum c1 (&a_set1);
  SubsetSum c2 (&a_set2);
  SubsetSum c3 (&a_set3);
  ExhaustiveSearch es1;
  ExhaustiveSearch es2;
  ExhaustiveSearch es3;
  string list;
  es1.set_parameters (&c1, &a_set1, false);
  es1.get_minima_list (8);
  list = es1.print_list_of_minima ();
  es2.set_parameters (&c2, &a_set2, false);
  es2.get_minima_list (1024);
  es3.set_parameters (&c3, &a_set3, false);
  es3.get_minima_list (2);

  if ((list.find ("<000>") != string::npos) &&
      (list.find ("<001>") != string::npos) &&
      (list.find ("<010>") != string::npos) &&
      (list.find ("<011>") != string::npos) &&
      (list.find ("<100>") != string::npos) &&
      (list.find ("<101>") != string::npos) &&
      (list.find ("<110>") != string::npos) &&
      (list.find ("<111>") != string::npos) &&
      (es1.number_of_minima() == 8) &&
      (es2.number_of_minima() == 1024) &&
      (es3.number_of_minima() == 2) )
    return true;
  else
    return false;

  }


  bool it_should_find_a_global_minimum ()
  {
    ElementSet set1 ("set", "input/subset_sum/Test_03_A.xml");
    ElementSet set2 ("set", "input/subset_sum/Test_09_A.xml");
    ElementSet set3 ("set", "input/hamming_distance/Test_07_A.xml");
    ElementSet set4 ("set", "input/explicit/Test_03_B.xml");
    SubsetSum c1 (&set1);
    SubsetSum c2 (&set2);
    HammingDistance c3 (&set3);
    Explicit c4 (&set4);
    ExhaustiveSearch es1;
    ExhaustiveSearch es2;
    ExhaustiveSearch es3;
    ExhaustiveSearch es4;

    es1.set_parameters (&c1, &set1, false);
    es1.get_minima_list (1);
    if (es1.print_list_of_minima ().find ("<010>") == string::npos)
      return false;

    es2.set_parameters (&c2, &set2, false);
    es2.get_minima_list (10);
    if ((es2.print_list_of_minima ().find ("<000010000>") == string::npos) ||
        (es2.print_list_of_minima ().find ("<100100000>") == string::npos) ||
        (es2.print_list_of_minima ().find ("<011000000>") == string::npos) )
      return false;

    es3.set_parameters (&c3, &set3, false);
    es3.get_minima_list (1);
    if (es3.print_list_of_minima ().find ("<0011100>") == string::npos)
      return false;

    es4.set_parameters (&c4, &set4, false);
    es4.get_minima_list (1);
    if (es4.print_list_of_minima ().find ("<101>") == string::npos)
      return false;

    return true;
  }


  bool it_should_store_all_the_visited_subsets ()
  {
    ElementSet set1 ("S1", 3, 1);    // |S1| = 3
    ExhaustiveSearch es1;
    SubsetSum c1 (&set1);
    string list;
    es1.set_parameters (&c1, &set1, true);
    es1.get_minima_list (1);
    list = es1.print_list_of_visited_subsets ();
    if ((list.find ("<000>") != string::npos) &&
        (list.find ("<001>") != string::npos) &&
        (list.find ("<010>") != string::npos) &&
        (list.find ("<011>") != string::npos) &&
        (list.find ("<100>") != string::npos) &&
        (list.find ("<101>") != string::npos) &&
        (list.find ("<110>") != string::npos) &&
        (list.find ("<111>") != string::npos))
      return true;
    else
      return false;
  }


  bool it_should_give_the_number_of_the_visited_subsets ()
  {
    ElementSet set1 ("S1", 3, 1);    // |S1| = 3
    ExhaustiveSearch es1;
    SubsetSum c1 (&set1);
    es1.set_parameters (&c1, &set1, false);
    es1.get_minima_list (1);
    if (es1.print_number_of_visited_subsets () >= 8)
      return true;
    else
      return false;
  }


} // End of namespace.
