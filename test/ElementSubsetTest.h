//
// ElementSubsetTest.h -- definition of the namespace "ElementSubsetTest".
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

#ifndef ELEMENTSUBSETTEST_H_
#define ELEMENTSUBSETTEST_H_

#include "../src/Element.h"
#include "../src/ElementSet.h"
#include "../src/ElementSubset.h"
#include "../src/global.h"

namespace ElementSubsetTest
{

  bool a_new_subset_should_be_an_empty_set ();

  bool an_element_not_in_subset_should_be_added ();

  bool an_element_in_subset_should_be_removed ();

  bool it_should_give_the_set_that_belongs_the_subset ();

  bool a_set_should_contains_its_subset ();

  bool a_subset_should_be_contained_by_its_set ();

  bool a_subset_should_be_successfully_cloned ();

  bool a_random_element_should_be_removed ();

  bool it_should_give_the_complement_of_the_set ();

}

#endif /* ELEMENTSUBSETTEST_H_ */
