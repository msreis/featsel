// NystromSpecCMI.h automatically generated by bin/add_new_algorithm.pl
// in Wed Feb  7 12:10:39 2018.

//
// NystromSpecCMI.h -- definition of the class "NystromSpecCMI".
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

#ifndef NYSTROMSPECCMI_H_
#define NYSTROMSPECCMI_H_

#include "../global.h"
#include "../Solver.h"
#include "../ElementSubset.h"
#include "../functions/ConditionalMutualInformation.h"

// Default Nystrom sampling rate
//
#define NYSTROM_SAMPLING_RATE 0.8

// If true, print for each feature index its respective ranking.
//
# define PRINT_FEATURE_RANKING true

// If true, it stores the chain of best subset between
// 
// \emptyset <= X_1 <= \ldots <= X_k, |X_1| = 1, \ldots |X_k| = k.
//
# define PRINT_SUBSET_CHAIN false

class NystromSpecCMI : public Solver
{

private:

  // Nystrom sampling rate controls the number of rows of Q (in the 
  // SpecCMI formulation)  that will be calculated to estimate the 
  // dominant eigenvector. Represented by gamma in Xuan Vinh et al. 
  // (2014)
  //
  double nystrom_sampling_rate;

  // Submatrices of Q', used to estimate Q. Let p be gamma * n. Then
  // [A_{p x p}  B_{p x (n - p)}] represent the first p rows of Q.
  //
  double ** A;
  double ** B;

  // Number of best-ranked features to be selected in order to compose the
  // output feature subset.
  //
  unsigned int k;

  // This the number of lines of Q that are sampled. p = set_cardinality * gamma
  //
  unsigned int p;

  // Creates A matrix
  //
  void create_A ();

  // Creates B matrix
  //
  void create_B ();

  // Stores the cost function used to calculate entries of Q matrix
  //
  ConditionalMutualInformation * cmi;

  // Calculates a Q entry. Used in sampling Q.
  //
  double compute_Q_entry (unsigned int, unsigned int);


public:

  // Default constructor.
  //
  NystromSpecCMI ();


  // Constructor with sampling rate.
  //
  NystromSpecCMI (double);

  // Constructor with number of selected features
  //
  NystromSpecCMI (unsigned int);

  // Constructor with number of selected features and sampling rate
  //
  NystromSpecCMI (unsigned int, double);

  // Default destructor.
  //
  virtual ~NystromSpecCMI ();

  // Run the algorithm, getting up to 'max_size_of_minima_list' minima.
  //
  void get_minima_list (unsigned int);

  // Samples the Q matrix and creates A and B matrices
  //
  void sample_Q ();

  // Returns the value sampled from Q in A or B
  //
  double get_Q_sample (unsigned int, unsigned int);

  // Returns an array with the feature ranking
  //
  double * rank_features ();
};

#endif /* NYSTROMSPECCMI_H_ */