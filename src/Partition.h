//
// Partition.h -- definition of the class "Partition".
//
//    This file is part of the featsel program
//    Copyright (C) 2016  Marcelo S. Reis, Gustavo E. Matos
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

#ifndef PARTITION_H_
#define PARTITION_H_

#include "ElementSet.h"
#include "ElementSubset.h"
#include <map>


class Partition
{

private:

  // Pointer to the set that contains the elements that are fixed or 
  // not, i.e the original set
  //
  ElementSet * original_set;

  // Pointer to the set that cointais the elements that are fixed
  //
  ElementSet * fixed_set;

  // Pointer to the set that cointais the elements that are not fixed
  //
  ElementSet * unfixed_set;

  // Used to map the elements of the fixed_set to the original set
  //
  unsigned int * fixed_elm_map;

  // Used to map the elements of the unfixed_set to the original set
  //
  unsigned int * unfixed_elm_map;

  // Stores the size of the element set without the fixed elements
  //
  unsigned int fixed_set_size;

public:

  // Default constructor. Receives the original element set and an
  // array that says if i-th element of the element set is fixed
  //
  Partition (ElementSet *, bool *);

  // Default destructor.
  //
  virtual ~Partition ();

  // Returns a pointer to the element set that contains the elements
  // of the original set and are not fixed
  //
  ElementSet * get_unfixed_elm_set ();

  // Returns a pointer to the element set that contains the elements
  // of the original set and are fixed
  //
  ElementSet * get_fixed_elm_set ();

  // Returns a pointer to the original element set
  //
  ElementSet * get_original_set ();

  // Returns the map for the fixed elements
  //
  unsigned int * get_fixed_elm_map ();

  // Returns the map for the unfixed elements
  //
  unsigned int * get_unfixed_elm_map ();
};

#endif /* PARTITION_H_ */
