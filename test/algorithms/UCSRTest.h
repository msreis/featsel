//
// UCSRTest.h -- definition of the namespace "UCSRTest".
//
//    This file is part of the featsel program
//    Copyright (C) 2016 Gustavo Estrela
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

#ifndef UCSR_TEST_H_
#define UCSR_TEST_H_

#include "../../src/algorithms/UCSR.h"
#include "../../src/functions/SubsetSum.h"
#include "../../src/functions/HammingDistance.h"
#include "../../src/functions/Explicit.h"

namespace UCSRTest
{
  bool it_should_find_a_global_minimum ();

  bool it_should_converge_for_large_hamming_sets ();

  bool it_should_converge_for_large_subset_sum_sets ();

  bool it_should_always_give_the_correct_answer ();
}

#endif /* UCSR_TEST_H_ */
