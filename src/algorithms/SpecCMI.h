//
// SpecCMI.h -- definition of the class "SpecCMI".
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
// Nguyen, X.V. and Chan, J. and Romano, S. and Bailey, J. (2014).
// "Effective Global Approaches for Mutual Information Based Feature Selection".
// Proceedings of the 20th ACM SIGKDD International Conference
// on Knowledge Discovery and Data Mining (KDD 2014).


#ifndef SPECCMI_H_
#define SPECCMI_H_

// This class uses the quadratic programming solver from GPL version of ALGLIB
// "ALGLIB (www.alglib.net), Sergey Bochkanov":
//
// http://www.alglib.net/optimization/quadraticprogramming.php
// http://www.alglib.net/translator/man/manual.cpp.html#unit_minqp
//

# include "../alglib/optimization.h"
# include "../alglib/stdafx.h"

# include "../global.h"
# include "../Solver.h"
# include "../ElementSubset.h"

using namespace alglib;

class SpecCMI : public Solver
{

public:

  // Default constructor.
  //
  SpecCMI ();

  // Default destructor.
  //
  virtual ~SpecCMI ();

  // Run the algorithm, getting up to 'max_size_of_minima_list' minima.
  //
  void get_minima_list (unsigned int);

};

#endif /* SPECCMI_H_ */
