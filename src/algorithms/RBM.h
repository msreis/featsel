//
// RBM.h -- definition of the class "RBM".
//
//    This file is part of the featsel program
//    Copyright (C) 2018  Marcelo S. Reis
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

#ifndef RBM_H_
#define RBM_H_

#include "../global.h"
#include "../Solver.h"
#include "../ElementSubset.h"
#include "UCurveToolBox.h"

// The Ris-Barrera-Martins-Jr. (RBM) original algorithm to tackle
// the U-curve optimization problem.

// Original algorithm published in:
//
// Ris, M., Barrera, J., and Martins-Jr, D. (2010).
// U-curve: A branch-and-bound optimization algorithm for U-shaped cost
// functions on Boolean lattices applied to the feature selection problem.
// Pattern Recognition, 43(3):557--568.


class RBM : public Solver
{

private:

	// The collection of lower restrictions.
	//
	Collection * lower_restriction;


	// The collection of upper restrictions.
	//
	Collection * upper_restriction;

  
  // Original, flawed minima exhausted subroutine of the RBM algorithm.
  // Ris et al., Algorithm 3.
  //
  void minima_exhausting (Collection * list_of_minima, Collection * RL,
    	                   Collection * RU, ElementSubset * M, CostFunction * c);

  // Select at random an adjacent element.
  //
  ElementSubset * select_adjacent (list <ElementSubset *> * L, Collection * RL,
                                   Collection * RU, ElementSubset * M);

  // Returns a random element that is lower adjacent to A (i.e., one that is
  // within the current search space and is not in the stack L). If such 
  // element does not exist, then it returns NULL.
  //
  ElementSubset * select_lower_adjacent (list <ElementSubset *> * L, 
                          Collection * RL, Collection * RU, ElementSubset * A);


  // Returns a random element that is upper adjacent to A (i.e., one that is
  // within the current search space and is not in the stack L). If such 
  // element does not exist, then it returns NULL.
  //
  ElementSubset * select_upper_adjacent (list <ElementSubset *> * L,
                          Collection * RL, Collection * RU, ElementSubset * A);


  // Get the element of minimum cost of a random chain and perform a 
  // minimum exhausting procedure on it. (Top-down iteration).
  // Ris et al., dual version of Algorithm 2.
  //
  bool up_down_direction (Collection   * L,
                          Collection   * lower_restriction,
                          Collection   * upper_restriction,
                          CostFunction * cost_function,
                          ElementSet   * set);


  // Get the element of minimum cost of a random chain and perform a 
  // minimum exhausting procedure on it. (Bottom-up iteration).
  // Ris et al., Algorithm 2.
  //
  bool down_up_direction (Collection   * L,
                          Collection   * lower_restriction,
                          Collection   * upper_restriction,
                          CostFunction * cost_function,
                          ElementSet   * set);

public:

  // Default constructor.
  //
  RBM ();

  // Default destructor.
  //
  virtual ~RBM ();

  // Run the algorithm, getting up to 'max_size_of_minima_list' minima.
  //
  void get_minima_list (unsigned int);

};

#endif /* RBM_H_ */
