//
// Collection.h : definition of the class "Collection".
//
//    This file is part of the featsel program
//    Copyright (C) 2015  Marcelo S. Reis
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

#ifndef COLLECTION_H_
#define COLLECTION_H_

#include "global.h"
#include "ElementSubset.h"
#include "CostFunction.h"


class Collection
{

private:

  // Sub-routine of the methods "remove_lower_covered_subsets" and
  // "remove_upper_covered_subsets".
  //
  unsigned int remove_covered_subsets (ElementSubset *, bool);

public:

  // container map; it is implemented as a self-balancing binary search tree
  // (namely a Red-black tree).
  //
  map<string, ElementSubset *> subset_collection;

  // Default constructor.
  //
  Collection ();

  // Default destructor.
  //
  virtual ~Collection ();

  // Adds a copy of the subset X to the collection.
  // Returns the address of the added element if the operation was successful
  // and NULL if it was not.
  //
  ElementSubset * add_subset (ElementSubset *);

  // Copy a given collection into this collection.
  //
  void copy (Collection *);

  // Returns the pointer to X if the ElementSubset is in the Collection and
  // NULL otherwise.
  //
  ElementSubset * get_subset (ElementSubset *);

  // Returns true if the collection has a copy of the subset X and false
  // if not.
  //
  bool has_subset (ElementSubset *);

  // Returns true if there is a subset in the collection that
  // contains the subset X, and false otherwise
  //
  bool lower_covers(ElementSubset *);

  // Returns a string with all the subsets of the collection.
  // (e.g. returns " [010] [101] [111] ").
	string print_collection();

  // Returns a string with all the subsets of the list of minima with their
  // respective cost (one subset-cost per line).
  // (e.g. returns " [010] : 1.12
  //                 [101] : 0.73
  //                 [111] : 0.12 ").
  //
  string print_collection(CostFunction *);

  // Removes the last subset from the collection and returns a pointer to it.
  // If the collection is empty, it returns NULL.
  //
  ElementSubset * remove_last_subset();

  // Receives a subset X and removes (deleting object) all the lower-covered
  // subsets in the collection. It returns the number of removed subsets.
  //
  unsigned int remove_lower_covered_subsets(ElementSubset *);

  // Receives a subset X and removes (deleting object) all the upper-covered
  // subsets in the collection. It returns the number of removed subsets.
  //
  unsigned int remove_upper_covered_subsets(ElementSubset *);

  // Returns the number of elements of the collection.
  //
  unsigned int size ();

  // Returns true if there is a subset in the collection that
  // is contained by the subset X, and false otherwise.
  //
  bool upper_covers(ElementSubset *);

};

#endif /* COLLECTION_H_ */
