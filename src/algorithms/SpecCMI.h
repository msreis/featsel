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


# ifndef SPECCMI_H_
# define SPECCMI_H_

# include "../global.h"
# include "../Solver.h"
# include "../ElementSubset.h"

# include "../functions/ConditionalMutualInformation.h"

class SpecCMI : public Solver
{

private:

  // Symmetric matrix containing the pairwise conditional mutual information,
  // as described in Xuan Vinh et al. (2014).
  //
  double ** Q;

  // Execute the Rayleigh quotient iteration algorithm on matrix Q,
  // with precision epsilon, mu and x as initial guesses for the dominant
  // eigenvalue and eigenvector, respectively.
  //
  double * Rayleigh (double epsilon, double mu, double * x);

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

  // Compute and return a unit-norm eigenvector whose corresponding eigenvalue 
  // is the dominant eigenvalue of Q, that is, the largest eigenvalue of Q.
  //
  double * rank_features ();

  // Compute the symmetric matrix Q of the quadratic term.
  //
  void compute_Q_matrix ();
  
  // Return the value Q[i][j].
  //
  double get_Q_matrix_value (unsigned int i, unsigned int j);

};

#endif /* SPECCMI_H_ */

