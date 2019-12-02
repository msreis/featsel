// PUCS.h automatically generated by bin/add_new_algorithm.pl
// in Thu Nov  9 10:12:16 2017.

//
// PUCS.h -- definition of the class "PUCS".
//
//    This file is part of the featsel program
//    Copyright (C) 2017  Gustavo Estrela, Marcelo S. Reis
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

#ifndef PUCS_H_
#define PUCS_H_

#include "../global.h"
#include "../Solver.h"
#include "../ROBDD.h"
#include "../CostFunction.h"
#include "../Collection.h"
#include "../ElementSubset.h"
#include "../Partition.h"
#include "../functions/PartCost.h"
#include "../PartitionNode.h"
#include "SFS.h"
#include "ExhaustiveSearch.h"
#include <omp.h>

#define ES_CUTOFF 5

class PUCS : public Solver
{

private:

  // The partitions of the poset which can have the minimal element
  //
  ROBDD * cand_part;

  // The partition used
  //
  Partition * partition;

  // The maximum depth that the algorithm can recurse until solve the
  // instance using SFS
  //
  unsigned int l;

  // The percentage of the element set that will be fixed in the 
  // partition model.
  //
  float p;

  // Sets the partition model used
  //
  void set_partition_model ();

  // A simple random walk between parts. This function receives as
  // argument a part P. From P the function walks in the following way:
  //
  // do {
  //     Choose an adjacent part Q
  //     Apply prunning rules
  //     If P and Q got prunned, return
  //     If Q got prunned, choose another neighbour of P
  //     Otherwise, P = Q
  // } while (P has unvisited neighbours);
  //
  void random_walk (PartitionNode *, list<ElementSubset *> *);

  // Receives a list of parts to be solved. Solves all parts and 
  // pushes the elements of lowest cost to the minima list
  //
  void solve_part (PartitionNode *, list<ElementSubset *> *);

  // Adds the elements from a list of elementsubset to a collection. 
  // This function is used to transfer the list of minima of a 
  // partition to the list of minima of the original problem
  //
  void part_minima_collection (Collection *, PartitionNode *, 
    list<ElementSubset *> *);

  // Finds the minimum of a partition and adds the list of minima of
  // this partition to the collection L.
  //
  Collection * part_minimum (PartitionNode *, unsigned int);

  // Given a PartitionNode P and an index i we return an adjacent 
  // part Q. Parts are defined according to fixed elements that can
  // be selected or not inside the part. If the set of selected
  // elements in one part differs to the set of selected elements in
  // another part by only one element then we call these parts
  // adjacent. 
  //    Given part P we return adjacent part Q such that Q is 
  // adjacent to P and the element that differs from both is the i-th
  // element.
  //
  PartitionNode * adjacent_part (PartitionNode *, unsigned int);

  // Given a part P and part Q, we prune the poset with the folowing 
  // rule.
  //     If P1 is upper adjacent to P2 then:
  //     1) If c (O (P1)) > c (O (P2)) then we can prune every part
  //        that covers P1 (including itself).
  //     2) If c (I (P2)) > c (I (P1)) then we can prune every part
  //        that is covered by P2 (including itself).
  //
  // This function returns:
  //     Q by default
  //     P if Q is restricted by the end of the function
  //     NULL if P and Q are restricted by the end of the function
  PartitionNode * prune_and_walk (PartitionNode *, PartitionNode *);

  // Returns true if the part is restricted
  //
  bool is_restricted (PartitionNode *);

  // Restricts a part
  //
  void restrict_part (PartitionNode *);

  // Updates the collection of visited subsets
  //
  void update_visited_subsets (Collection *, PartitionNode *);


  // The maximum size of the list of minima
  //
  unsigned int max_size_of_minima_list;

public:

  // Default constructor.
  //
  PUCS ();

  // Constructor that defines p and l
  //
  PUCS (float, unsigned int);

  // Default destructor.
  //
  virtual ~PUCS ();

  // Run the algorithm, getting up to 'max_size_of_minima_list' minima.
  //
  void get_minima_list (unsigned int);

};

#endif /* PUCS_H_ */
