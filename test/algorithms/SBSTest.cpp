//
// SBSTest.cpp -- implementation of the namespace "SBSTest".
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

#include "SBSTest.h"

namespace SBSTest
{

  bool it_should_store_all_the_visited_subsets ()
  {
    ElementSet set ("S1", 3, 1);    // |S1| = 3
    SBS t;
    string list;
    SubsetSum c (&set);
    t.set_parameters (&c, &set, true);
    t.get_minima_list (1);
    list = t.print_list_of_visited_subsets ();
    //
    // For 2^3 it should have at least 7 elements (# visited nodes by SBS)!
    //
    if ((list.find ("<111>") != string::npos) &&
        (list.find ("<101>") != string::npos) &&
        (list.find ("<011>") != string::npos) &&
        (list.find ("<110>") != string::npos))
      return true;
    else
      return false;
  }


  bool it_should_give_the_number_of_the_visited_subsets ()
  {
    ElementSet set ("S1", 3, 1);    // |S1| = 3
    SBS t;
    SubsetSum c (&set);
    t.set_parameters (&c, &set, true);
    t.get_minima_list (1);
    if ((t.print_list_of_visited_subsets ().size () /
        (set.get_set_cardinality() + 4)) >= 7)
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
    ElementSet set ("set", n, 2);  // rand() % 2 results in Hamming instance.
    SBS t;
    HammingDistance c (&set);
    t.set_parameters (&c, &set, false);
    t.get_minima_list (1);
    for (i = 0; i < n; i++)
    { // gets the minimum from the set1
      if ((set.get_element (i))->get_element_value (0) == 0)
        minimum.append ("0");
      else
        minimum.append ("1");
    }
    minimum.append("> ");
    if (t.print_list_of_minima ().find (minimum) == string::npos)
      return false;
    return true;
  }

  bool it_should_find_a_local_minimum ()
  {
    ElementSet set1 ("set", "input/subset_sum/Test_03_A.xml");
    ElementSet set3 ("set", "input/hamming_distance/Test_07_A.xml");
    SBS s1;
    SBS s3;
    SubsetSum c1 (&set1);
    HammingDistance c3 (&set3);

    s1.set_parameters (&c1, &set1, false);
    s1.get_minima_list (1);
    if (s1.print_list_of_minima ().find ("<010>") == string::npos)
      return false;

    s3.set_parameters (&c3, &set3, false);
    s3.get_minima_list (1);
    if (s3.print_list_of_minima ().find ("<0011100>") == string::npos)
      return false;

    return true;
  }


} // end of namespace
