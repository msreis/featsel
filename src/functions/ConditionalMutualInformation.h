//
// ConditionalMutualInformation.h -- definition of the class
//                                   "ConditionalMutualInformation".
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

#ifndef CONDITIONALMUTUALINFORMATION_H_
#define CONDITIONALMUTUALINFORMATION_H_


#include "../global.h"
#include "../ElementSet.h"
#include "../ElementSubset.h"
#include "../CostFunction.h"


// Definition of this cost function c:P(S)->R+:
//
// c(X) = I(x; Y), if X = { x };
//
// c(X) = 1/2 * (I(x1; Y | x2) + I(x2; Y | x1)), if X = { x1, x2 };
//
// c(X) = 0, otherwise.

//
// Hence, this cost function is useful to compute the Hessian matrix Q for the
// SPEC-CMI algorithm as described in Xuan Vinh et al. (2014).
//


class ConditionalMutualInformation : public CostFunction
{

private:

  // Compute I(x; Y) = \sum_{x,Y} P(x, Y) log P(x, Y) / (P(x) P(Y)).
  //
  double mutual_information (unsigned int x1);

  // Compute I(x1; Y | x2) = 
  //    \sum_{x1,Y,x2} P(x1, Y, x2) log (P(x2) P(x1, Y, x2))/(P(x1,x2) P(Y,x2)).
  //
  double conditional_mutual_information (unsigned int x1, unsigned int x2);


public:

  // Default constructor.
  //
  ConditionalMutualInformation (ElementSet *);


  // Default destructor.
  //
  virtual ~ConditionalMutualInformation ();


  // Returns the value of c(X), where X is a subset.
  //
  double cost (ElementSubset *);

};

#endif /* CONDITIONALMUTUALINFORMATION_H_ */
