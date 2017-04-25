//
// MutualInformation.h -- definition of the class "MutualInformation".
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

#ifndef MUTUALINFORMATION_H_
#define MUTUALINFORMATION_H_


#include "../global.h"
#include "../ElementSet.h"
#include "../ElementSubset.h"
#include "../CostFunction.h"

class MutualInformation : public CostFunction
{

protected: 

  // Number of rows in the DAT file.
  //
  unsigned int t;

  // Total number of samples.
  //
  unsigned int m;

  // Samples for a W-operator feature selection.
  //
  map <string, unsigned int *> samples;

  // Calculates H (Y | X = x)
  //
  double calculate_conditional_entropy (unsigned int *, double);

  // Calculates the Mutual Information of X
  //
  double calculate_MI (ElementSubset *);

  // Compute the distribution of a subset X from samples,
  // storing them into as subsets of a set S, where |S| = |X|.
  // |X| must be greater than zero.
  //
  unsigned int * calculate_distributions_from_the_samples (ElementSubset *);

public:

  // Default constructor.
  //
  MutualInformation (ElementSet *);


  // Default destructor.
  //
  virtual ~MutualInformation ();


  // Returns the value of c(X), where X is a subset.
  //
  double cost (ElementSubset *);

};

#endif /* MUTUALINFORMATION_H_ */

