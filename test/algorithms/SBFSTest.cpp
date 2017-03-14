//
// SBFSTest.cpp -- implementation of the namespace "SBFSTest".
//
//    This file is part of the featsel program
//    Copyright (C) 2017 Marcelo S. Reis
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

#include "SBFSTest.h"

namespace SBFSTest
{

  bool it_should_add_the_best_element ()
  {
    ElementSet set ("S", "input/hamming_distance/Test_07_A.xml");
    ElementSubset X ("X", &set);
    SBFSMock s;
    HammingDistance c (&set);

    // Best subset: 0011100.

    X.set_complete_subset ();
    s.set_parameters (&c, &set, 0);
    X.remove_element (0);
    X.remove_element (1);
    X.remove_element (5); // X = 0011101

    if (s.get_best_element_test (&X) != 6)
      return false;

    X.remove_element (6); // now no element can be removed to improve c(X)!

    if (s.get_best_element_test (&X) != set.get_set_cardinality ())
      return false;

    return true;
  }


  bool it_should_remove_the_worst_element ()
  {
    ElementSet set ("S", "input/hamming_distance/Test_07_A.xml");
    ElementSubset X ("X", &set);
    SBFSMock s;
    HammingDistance c (&set);
    s.set_parameters (&c, &set, 0);

    // Best subset: 0011100.

    X.set_complete_subset ();
    X.remove_element (1);
    X.remove_element (3); // X = 1010111

    if (s.get_worst_element_test (&X) != 1)
      return false;

    X.add_element (1); // now no element can be removed to worse c(X)!
    if (s.get_worst_element_test (&X) != set.get_set_cardinality ())
      return false;

    return true;
  }


  bool it_should_find_a_local_minimum ()
  {
    ElementSet set1 ("set", "input/subset_sum/Test_03_A.xml");
    ElementSet set3 ("set", "input/hamming_distance/Test_07_A.xml");
    SBFS SBFS1;
    SBFS SBFS3;
    SubsetSum c1 (&set1);
    HammingDistance c3 (&set3);

    SBFS1.set_parameters (&c1, &set1, false);
    SBFS1.get_minima_list (1);
    if (SBFS1.print_list_of_minima ().find ("<010>") == string::npos)
      return false;

    SBFS3.set_parameters (&c3, &set3, false);
    SBFS3.get_minima_list (1);
    if (SBFS3.print_list_of_minima ().find ("<0011100>") == string::npos)
      return false;

    return true;
  }


  bool it_should_store_all_the_visited_subsets ()
  {
    ElementSet set1 ("S1", 3, 1);    // |S1| = 3
    SBFS SBFS1;
    string list;
    SubsetSum c1 (&set1);
    SBFS1.set_parameters (&c1, &set1, true);
    SBFS1.get_minima_list (1);
    list = SBFS1.print_list_of_visited_subsets ();
    //
    // For 2^3 it should have at least 7 elements (# visited nodes by SFS)!
    //
    if ((list.find ("<000>") != string::npos) &&
        (list.find ("<001>") != string::npos) &&
        (list.find ("<011>") != string::npos) &&
        (list.find ("<111>") != string::npos))
      return true;
    else
      return false;
  }


  // It was tested up to 2^64, but for large n it may take a LONG time... :-)
  //
  bool it_should_converge_for_large_hamming_sets ()
  {
    unsigned int i, n = 64;
    string minimum = " <";
    ElementSet set1 ("set", n, 2);  // rand() % 2 results in Hamming instance.
    SBFS SBFS1;
    HammingDistance c1 (&set1);
    SBFS1.set_parameters (&c1, &set1, false);
    SBFS1.get_minima_list (1);
    for (i = 0; i < n; i++)
    { // gets the minimum from the set1
      if ((set1.get_element (i))->get_element_value (0) == 0)
        minimum.append ("0");
      else
        minimum.append ("1");
    }
    minimum.append("> ");
    if (SBFS1.print_list_of_minima ().find (minimum) == string::npos)
      return false;
    return true;
  }


  bool it_should_give_the_number_of_the_visited_subsets ()
  {
    ElementSet set1 ("S1", 3, 1);    // |S1| = 3
    SBFS SBFS;
    SubsetSum c1 (&set1);
    SBFS.set_parameters (&c1, &set1, true);
    SBFS.get_minima_list (1);
    if ((SBFS.print_list_of_visited_subsets ().size () /
        (set1.get_set_cardinality() + 4)) >= 7)
      return true;
    else
      return false;
  }


} // end of namespace
