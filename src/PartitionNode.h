//
// PartitionNode.h -- definition of the class "PartitionNode".
//
//    This file is part of the featsel program
//    Copyright (C) 2016 Marcelo S. Reis, Gustavo E. Matos
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

#ifndef PARTITION_NODE_H_
#define PARTITION_NODE_H_

#include "ElementSet.h"
#include "ElementSubset.h"
#include "Partition.h"

// This class is used to fix elements of an ElementSet and also define
// a subposet of the poset defined by power set of ElementSet
//
class PartitionNode
{

private:

  // All the fixed elements that are selected. This is defined over
  // the ElementSet that contains only the fixed elements
  //
  ElementSubset * selected_elements;

  // Stores the minimal element of the part. This is the same
  // as the ElementSubset selected_elements but defined over the 
  // original ElementSet
  //
  ElementSubset * least_subset;

  // Stores the greatest subset of the part. This is the same
  //
  ElementSubset * greatest_subset;

  // A pointer to the partition this part is from
  //
  Partition * partition;

  // Find the minimal subset of this partition
  // 
  void find_least_subset ();


  // Find the minimal subset of this partition
  // 
  void find_greatest_subset ();

public:

  // Default constructor. Receives the elementsubset that represents
  // the lowest element of this fraction of the poset
  //  
  PartitionNode (Partition *, ElementSubset *);

  // Copy constructor
  //
  PartitionNode (PartitionNode *);

  // Default destructor.
  //
  virtual ~PartitionNode ();

  // From an elementsubset defined in this partition node returns the
  // elementsubset correspondent in the original poset
  //
  ElementSubset * get_original_subset (ElementSubset *);

  // Returns the set of elements that are fixed or not in this part
  //
  ElementSet * get_original_set ();

  // Returns the element subset that represents this part based
  // on the fixed element set. Example: if the part is XX010, then
  // this function returns 010
  //
  ElementSubset * get_selected_elements ();

  // Returns the Partition of this part
  //
  Partition * get_partition ();

  // Returns the number of fixed elements in this part
  //
  unsigned int get_number_of_fixed_elms ();

  // Verifies if this part is upper adjacent to another part
  //
  bool is_upper_adjacent (PartitionNode *);

  // Returns the minimal subset of this part
  //
  ElementSubset * get_least_subset ();

  // Returns the maximal subset of this part
  //
  ElementSubset * get_greatest_subset ();

};

#endif /* PARTITION_NODE_H_ */