//
// ElementTest.h -- definition of the namespace "ElementTest".
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

#ifndef ELEMENTTEST_H_
#define ELEMENTTEST_H_

#include "../src/Element.h"
#include "../src/global.h"

namespace ElementTest
{

  bool an_element_should_have_a_name ();

  bool a_new_element_should_not_have_added_values ();

  bool an_element_without_all_values_added_should_allow_adding ();

  bool an_element_with_all_values_added_should_not_allow_adding ();

  bool an_added_value_should_be_found ();

}

#endif /* ELEMENTTEST_H_ */
