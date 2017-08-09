//
// SubsetSum.h -- definition of the class "SubsetSum".
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

// Original cost function published in:
//
// Reis, M.S. (2012).
// "Minimization of Decomposable in U-shaped Curves Functions Defined on Poset
//  Chains -- Algorithms and Applications". (PhD Thesis).
// Institute of Mathematics and Statistics, University of Sao Paulo, Brazil.


#ifndef SUBSET_SUM_H_
#define SUBSET_SUM_H_


#include "../global.h"
#include "../ElementSet.h"
#include "../ElementSubset.h"
#include "../CostFunction.h"


class SubsetSum : public CostFunction
{

protected:

  // t is the average of all values
  //
  int t;

public:

  // Default constructor.
  //
  SubsetSum (ElementSet *);

  // Default destructor.
  //
  virtual ~SubsetSum ();

  // Returns the value of c(X), where X is a subset.
  //
  double cost (ElementSubset *);

};

#endif /* SUBSET_SUM_H_ */

