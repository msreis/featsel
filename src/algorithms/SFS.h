//
// SFS.h -- definition of the class "SFS" ("Sequential Forward Selection").
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
//  Whitney, A.W. (1971).
//  "A Direct Method of Nonparametric Measurement Selection".
//  IEEE Transactions on Computers, 20(9), 1100--1103.


#ifndef SFS_H_
#define SFS_H_

#include "../global.h"
#include "../Solver.h"
#include "../ElementSubset.h"


class SFS : public Solver
{

public:

  // Default constructor.
  //
  SFS ();

  // Default destructor.
  //
  virtual ~SFS ();

  // Run the algorithm, getting up to 'max_size_of_minima_list' minima.
  //
  void get_minima_list (unsigned int);

};

#endif /* SFS_H_ */
