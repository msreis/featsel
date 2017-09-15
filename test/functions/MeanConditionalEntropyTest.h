//
// MeanConditionalEntropyTest.h -- definition of the namespace
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

#ifndef MEAN_CONDITIONAL_ENTROPY_TEST_H_
#define MEAN_CONDITIONAL_ENTROPY_TEST_H_

#include "../../src/functions/MeanConditionalEntropy.h"
#include "../../src/ElementSet.h"
#include "../../src/ElementSubset.h"
#include "../../src/global.h"

#include "MeanConditionalEntropyMock.h"

namespace MeanConditionalEntropyTest
{
  bool an_empty_set_should_have_cost_zero ();

  bool it_should_add_and_retrieve_the_elapsed_time ();

  //

  bool it_should_store_the_frequencies_of_the_samples ();

  bool it_should_store_the_label_frequencies_of_the_samples ();

  bool it_should_calculate_the_conditional_entropy ();

  bool it_should_calculate_the_mean_conditional_entropy ();
}

#endif /* MEAN_CONDITIONAL_ENTROPY_TEST_H_ */

