//
// SBS.h -- definition of the class "SBS".
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
// Marill, T., and Green, D. (1963).
// "On the effectiveness of receptors in recognition systems."
// IEEE transactions on Information Theory 9.1: 11-17.

#ifndef SBS_H_
#define SBS_H_

#include "../global.h"
#include "../Solver.h"
#include "../ElementSubset.h"


class SBS : public Solver
{

public:

  // Default constructor.
  //
  SBS ();

  // Default destructor.
  //
  virtual ~SBS ();

  // Run the algorithm, getting up to 'max_size_of_minima_list' minima.
  //
  void get_minima_list (unsigned int);

};

#endif /* SBS_H_ */
