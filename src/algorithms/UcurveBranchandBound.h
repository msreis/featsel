//
// UcurveBranchandBound.cpp -- definition of the class "UcurveBranchandBound".
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

// Original algorithm published in:
//
// Reis, M.S. (2012).
// "Minimization of Decomposable in U-shaped Curves Functions Defined on Poset
//  Chains -- Algorithms and Applications". (PhD Thesis).
// Institute of Mathematics and Statistics, University of Sao Paulo, Brazil.


#ifndef UCURVE_BRANCH_AND_BOUND_H_
#define UCURVE_BRANCH_AND_BOUND_H_

#include "../global.h"
#include "../Solver.h"
#include "../ElementSubset.h"

#define MAX_SUBSET_SIZE 10000

class UcurveBranchandBound : public Solver
{

public:

  UcurveBranchandBound ();

  virtual ~UcurveBranchandBound ();

  // Run the algorithm, getting up to 'max_size_of_minima_list' minima.
  //
  void get_minima_list (unsigned int);

};

#endif /* UCURVE_BRANCH_AND_BOUND_H_ */
