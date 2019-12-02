//
// ElementSetTest.h -- definition of the namespace "ElementSetTest".
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

#ifndef ELEMENTSETTEST_H_
#define ELEMENTSETTEST_H_

#include "../src/Element.h"
#include "../src/ElementSet.h"
#include "../src/global.h"

namespace ElementSetTest
{

  bool a_set_should_have_a_name ();

  bool an_empty_set_should_not_have_elements ();

  bool it_should_load_data_of_a_set_from_file ();

  bool it_should_load_data_of_a_set_from_a_DAT_file ();

  bool it_should_create_a_set_with_random_values ();

  bool it_should_return_the_set_cardinality ();

  bool it_should_return_an_element_that_belongs_to_the_set ();

  bool it_should_not_return_an_element_that_not_belongs_to_the_set ();

  bool values_from_a_random_set_should_be_within_the_given_range ();

}

#endif /* ELEMENTSETTEST_H_ */
