//
// SFFSMock.h -- definition of the mock class "SFFSMock".
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

// A class to create mock objects, in order to test protected functions of
// class "CostFunction"

#ifndef SFFSMOCK_H_
#define SFFSMOCK_H_

#include "../../src/global.h"
#include "../../src/algorithms/SFFS.h"
#include "../../src/ElementSubset.h"
#include "../../src/ElementSet.h"

class SFFSMock : public SFFS
{

public:

  // Return the index of the element x in S - X that maximizes c(X \cup x),
  // and |S| if no x increases the cost.
  //
  unsigned int get_best_element_test (ElementSubset *);

  // Return the index of the element x in X that minimizes c(X - x),
  // and |S| if no x decreases the cost.
  //
  unsigned int get_worst_element_test (ElementSubset *);

  // Default constructor.
  //
  SFFSMock ();

  // Default destructor.
  //
  virtual ~SFFSMock ();

};

#endif /* SFFSMOCK_H_ */
