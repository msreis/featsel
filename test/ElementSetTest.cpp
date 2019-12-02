//
// ElementSetTest.cpp -- implementation of the namespace "ElementSetTest".
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

#include "ElementSetTest.h"

namespace ElementSetTest
{

  bool a_set_should_have_a_name ()
  {
    ElementSet set1 ("a name");
    ElementSet set2 ("");
    if ((set1.get_set_name ().compare ("a name") == 0) &&
        (set2.get_set_name ().compare ("S") == 0))
      return true;
    else
      return false;
  }


  bool an_empty_set_should_not_have_elements ()
  {
    ElementSet set1 ("S1");
    ElementSet set2 ("S2", 0,3);
    ElementSet set3 ("S3", "input/parsers/Test_XmlParserDriver_B.xml");
    if ((set1.get_set_cardinality () == 0) &&
        (set2.get_set_cardinality () == 0) &&
        (set3.get_set_cardinality () == 0))
      return true;
    else
      return false;
  }


  bool it_should_load_data_of_a_set_from_file ()
  {
    ElementSet set ("S1", "input/parsers/Test_XmlParserDriver_A.xml");
    if ( (set.get_set_cardinality () == 3) &&
	 (!set.get_element (0)->add_element_value (77) ) &&
         (set.get_element (0)->get_max_number_of_values () == 3) &&
         (set.get_element (1)->get_element_value (1) == -2) &&
         (set.get_element (1)->get_number_of_values () == 3) &&
         (set.get_element (2)->get_element_name ().compare ("x02") == 0))
      return true;
    else
      return false;
  }


  bool it_should_load_data_of_a_set_from_a_DAT_file ()
  {
    ElementSet set ("input/parsers/Test_DatParserDriver_A.dat", 7);
    if ((set.get_set_cardinality () == 7) &&
        (set.get_element (0)->get_max_number_of_values () == 4) &&
        (set.get_element (7)->get_element_value(3) == 8) && // W-operator label
        (set.get_element (7)->get_element_value(0) == 2) && // W-operator label
        (set.get_element (0)->get_element_value(3) == 0) &&
        (set.get_element (5)->get_element_value(1) == 0))
      return true;
    else
      return false;
  }


  bool it_should_create_a_set_with_random_values ()
  {
    ElementSet set ("S1", 2, 1000);
    if ( (set.get_set_cardinality () == 2) &&
         (set.get_set_name ().compare ("S1") == 0) &&
         (set.get_element (0)->get_element_name ().compare ("elem-0") == 0) &&
         (set.get_element (0)->get_number_of_values () == 1) &&
         (set.get_element (1)->get_element_name ().compare ("elem-1") == 0) &&
         (set.get_element (1)->get_number_of_values () == 1))
      return true;
    else
      return false;
  }


  bool it_should_return_the_set_cardinality ()
  {
    ElementSet set1 ("S1", 3, 1000);
    ElementSet set2 ("S2");
    if ( (set1.get_set_cardinality () == 3) &&
         (set2.get_set_cardinality () == 0) )
      return true;
    else
      return false;
  }


  bool it_should_not_return_an_element_that_not_belongs_to_the_set ()
  {
    ElementSet set ("S1", 2, 1000);
    if (set.get_element (3) == NULL)
      return true;
    else
      return false;
  }


  bool it_should_return_an_element_that_belongs_to_the_set ()
  {
    ElementSet set ("S1", 2, 1000);
    if (set.get_element (1)->get_element_name ().compare ("elem-1") == 0)
      return true;
    else
      return false;
  }


  bool values_from_a_random_set_should_be_within_the_given_range ()
  {
    int range = 1000;
    unsigned int i, count_zeros = 0;
    ElementSet set ("S1", 10000, range);
    for (i = 0; i < 10000; i++)
    {
      if ((set.get_element (i)->get_element_value (0) >= range) ||
          (set.get_element (i)->get_element_value (0) < 0) )
        return false;
      if (set.get_element (i)->get_element_value (0) == 0)
        count_zeros++;
    }
    if (count_zeros > 1000)
      cout << "\nWarning: too many zeros in ElementSet () : " <<
      count_zeros << "!" << endl;
    return true;
  }


} // end namespace
