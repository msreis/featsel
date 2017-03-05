//
// BFS.h -- definition of the class "BFS" (Best-First Search).
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
// Kohavi, Ron, and George H. John.
// "Wrappers for feature subset selection."
// Artificial intelligence 97.1-2 (1997): 273-324.

// Algorithm is described in Table 6 of that work.


#ifndef BFS_H_
#define BFS_H_

#include "../global.h"
#include "../Solver.h"
#include "../ElementSubset.h"


class BFS : public Solver
{

public:

  // Default constructor.
  //
  BFS ();

  // Default destructor.
  //
  virtual ~BFS ();

  // Run the algorithm, getting up to 'max_size_of_minima_list' minima.
  //
  void get_minima_list (unsigned int);

};

#endif /* BFS_H_ */
