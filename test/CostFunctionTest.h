//
// CostFunctionTest.h -- definition of the namespace "CostFunctionTest".
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

#ifndef COSTFUNCTIONTEST_H_
#define COSTFUNCTIONTEST_H_

#include "../src/functions/Explicit.h"
#include "../src/functions/MeanConditionalEntropy.h"
#include "../src/ElementSet.h"
#include "../src/ElementSubset.h"
#include "../src/global.h"
#include "CostFunctionMock.h"

namespace CostFunctionTest
{
  bool it_should_works_with_an_explicitly_declared_cost_function ();

  bool it_should_store_the_frequencies_of_the_samples ();

  bool it_should_store_the_label_frequencies_of_the_samples ();

  bool it_should_calculate_the_conditional_entropy ();

  bool it_should_calculate_the_mean_conditional_entropy ();

  bool it_should_add_and_retrieve_the_elapsed_time ();

}

#endif /* COSTFUNCTIONTEST_H_ */
