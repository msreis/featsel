//
// CFS.h -- definition of the class "CFS".
//          (Correlation-based Feature Selection).
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


// Cost function originally published in:
//
// Hall, M.A. (2000).
// "Correlation-based feature selection of discrete and 
// numeric class machine learning."


#ifndef CFS_H_
#define CFS_H_


#include "../global.h"
#include "../ElementSet.h"
#include "../ElementSubset.h"
#include "../CostFunction.h"

class CFS : public CostFunction
{
private:

  // Matrix that stores the pairwise correlations between features, and also
  // correlations between individual features and the class.
  //
  double ** correlation;

  // Array containing the entropy of either a single feature or the label;
  //
  double * H;

  // Number of features.
  //
  unsigned int n;

  // Maximum value among all features.
  //
  unsigned int max_feature_value;

  // Number of rows in the DAT file.
  //
  unsigned int number_of_rows;

  // Auxiliary array to generate contigency tables.
  //
  double * Pr_X;

  // Auxiliary array to generate contigency tables.
  //
  double ** Pr_X1_X2;

  // Auxiliary array to generate contigency tables.
  //
  double ** Pr_X1_Y;

  // Auxiliary array to generate contigency tables.
  //
  double * Pr_Y;

  // Auxiliary function to compute one term of a given entropy.
  //
  double compute_entropy_term (double x);

  // It computes H(X_i), where X_i may be either a feature or a label Y.
  //
  double compute_entropy (unsigned int i);  

  // It computes H(X_i,X_j), where X_j may be either a feature or a label Y.
  //
  double compute_joint_entropy (unsigned int i, unsigned int j);  

  // Computes the symmetrical uncertainity (SU) as a measure of correlation
  // between two features or between a feature and a class. It is given by: 
  //
  // SU(X1,X2) = 2 * ((H(X1) + H(X2) - H(X1,X2)) / (H(X1) + H(X2))),
  //
  // where X1 is a feature and X2 is either another feature or the class Y.
  //
  // This method is similar to the method "correlate" from
  // CfsSubsetEval.java class in Weka source code.
  //
  double symmetrical_uncertainity (unsigned int i, unsigned int j);

  // Computes the "merit" of a given subset X, which is given by:
  //
  // Merit(X) = (k * avg_f_c_corr) / sqrt (k + k * (k - 1) * avg_f_f_corr),
  //
  // where "avg_f_c_corr" and "avg_f_f_corr" are the average correlation between
  // feature-class and feature-feature, respectively. 
  //
  // This method is similar to the method "evaluateSubset" from
  // CfsSubsetEval.java class in Weka source code.
  //
  double evaluateSubset (ElementSubset * X);
  
public:

  // Default constructor.
  //
  CFS (ElementSet *);


  // Default destructor.
  //
  virtual ~CFS ();


  // The CFS cost of a subset X <= S, for a given label Y, is defined as:
  //
  // CFS(X) = 2 * ((H(X) + H(Y) - H(X,Y)) / (H(X) + H(Y))).
  //
  double cost (ElementSubset *);

};

#endif /* CFS_H_ */

