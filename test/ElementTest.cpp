//
// ElementTest.cpp -- implementation of the namespace "ElementTest".
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


#include "ElementTest.h"

namespace ElementTest
{

  bool an_element_should_have_a_name ()
  {
    Element elem1 (0,"a name");
    Element elem2 (0, "");
    if ((elem1.get_element_name ().compare ("a name") == 0) &&
        (elem2.get_element_name ().compare ("s") == 0))
      return true;
    else
      return false;
  }


  bool a_new_element_should_not_have_added_values ()
  {
    Element elem1 (0, "");
    Element elem2 (1000, "");
    if ((elem1.get_number_of_values () == 0) &&
        (elem2.get_number_of_values () == 0))
      return true;
    else
      return false;
  }


  bool an_element_without_all_values_added_should_allow_adding ()
  {
    Element elem (2, "");
    elem.add_element_value (6);
    if (elem.add_element_value (7))
      return true;
    else
      return false;
  }


  bool an_element_with_all_values_added_should_not_allow_adding ()
  {
    Element elem (1, "");
    elem.add_element_value (1);
    if (!elem.add_element_value (2))
      return true;
    else
      return false;
  }


  bool an_added_value_should_be_found ()
  {
    Element elem (2, "");
    elem.add_element_value (3);
    elem.add_element_value (4);
    if((elem.get_element_value (0) == 3) &&
       (elem.get_element_value (1) == 4))
      return true;
    else
      return false;
  }


} // end namespace
