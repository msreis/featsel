//
// SBFS.h -- definition of the class "SBFS".
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
// Pudil, P. and Novovicova, J. and Kittler, J. (1994).
// "Floating Search Methods in Feature Selection".
// Pattern Recognition Letters, 15(11), 1119--1125.

#ifndef SBFS_H_
#define SBFS_H_

#include "../global.h"
#include "../Solver.h"
#include "../ElementSubset.h"


#define PARAMETER_DELTA_SBFS 3


class SBFS : public Solver
{

protected:

  // Returns the index of the element x in S - X that maximizes c(X \cup x).
  // Returns |S| if no x increases the cost.
  //
  unsigned int get_best_element (ElementSubset *);

  // Returns the index of the element x in X that minimizes c(X - x).
  // Returns |S| if no x decreases the cost.
  //
  unsigned int get_worst_element (ElementSubset *);

public:

  // Default constructor.
  //
  SBFS ();

  // Default destructor.
  //
  virtual ~SBFS ();

  // Runs the UCurve algorithm, getting up to 'max_size_of_minima_list' minimum
  // subsets.
  //
  void get_minima_list (unsigned int);

};

#endif /* SBFS_H_ */

