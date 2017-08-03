//
// TailorConvexHullTest.cpp -- implementation of the namespace "TailorConvexHullTest".
//
//    This file is part of the featsel program
//    Copyright (C) 2017  Marcelo S. Reis
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

#include "TailorConvexHullTest.h"

namespace TailorConvexHullTest
{
  bool it_should_add_and_retrieve_the_elapsed_time ()
  {
    ElementSet set ("a_set", 1, 1);
    TailorConvexHull c (& set);
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
    TailorConvexHull c (&set);

    if (c.cost (&O) != 0)
      return false;

    return true;
  }

  bool it_should_fit_pieces_into_the_cloth ()
  {
    ElementSet set1 ("set", "input/tailor/Test_03_A.xml");
    ElementSet set2 ("set", "input/tailor/Test_03_B.xml");
    ElementSet set3 ("set", "input/tailor/Test_07_A.xml");
    TailorConvexHull c1 (& set1), c2 (& set2), c3 (& set3);
    ElementSubset X ("", & set1), Y ("", & set2), Z ("", & set3);
    double cost;

    // Testing c1:
    //
    X.set_empty_subset ();                  // 000
    cost = c1.cost (& X);
    if (cost != -1)                         // cost = -1
      return false;

    X.add_element (0);                      // 100
    cost = c1.cost (& X);
    if ((cost > -1.3) || (cost <= -1.4))    // cost = -1.333...
      return false;

    X.remove_element (0);                   // 010
    X.add_element (1);
    cost = c1.cost (& X);
    if ((cost > -1.6) || (cost <= -1.7))    // cost = -1.666...
      return false;

    X.remove_element (1);                   // 001
    X.add_element (2);
    cost = c1.cost (& X);
    if ((cost > -1.6) || (cost <= -1.7))    // cost = -1.666...
      return false;

    X.add_element (1);                      // 011
    cost = c1.cost (& X);
    if ((cost > -2.3) || (cost <= -2.4))    // cost = -2.333...
      return false;

    X.add_element (0);                      // 101
    X.remove_element (1);
    cost = c1.cost (& X);
    if (cost != -2)                         // cost = -2
      return false;

    X.add_element (1);                      // 110
    X.remove_element (2);
    cost = c1.cost (& X);
    if (cost != -2)                         // cost = -2
      return false;

    X.set_complete_subset ();               // 111
    cost = c1.cost (& X);
    if (cost != INFTY)                      // cost = infinity
      return false;

    // Testing c2:
    //
    Y.set_empty_subset ();                  // 000
    cost = c2.cost (& Y);
    if (cost != -1)                         // cost = -1
      return false;

    Y.add_element (0);                      // 100
    cost = c2.cost (& Y);
    if ((cost > -1.3) || (cost <= -1.4))    // cost = -1.333...
      return false;

    Y.add_element (1);                      // 110
    cost = c2.cost (& Y);
    if (cost != INFTY)                      // cost = infinity
      return false;

    Y.remove_element (1);
    Y.add_element (2);                      // 101
    cost = c2.cost (& Y);
    if (cost != -2) // cost = -2
      return false;

    Y.set_complete_subset ();               // 111
    cost = c2.cost (& Y);
    if (cost != INFTY)                      // cost = infinity
      return false;

    // Testing c3:
    //
    Z.set_empty_subset ();                  // 0000000
    cost = c3.cost (& Z);
    if (cost != -1)                         // cost = -1
      return false;

    Z.add_element (0);                      // 1000000
    cost = c3.cost (& Z);
    if (cost != -1.625)                     // cost = -1.625
      return false;

    Z.add_element (4);                      // 1000100
    cost = c3.cost (& Z);
    if (cost != -2.5)                       // cost = -2.5
      return false;

    Z.add_element (6);                      // 1000101
    cost = c3.cost (& Z);
    if (cost != -3 )                        // cost = -3
      return false;

    Z.add_element (5);                      // 1000111
    cost = c3.cost (& Z);
    if (cost != INFTY)                      // cost = infinity
      return false;

    return true;
  }

} // End of namespace.

