//
// SubsetSumTest.h -- definition of the namespace "SubsetSumTest".
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

#ifndef SUBSET_SUM_TEST_H_
#define SUBSET_SUM_TEST_H_

#include "../../src/global.h"
#include "../../src/ElementSet.h"
#include "../../src/ElementSubset.h"

#include "../../src/functions/SubsetSum.h"

namespace SubsetSumTest
{
  bool an_empty_set_should_have_cost_zero ();
  
  bool it_should_add_and_retrieve_the_elapsed_time ();

  bool it_should_give_the_subset_sum_cost_of_a_subset ();
}

#endif /* SUBSET_SUM_TEST_H_ */

