//
// MeanConditionalEntropyMock.h -- definition of the mock class
// "MeanConditionalEntropyMock".
//
//    This file is part of the featsel program
//    Copyright (C) 2017  Marcelo S. Reis
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

// A class to create mock objects, in order to test protected functions
// of class "MeanMutualInformation"

#ifndef MEAN_CONDITIONAL_ENTROPY_MOCK_H_
#define MEAN_CONDITIONAL_ENTROPY_MOCK_H_

#include "../../src/global.h"
#include "../../src/ElementSet.h"
#include "../../src/ElementSubset.h"

#include "../../src/functions/MeanConditionalEntropy.h"

class MeanConditionalEntropyMock : public MeanConditionalEntropy
{

public:

  // Default constructor.
  //
  MeanConditionalEntropyMock (ElementSet *);

  // Default destructor.
  //
  virtual ~MeanConditionalEntropyMock ();

  // Methods for testing of the W-operator feature selection operations.
  //
  string print_W_operator_samples_frequencies (ElementSubset *);

  string print_label_frequencies (ElementSubset *);

  string print_conditional_entropy (ElementSubset *);

};

#endif /* MeanConditionalEntropyMOCK_H_ */
