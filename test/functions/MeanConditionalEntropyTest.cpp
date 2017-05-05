//
// MeanConditionalEntropyTest.cpp -- implementation of the namespace
// "MeanConditionalEntropyTest".
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

#include "MeanConditionalEntropyTest.h"

namespace MeanConditionalEntropyTest
{

  bool it_should_store_the_frequencies_of_the_samples ()
  {
    ElementSet set ("input/mce/Test_07_04_A.dat", 7);
    ElementSubset X ("", & set);
    MeanConditionalEntropyMock c (& set);
    string samples_frequencies;

    X.set_complete_subset ();

    samples_frequencies = c.print_W_operator_samples_frequencies (& X);
    if (!((samples_frequencies.find ("0001001: 1") != string::npos) &&
          (samples_frequencies.find ("0101011: 2") != string::npos) &&
          (samples_frequencies.find ("0111110: 1") != string::npos) &&
          (samples_frequencies.find ("1111111: ") == string::npos) &&
          (samples_frequencies.find ("0000000: ") == string::npos)))
      return false;

    // |X| must be greater than 0
    //
    X.set_empty_subset ();  
    X.add_element(1);
    X.add_element(2);
    X.add_element(3);
    samples_frequencies = c.print_W_operator_samples_frequencies (& X);
    if (!((samples_frequencies.find ("X101XXX: 2") != string::npos) &&
          (samples_frequencies.find ("X001XXX: 1") != string::npos) &&
          (samples_frequencies.find ("X111XXX: 1") != string::npos) &&
          (samples_frequencies.find ("X000XXX: ") == string::npos) &&
          (samples_frequencies.find ("X110XXX: ") == string::npos)))
      return false;

    X.set_empty_subset ();
    X.add_element(0);
    X.add_element(2);
    X.add_element(4);
    X.add_element(6);
    samples_frequencies = c.print_W_operator_samples_frequencies (& X);
    if (!((samples_frequencies.find ("0X0X0X1: 3") != string::npos) &&
          (samples_frequencies.find ("0X1X1X0: 1") != string::npos) &&
          (samples_frequencies.find ("1X0X1X0: ") == string::npos) &&
          (samples_frequencies.find ("0X0X0X0: ") == string::npos) &&
          (samples_frequencies.find ("1X1X1X1: ") == string::npos)))
      return false;

    X.set_empty_subset ();
    X.add_element(5);
    samples_frequencies = c.print_W_operator_samples_frequencies (& X);
    if (!((samples_frequencies.find ("XXXXX1X: 3") != string::npos) &&
          (samples_frequencies.find ("XXXXX0X: 1") != string::npos)))
      return false;

    return true;
  }


  bool it_should_store_the_label_frequencies_of_the_samples ()
  {
    ElementSet set ("input/mce/Test_03_13_B.dat", 3);
    ElementSubset X ("", & set);
    MeanConditionalEntropyMock c (& set);
    string samples_frequencies;

    // |X| must be greater than 0
    //
    X.set_complete_subset ();
    samples_frequencies = c.print_label_frequencies (& X);
    if (!((samples_frequencies.find ("011: 2 0") != string::npos) &&
          (samples_frequencies.find ("110: 1 2") != string::npos) &&
          (samples_frequencies.find ("010: 2 2") != string::npos) &&
          (samples_frequencies.find ("000: 3 2") != string::npos) &&
          (samples_frequencies.find ("111: ") == string::npos)))
      return false;

    // |X| must be greater than 0
    //
    X.remove_element (0);
    samples_frequencies = c.print_label_frequencies (& X);
    if (!((samples_frequencies.find ("X00: 3 2") != string::npos) &&
          (samples_frequencies.find ("X10: 3 4") != string::npos) &&
          (samples_frequencies.find ("X11: 2 0") != string::npos) &&
          (samples_frequencies.find ("X01: ") == string::npos)))
      return false;

    return true;
  }


  bool it_should_calculate_the_conditional_entropy ()
  {
    ElementSet set ("input/mce/Test_03_09_C.dat", 3);
    ElementSubset X ("", & set);
    MeanConditionalEntropyMock c (& set);
    string entropy;

    X.set_complete_subset ();
    entropy = c.print_conditional_entropy (& X);

    if (!((entropy.find ("011: 0.918296") != string::npos) &&
          (entropy.find ("000: 0.970951") != string::npos) &&
          (entropy.find ("010: 0") != string::npos) &&
          (entropy.find ("111: ") == string::npos)))
      return false;
    else
      return true;
  }


  bool it_should_calculate_the_mean_conditional_entropy ()
  {
    ElementSet set_1 ("input/mce/Test_03_09_C.dat", 3);
    ElementSet set_2 ("input/mce/Test_03_06_D.dat", 3);
    MeanConditionalEntropy cf_1 (& set_1);
    MeanConditionalEntropy cf_2 (& set_2);
    ElementSubset X ("X", & set_1);
    ElementSubset Y ("Y", & set_2);
    double cost;

    X.set_complete_subset ();
    cost = cf_1.cost (& X);

    if (!((cost >= 0.760) && (cost <= 0.761)))
      return false;

    X.remove_element (2);
    cost = cf_1.cost (& X);

    if (!((cost >= 0.8463) && (cost <= 0.8465)))
      return false;

    Y.set_complete_subset ();  // Testing the penalty for one occurrence.
    cost = cf_2.cost (& Y);

    if (!((cost >= 0.6666) && (cost <= 0.6668)))
      return false;

    return true;
  }


  bool it_should_add_and_retrieve_the_elapsed_time ()
  {
    ElementSet set ("a_set", 1, 1);
    MeanConditionalEntropyMock c (& set);
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
    MeanConditionalEntropy c (&set);

    if (c.cost (&O) != 0)
      return false;

    return true;
  }

} // end of namespace

