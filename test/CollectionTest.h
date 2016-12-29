//
// CollectionTest.h -- definition of the namespace "CollectionTest".
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

#ifndef COLLECTIONTEST_H_
#define COLLECTIONTEST_H_

#include "../src/Collection.h"
#include "../src/CostFunction.h"
#include "../src/ElementSet.h"
#include "../src/ElementSubset.h"
#include "../src/global.h"

namespace CollectionTest
{

  bool a_lower_covered_subset_should_be_found ();

  bool a_non_lower_covered_subset_should_not_be_found ();

  bool it_should_remove_lower_covered_subsets_in_a_collection ();

  bool an_upper_covered_subset_should_be_found ();

  bool a_non_upper_covered_subset_should_not_be_found ();

  bool it_should_remove_upper_covered_subsets_in_a_collection ();

  bool the_evaluated_subset_should_not_be_deleted ();

  bool it_should_copy_a_collection ();

}

#endif /* COLLECTIONTEST_H_ */
