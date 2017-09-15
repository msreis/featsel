//
// MeanConditionalEntropy.h -- definition of the class "MeanConditionalEntropy"
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

// Original cost function published in:
//
// Martins-Jr, D.C. and Cesar-Jr, R.M. and Barrera, J. (2006).
// "W-operator Window Design by Minimization of Mean Conditional Entropy".
// Pattern Analysis and Applications, 9(2), 139--153.


#ifndef MEAN_CONDITIONAL_ENTROPY_H_
#define MEAN_CONDITIONAL_ENTROPY_H_


#include "../global.h"
#include "../ElementSubset.h"
#include "../CostFunction.h"


class MeanConditionalEntropy : public CostFunction
{

protected:

  // Return the MCE of a given subset X.
  //
  double calculate_MCE (ElementSubset *);

  // Compute the distribution of a subset X from W-operator samples,
  // storing them into as subsets of a set S, where |S| = |X|.
  // |X| must be greater than zero.
  //
  void calculate_distributions_from_the_samples (ElementSubset *, 
          map<string, SampleLabels *> *, unsigned int *);

  // Return the conditional entropy of Y given X = x; the distribution of the
  // samples must be computed before using this function.
  //
  double calculate_conditional_entropy (SampleLabels *, double, unsigned int);

public:

  // Default constructor.
  //
  MeanConditionalEntropy (ElementSet *);

  // Default destructor.
  //
  virtual ~MeanConditionalEntropy ();

  // Returns the value of c(X), where X is a subset.
  //
  double cost (ElementSubset *);

};

#endif /* MEAN_CONDITIONAL_ENTROPY_H_ */
