//
// UCurveSearch.h -- definition of the class "UCurveSearch".
//
//    This file is part of the featsel program
//    Copyright (C) 2016  Marcelo S. Reis
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
// Reis, M.S., Ferreira, C.E., and Barrera, B. (2014).
// "The U-curve optimization problem: improvements on the original
// algorithm and time complexity analysis."
// arXiv preprint arXiv:1407.6067.


#ifndef UCURVESEARCH_H_
#define UCURVESEARCH_H_

#include "../global.h"
#include "../Solver.h"
#include "../ElementSubset.h"
#include "UCurveToolBox.h"


class UCurveSearch : public Solver
{

private:

  // The collection of restrictions
  //
  Collection * lower_restriction, * upper_restriction;

public:

  // Default constructor.
  //
  UCurveSearch ();

  // Default destructor.
  //
  virtual ~UCurveSearch ();

  // Run the algorithm, getting up to 'max_size_of_minima_list' minima.
  //
  void get_minima_list (unsigned int);

};

#endif /* UCURVESEARCH_H_ */
