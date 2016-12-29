//
// HammingDistanceTest.cpp -- implementation of the namespace
// "HammingDistanceTest".
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

#include "HammingDistanceTest.h"

namespace HammingDistanceTest
{

  bool it_should_give_the_hamming_distance_cost_of_a_subset ()
  {
    ElementSet set ("set", "input/hamming_distance/Test_03_A.xml");
    ElementSubset I ("I", &set);
    ElementSubset O ("O", &set);
    ElementSubset X ("X", &set);
    ElementSubset Y ("Y", &set);
    ElementSubset Z ("X", &set);
    HammingDistance c (&set);

    I.set_complete_subset ();
    X.add_element (0);    // X = [101]
    X.add_element (2);
    Y.add_element (1);    // Y = [011]
    Y.add_element (2);
    Z.add_element (1);    // Z = [010]
    if ((c.cost (&I) == 1) &&
        (c.cost(&X) == 0) &&
        (c.cost(&Y) == 2) &&
        (c.cost(&Z) == 3) &&
        (c.cost(&O) == 2) )
      return true;
    else
      return false;
  }


  bool it_should_add_and_retrieve_the_elapsed_time ()
  {
    ElementSet set ("a_set", 1, 1);
    HammingDistance c (& set);
    int t1 =    10,     // 0.00001 second
        t2 =   100,     // 0.0001 second
        t3 =   500,     // 0.0005 second
        t4 =  1000,     // 0.001 second
        t5 = 2000000,   // 2 seconds
        elapsed_time;

    // elapsed_time = 1500 microseconds
    c.add_elapsed_time (t3);
    c.add_elapsed_time (t4);
    elapsed_time =
    c.get_the_elapsed_time_of_the_calls_of_this_cost_function();
    if (!(elapsed_time >= 0 && elapsed_time <= 3500)) // 2 ms precision
			return false;

    // elapsed_time = 1520 microseconds
    c.add_elapsed_time (t1);
    c.add_elapsed_time (t1);
    elapsed_time =
    c.get_the_elapsed_time_of_the_calls_of_this_cost_function();
    if (!(elapsed_time >= 0 && elapsed_time <= 3520)) // 2 ms precision
      return false;

    // elapsed_time = 2120 microseconds
    c.add_elapsed_time (t3);
    c.add_elapsed_time (t2);
    elapsed_time =
    c.get_the_elapsed_time_of_the_calls_of_this_cost_function();
    if (!(elapsed_time >= 120 && elapsed_time <= 4120)) // 2 ms precision
      return false;

    // elapsed_time = 2002120 microseconds
    c.add_elapsed_time (t5);
    elapsed_time =
    c.get_the_elapsed_time_of_the_calls_of_this_cost_function ();
    if (!(elapsed_time >= 2000120 && elapsed_time <= 2004120)) // 2ms precision
      return false;

    return true;
  }


  bool an_empty_set_should_have_cost_zero ()
  {
    ElementSet set ("set", "input/parsers/Test_XmlParserDriver_B.xml");
    ElementSubset O ("O", &set);
    HammingDistance c (&set);

    if (c.cost (&O) != 0)
      return false;

    return true;
  }

} // end of namespace

