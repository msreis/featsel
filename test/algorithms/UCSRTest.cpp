//
// UCSRTest.cpp -- implementation of the namespace "UCSRTest".
//
//    This file is part of the featsel program
//    Copyright (C) 2016 Gustavo Estrela
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

#include "UCSRTest.h"

namespace UCSRTest
{
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
    UCSR ucsr1;
    UCSR ucsr2;
    UCSR ucsr3;
    UCSR ucsr4;

    ucsr1.set_parameters (&c1, &set1, false);
    ucsr1.get_minima_list (1);
    if (ucsr1.print_list_of_minima ().find ("<010>") == string::npos)
      return false;

    ucsr2.set_parameters (&c2, &set2, false);
    ucsr2.get_minima_list (10);
    if ((ucsr2.print_list_of_minima ().find ("<000010000>") == string::npos) ||
        (ucsr2.print_list_of_minima ().find ("<100100000>") == string::npos) ||
        (ucsr2.print_list_of_minima ().find ("<011000000>") == string::npos) )
      return false;

    ucsr3.set_parameters (&c3, &set3, false);
    ucsr3.get_minima_list (1);
    if (ucsr3.print_list_of_minima ().find ("<0011100>") == string::npos)
      return false;

    ucsr4.set_parameters (&c4, &set4, false);
    ucsr4.get_minima_list (1);
    if (ucsr4.print_list_of_minima ().find ("<101>") == string::npos)
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
    UCSR ucsr1;
    HammingDistance c1 (&set1);
    ucsr1.set_parameters (&c1, &set1, false);
    ucsr1.get_minima_list (1);
    for (i = 0; i < n; i++)
    {       // gets the minimum from the set1
      if ((set1.get_element (i))->get_element_value (0) == 0)
        minimum.append ("0");
      else
        minimum.append ("1");
    }
    minimum.append ("> ");
    if (ucsr1.print_list_of_minima ().find (minimum) == string::npos)
      return false;
    return true;
  }


  // It was tested up to 2^32, but for large n it may take a LONG time... :-)
  //
  bool it_should_converge_for_large_subset_sum_sets ()
  {
    unsigned int n = 16;
    ElementSet set1 ("set", n, 10000); // subset sum instance.
    UCSR ucsr1;
    SubsetSum c1 (&set1);
    ucsr1.set_parameters (&c1, &set1, false);
    ucsr1.get_minima_list (1);
    return true;
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
    UCSR * ucsr;
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
      ucsr = new UCSR ();
      ucsr->set_parameters (&c1, &set1, false);
      ucsr->get_minima_list (1);
      result.clear ();
      result = ucsr->print_list_of_minima ();
      delete ucsr;
      if (result.find ("<0110>") == string::npos)
        return false;
    }
    for (i = 1; i <= k; i++)
    {
      ucsr = new UCSR ();
      ucsr->set_parameters (&c2, &set2, false);
      ucsr->get_minima_list (1);
      result.clear ();
      result = ucsr->print_list_of_minima ();
      delete ucsr;
      if (result.find ("<0011>") == string::npos)
        return false;
    }
    for (i = 1; i <= k; i++)
    {
      ucsr = new UCSR ();
      ucsr->set_parameters (&c3, &set3, false);
      ucsr->get_minima_list (1);
      result.clear ();
      result = ucsr->print_list_of_minima ();
      delete ucsr;
      if (result.find ("<010001100>") == string::npos)
        return false;
    }
    for (i = 1; i <= k; i++)
    {
      ucsr = new UCSR ();
      ucsr->set_parameters (&c4, &set4, false);
      ucsr->get_minima_list (1);
      result.clear ();
      result = ucsr->print_list_of_minima ();
      delete ucsr;
      if (result.find ("<0101101>") == string::npos)
        return false;
    }
    for (i = 1; i <= k; i++)
    {
      ucsr = new UCSR ();
      ucsr->set_parameters (&c5, &set5, false);
      ucsr->get_minima_list (1);
      result.clear ();
      result = ucsr->print_list_of_minima ();
      delete ucsr;
      if (result.find ("<0000001000010>") == string::npos)
        return false;
    }
    for (i = 1; i <= k; i++)
    {
      ucsr = new UCSR ();
      ucsr->set_parameters (&c6, &set6, false);
      ucsr->get_minima_list (1);
      result.clear ();
      result = ucsr->print_list_of_minima ();
      delete ucsr;
      if (result.find ("<0001101>") == string::npos)
        return false;
    }

    return true;
  }

} // end of namespace
