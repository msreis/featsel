//
// TailorConvexHull.h -- definition of the class "TailorConvexHull".
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
// Reis, M.S., and Barrera, J. (2013).
// "Solving Problems in Mathematical Morphology through
// Reductions to the U-Curve Problem."
// International Symposium on Mathematical Morphology and Its Applications 
// to Signal and Image Processing. Springer Berlin Heidelberg, 2013.


#ifndef TAILORCONVEXHULL_H_
#define TAILORCONVEXHULL_H_


#include "../global.h"
#include "../ElementSet.h"
#include "../ElementSubset.h"
#include "../CostFunction.h"

class TailorConvexHull : public CostFunction
{

protected:

  // The size of the cloth.
  //
  int cloth_size;

  // Performs a local branch-and-bound with the pieces of the subset X.
  //
  double branch_and_bound (ElementSubset * X);

  // Returns the greatest maximal convex hull of array [] cloth
  // (in R^1 it is the greatest sequence of zeroes).
  //
  unsigned int search_greatest_maximal_convex_hull (unsigned int * cloth);

  // Verifies if it is possible to insert a piece with "piece_index"
  // in Element object into the cloth [] array, starting from cloth_index
  // position. Returns false if it is not possible and true otherwise.
  //
  bool piece_fits_in_cloth (unsigned int piece_index, unsigned int cloth_index,
                            unsigned int * cloth);

  // Inserts a piece into the cloth.
  //
  void insert_piece (unsigned int piece_index, unsigned int cloth_index,
                     unsigned int * cloth);

  // Removes a piece from the cloth
  //
  void remove_piece (unsigned int piece_index, unsigned int cloth_index,
                     unsigned int * cloth);

public:

  // Default constructor.
  //
  TailorConvexHull (ElementSet *);


  // Default destructor.
  //
  virtual ~TailorConvexHull ();


  // Returns the value of c(X), where X is a subset.
  //
  double cost (ElementSubset *);

};

#endif /* TAILORCONVEXHULL_H_ */
