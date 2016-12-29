//
// UcurveBranchandBoundTest.cpp -- implementation of the namespace "UcurveBranchandBoundTest".
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

#include "UcurveBranchandBoundTest.h"

namespace UcurveBranchandBoundTest
{

  bool a_lattice_with_constant_cost_should_be_totally_exausted ()
  {
  ElementSet a_set1 ("S1", 3, 1);    // |S1| = 3
  ElementSet a_set2 ("S2", 10, 1);   // |S2| = 10
  ElementSet a_set3 ("S3", 1, 1);    // |S3| = 1
  SubsetSum c1 (&a_set1);
  SubsetSum c2 (&a_set2);
  SubsetSum c3 (&a_set3);
  UcurveBranchandBound ubb1;
  UcurveBranchandBound ubb2;
  UcurveBranchandBound ubb3;
  string list;
  ubb1.set_parameters (&c1, &a_set1, false);
  ubb1.get_minima_list (8);
  list = ubb1.print_list_of_minima ();
  ubb2.set_parameters (&c2, &a_set2, false);
  ubb2.get_minima_list (1024);
  ubb3.set_parameters (&c3, &a_set3, false);
  ubb3.get_minima_list (2);

  if ((list.find ("<000>") != string::npos) &&
      (list.find ("<001>") != string::npos) &&
      (list.find ("<010>") != string::npos) &&
      (list.find ("<011>") != string::npos) &&
      (list.find ("<100>") != string::npos) &&
      (list.find ("<101>") != string::npos) &&
      (list.find ("<110>") != string::npos) &&
      (list.find ("<111>") != string::npos) &&
      (ubb1.number_of_minima() == 8) &&
      (ubb2.number_of_minima() == 1024) &&
      (ubb3.number_of_minima() == 2) )
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
    UcurveBranchandBound ubb1;
    UcurveBranchandBound ubb2;
    UcurveBranchandBound ubb3;
    UcurveBranchandBound ubb4;

    ubb1.set_parameters (&c1, &set1, false);
    ubb1.get_minima_list (1);
    if (ubb1.print_list_of_minima ().find ("<010>") == string::npos)
      return false;

    ubb2.set_parameters (&c2, &set2, false);
    ubb2.get_minima_list (10);
    if ((ubb2.print_list_of_minima ().find ("<000010000>") == string::npos) ||
        (ubb2.print_list_of_minima ().find ("<100100000>") == string::npos) ||
        (ubb2.print_list_of_minima ().find ("<011000000>") == string::npos) )
      return false;

    ubb3.set_parameters (&c3, &set3, false);
    ubb3.get_minima_list (1);
    if (ubb3.print_list_of_minima ().find ("<0011100>") == string::npos)
      return false;

    ubb4.set_parameters (&c4, &set4, false);
    ubb4.get_minima_list (1);
    if (ubb4.print_list_of_minima ().find ("<101>") == string::npos)
      return false;

    return true;
  }


  // It was tested up to 2^32, but for large n it may take a LONG time... :-)
  //
  bool it_should_converge_for_large_hamming_sets ()
  {
    unsigned int i, n = 16; // 2^16
    string minimum = " <";
    ElementSet set1 ("set", n, 2);  // rand() % 2 results in Hamming instance.
    UcurveBranchandBound ubb1;
    HammingDistance c1 (&set1);
    ubb1.set_parameters (&c1, &set1, false);
    ubb1.get_minima_list (1);
    for (i = 0; i < n; i++)
    {       // gets the minimum from the set1
      if ((set1.get_element (i))->get_element_value (0) == 0)
        minimum.append ("0");
      else
        minimum.append ("1");
    }
    minimum.append ("> ");
    if (ubb1.print_list_of_minima ().find (minimum) == string::npos)
      return false;
    return true;
  }


