//
// ExplicitTest.cpp -- implementation of the namespace "ExplicitTest".
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

#include "ExplicitTest.h"

namespace ExplicitTest
{

  bool it_should_works_with_an_explicitly_declared_cost_function ()
  {
    ElementSet set_1 ("S", "input/explicit/Test_03_B.xml");
    ElementSet set_2 ("S", "input/explicit/Test_02_A.xml");
    Explicit c1 (& set_1);
    Explicit c2 (& set_2);
    ElementSubset X ("X", & set_1);
    ElementSubset Y ("Y", & set_2);

    if (c1.cost (&X) != 5)
      return false;

    X.add_element (0);
    if (c1.cost (&X) != 4)
      return false;

    X.add_element (2);
    if (c1.cost (&X) != 1)
      return false;

    X.add_element (1);
    if (c1.cost (&X) != 7)
      return false;

    // testing the storage of floating point values
    if (c2.cost (&Y) != (float) 0.77)
      return false;

    Y.add_element (0);
    if (c2.cost (&Y) != 0)
      return false;

    Y.add_element (1);
    if (c2.cost (&Y) != 3)
      return false;

    Y.remove_element (0);
    if (c2.cost (&Y) != (float) 1.2)
      return false;

    return true;
  }


  bool it_should_add_and_retrieve_the_elapsed_time ()
  {
    ElementSet set ("a_set", 1, 1);
    Explicit c (& set);
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
    c.get_the_elapsed_time_of_the_calls_of_this_cost_function ();
    if (!(elapsed_time >= 0 && elapsed_time <= 3500)) // 2 ms precision
			return false;

    // elapsed_time = 1520 microseconds
    c.add_elapsed_time (t1);
    c.add_elapsed_time (t1);
    elapsed_time =
    c.get_the_elapsed_time_of_the_calls_of_this_cost_function ();
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
    Explicit c (&set);

    if (c.cost (&O) != 0)
      return false;

    return true;
  }

} // end of namespace

