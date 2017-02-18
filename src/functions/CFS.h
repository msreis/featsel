//
// CFS.h -- definition of the class "CFS".
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


// Cost function originally published in:
//
// Hall, M.A. (2000).
// "Correlation-based feature selection of discrete and 
// numeric class machine learning."


#ifndef CFS_H_
#define CFS_H_


#include "../global.h"
#include "../ElementSet.h"
#include "../ElementSubset.h"
#include "../CostFunction.h"

class CFS : public CostFunction
{

public:

  // Default constructor.
  //
  CFS (ElementSet *);


  // Default destructor.
  //
  virtual ~CFS ();


  // The CFS cost of a subset X <= S, for a given label Y, is defined as:
  //
  // CFS(X) = 2 * ((H(X) + H(Y) - H(X,Y)) / (H(X) + H(Y))).
  //
  float cost (ElementSubset *);

};

#endif /* CFS_H_ */