  // It was tested up to 2^32, but for large n it may take a LONG time... :-)
  //
  bool it_should_converge_for_large_subset_sum_sets ()
  {
    unsigned int n = 16;
    ElementSet set1 ("set", n, 10000); // subset sum instance.
    UcurveBranchandBound ubb1;
    SubsetSum c1 (&set1);
    ubb1.set_parameters (&c1, &set1, false);
    ubb1.get_minima_list (1);
    return true;
  }


  bool it_should_store_all_the_visited_subsets ()
  {
    ElementSet set1 ("S1", 3, 1);    // |S1| = 3
    UcurveBranchandBound ubb1;
    SubsetSum c1 (&set1);
    string list;
    ubb1.set_parameters (&c1, &set1, true);
    ubb1.get_minima_list (1);
    list = ubb1.print_list_of_visited_subsets ();
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
    UcurveBranchandBound ubb1;
    SubsetSum c1 (&set1);
    ubb1.set_parameters (&c1, &set1, false);
    ubb1.get_minima_list (1);
    if (ubb1.print_number_of_visited_subsets () >= 8)
      return true;
    else
      return false;
  }


  // Repeat the test k times (default: k = 10)
  //
  bool it_should_always_give_the_correct_answer ()
  {
    ElementSet set1 ("set", "input/subset_sum/Test_04_A.xml");
    ElementSet set2 ("set", "input/subset_sum/Test_04_B.xml");
    ElementSet set3 ("set", "input/subset_sum/Test_09_B.xml");
    ElementSet set4 ("set", "input/subset_sum/Test_07_B.xml");
    ElementSet set5 ("set", "input/subset_sum/Test_13_A.xml");
    ElementSet set6 ("set", "input/subset_sum/Test_07_C.xml");
    UcurveBranchandBound * ubb;
    SubsetSum c1 (&set1);
    SubsetSum c2 (&set2);
    SubsetSum c3 (&set3);
    SubsetSum c4 (&set4);
    SubsetSum c5 (&set5);
    SubsetSum c6 (&set6);
    int i, k = 10;
    string result;

    for (i = 1; i <= k; i++)
    {
      ubb = new UcurveBranchandBound ();
      ubb->set_parameters (&c1, &set1, false);
      ubb->get_minima_list (1);
      result.clear ();
      result = ubb->print_list_of_minima ();
      delete ubb;
      if (result.find ("<0110>") == string::npos)
        return false;
    }
    for (i = 1; i <= k; i++)
    {
      ubb = new UcurveBranchandBound ();
      ubb->set_parameters (&c2, &set2, false);
      ubb->get_minima_list (1);
      result.clear ();
      result = ubb->print_list_of_minima ();
      delete ubb;
      if (result.find ("<0011>") == string::npos)
        return false;
    }
    for (i = 1; i <= k; i++)
    {
      ubb = new UcurveBranchandBound ();
      ubb->set_parameters (&c3, &set3, false);
      ubb->get_minima_list (1);
      result.clear ();
      result = ubb->print_list_of_minima ();
      delete ubb;
      if (result.find ("<010001100>") == string::npos)
        return false;
    }
    for (i = 1; i <= k; i++)
    {
      ubb = new UcurveBranchandBound ();
      ubb->set_parameters (&c4, &set4, false);
      ubb->get_minima_list (1);
      result.clear ();
      result = ubb->print_list_of_minima ();
      delete ubb;
      if (result.find ("<0101101>") == string::npos)
        return false;
    }
    for (i = 1; i <= k; i++)
    {
      ubb = new UcurveBranchandBound ();
      ubb->set_parameters (&c5, &set5, false);
      ubb->get_minima_list (1);
      result.clear ();
      result = ubb->print_list_of_minima ();
      delete ubb;
      if (result.find ("<0000001000010>") == string::npos)
        return false;
    }
    for (i = 1; i <= k; i++)
    {
      ubb = new UcurveBranchandBound ();
      ubb->set_parameters (&c6, &set6, false);
      ubb->get_minima_list (1);
      result.clear ();
      result = ubb->print_list_of_minima ();
      delete ubb;
      if (result.find ("<0001101>") == string::npos)
        return false;
    }

    return true;
  }

} // end of namespace
