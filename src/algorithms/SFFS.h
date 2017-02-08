//
// SFFS.h -- definition of the class "SFFS"
// ("Sequential Forward Floating Selection").
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


#ifndef SFFS_H_
#define SFFS_H_

#define PARAMETER_DELTA 3

#include "../global.h"
#include "../Solver.h"
#include "../ElementSubset.h"


class SFFS : public Solver
{

protected:

  // Return the index of the element x in S - X that maximizes c(X \cup x),
  // and |S| if no x increases the cost.
  //
  unsigned int get_best_element (ElementSubset *);

  // Return the index of the element x in X that minimizes c(X - x),
  // and |S| if no x decreases the cost.
  //
  unsigned int get_worst_element (ElementSubset *);

public:

  // Default constructor.
  //
  SFFS ();

  // Default destructor.
  //
  virtual ~SFFS ();

  // Run the algorithm, getting up to 'max_size_of_minima_list' minima.
  //
  void get_minima_list (unsigned int);

};

#endif /* SFFS_H_ */
