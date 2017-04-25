//
// Point.h -- definition of the class "Point".
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

#ifndef POINT_H_
#define POINT_H_

// This cost function employs the same format as the one used by the
// HammingDistance cost function XML files.
//
// It returns cost(X) == 0 if X is precisely the subset defined in the XML
// instance and 1 otherwise (i.e., it defines a whole constant search space. 
// with an outlier "point" somewhere in it.
//

#include "../global.h"
#include "../ElementSet.h"
#include "../ElementSubset.h"
#include "../CostFunction.h"

class Point : public CostFunction
{

public:

  // Default constructor.
  //
  Point (ElementSet *);


  // Default destructor.
  //
  virtual ~Point ();


  // Returns the value of c(X), where X is a subset.
  //
  double cost (ElementSubset *);

};

#endif /* POINT_H_ */
