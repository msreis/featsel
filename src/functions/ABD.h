//
// ABD.h -- definition of the class "ABD"
//         (Atashpaz-Gargari, Braga-Neto, Dougherty).
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

// Cost function originally used in:
//
// Atashpaz-Gargari, Esmaeil, Ulisses M. Braga-Neto, and Edward R. Dougherty.
// "Improved branch-and-bound algorithm for U-curve optimization."
// Genomic Signal Processing and Statistics (GENSIPS), 2013.


#ifndef ABD_H_
#define ABD_H_


#include "../global.h"
#include "../ElementSet.h"
#include "../ElementSubset.h"
#include "../CostFunction.h"


#define C_MAX_VALUE 1


class ABD : public CostFunction
{
private:

  // Cost scale, or ideal maximum value of cost.
  //
  double c_max;

  // Global minimum of the cost function.
  //
  ElementSubset * F0;

  // Size of the feature set.
  //
  unsigned int n;

  // Positive-definite weighting matrix (shapping matrix).
  //
  double ** W;

  // This method receives the vector (F - F0), computes
  // (F - F0)^T * W * (F - F0) and returns the resulting scalar.
  //
  double compute_products (double * F_minus_F0);

public:

  // Default constructor.
  //
  ABD (ElementSet *);


  // Default destructor.
  //
  virtual ~ABD ();


  // Returns the value of c(F), where F is a subset.
  //
  double cost (ElementSubset * F);

};

#endif /* ABD_H_ */
