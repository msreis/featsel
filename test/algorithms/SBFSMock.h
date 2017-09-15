//
// SBFSMock.h -- definition of the mock class "SBFSMock".
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

// A class to create mock objects, in order to test protected functions of
// class "CostFunction"

#ifndef SBFSMOCK_H_
#define SBFSMOCK_H_

#include "../../src/global.h"
#include "../../src/algorithms/SBFS.h"
#include "../../src/ElementSubset.h"
#include "../../src/ElementSet.h"

class SBFSMock : public SBFS
{

public:

  // Return the index of the element x in S - X that maximizes c(X \cup x),
  // and |S| if no x increases the cost.
  //
  unsigned int get_worst_element_test (ElementSubset *);

  // Return the index of the element x in X that minimizes c(X - x),
  // and |S| if no x decreases the cost.
  //
  unsigned int get_best_element_test (ElementSubset *);

  // Default constructor.
  //
  SBFSMock ();

  // Default destructor.
  //
  virtual ~SBFSMock ();

};

#endif /* SBFSMOCK_H_ */
