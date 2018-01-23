//
// CFSTest.cpp -- implementation of the namespace "CFSTest".
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

#include "CFSTest.h"

namespace CFSTest
{

  bool it_should_compute_the_correlation_based_feature_selection ()
  {
    // This instance is the same as "Test_3_1" in the Zoo data set, and applying
    // the CFS cost function onto it yields the following results:
    //
    // <111> : -0.61183899641036987
    // <011> : -0.57568144798278809
    // <110> : -0.54804110527038574
    // <101> : -0.54384428262710571
    // <001> : -0.51110106706619263
    // <100> : -0.46345213055610657
    // <010> : -0.35979440808296204
    // <000> : inf
    //
    ElementSet set (7, "input/cfs/Test_3_1.dat", 3);

    ElementSubset X ("X", &set);

    CFS c (&set);

    if (c.cost (&X) != INFTY)        // X = 000
      return false;

    X.add_element (0);               // X = 100 

    if ((c.cost (&X) <= -0.463453) || (c.cost (&X) >= -0.463451))
      return false;

    X.add_element (1);               // X = 110
    if ((c.cost (&X) <= -0.548042) || (c.cost (&X) >= -0.548040))
      return false;

    X.add_element (2);               // X = 111
    if ((c.cost (&X) <= -0.611840) || (c.cost (&X) >= -0.611838))
      return false;

    X.remove_element (1);            // X = 101
    if ((c.cost (&X) <= -0.543845) || (c.cost (&X) >= -0.543843))
      return false;

    X.remove_element (0);            // X = 001
    if ((c.cost (&X) <= -0.511102) || (c.cost (&X) >= -0.511100))
      return false;
    
    X.add_element (1);               // X = 011
    if ((c.cost (&X) <= -0.575682) || (c.cost (&X) >= -0.575680))
      return false;

    X.remove_element (2);            // X = 010
    if ((c.cost (&X) <= -0.359795) || (c.cost (&X) >= -0.359793))
      return false;


    // This instance is from "Arrhythmia" data set from UCI Machine Learning
    // repository (279 features, 16 labels).
    //
    ElementSet arrhythmia (16, "input/Arrhythmia/Test_01_A.dat", 279);

    CFS cf (& arrhythmia);

    ElementSubset Y ("Y", & arrhythmia);

    Y.add_element (4);
    Y.add_element (6);
    Y.add_element (14);
    Y.add_element (89);
    Y.add_element (90);
    Y.add_element (92);
    Y.add_element (99);
    Y.add_element (102);
    Y.add_element (111);
    Y.add_element (112);
    Y.add_element (166);
    Y.add_element (176);
    Y.add_element (180);
    Y.add_element (189);
    Y.add_element (206);
    Y.add_element (227);
    Y.add_element (242);
    Y.add_element (266);
    Y.add_element (276);
    Y.add_element (278); 

    // c(Y) = -0.36845877 172694702, which is close to the value obtained when
    // this instance is evaluated with the "CfsSubsetEval" class in Weka:
    //
    // -0.36845877 37903233
    //
    if ((cf.cost (&Y) > -0.36845877) || (cf.cost (&Y) < -0.36845878)) 
      return false;      

    unsigned int values[60] = {0, 1, 4, 5, 6, 7, 14, 17, 20, 29, 32, 39, 51, 52,
                              63, 64, 75, 86, 89, 90, 92, 94, 99, 101, 102, 104,
                              111, 112, 113, 120, 136, 142, 148, 159, 165, 166,
                              176, 179, 180, 189, 196, 206, 215, 216, 223, 227,
                              229, 231, 232, 233, 236, 239, 242, 249, 255, 257,
                              261, 266, 269, 276};
        
    // c(Y) = -0.41128125 696689843
    //
    // (it matches with Weka's result up to the eighth precision digit ("5")).
    //
    // Weka: 0.41128125 89422674
    //
    Y.set_empty_subset ();

    for (unsigned int i = 0; i < 60; i++)
      Y.add_element (values[i]);

    if ((cf.cost (&Y) > -0.41128125) || (cf.cost (&Y) < -0.41128126)) 
      return false;      

    return true;
  }


  bool it_should_add_and_retrieve_the_elapsed_time ()
  {
    ElementSet set ("a_set", 1, 1);
    CFS c (&set);
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
    c.get_the_elapsed_time_of_the_calls_of_this_cost_function ();
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
    CFS c (&set);

    if (c.cost (&O) != 0)
      return false;

    return true;
  }

} // end of namespace

