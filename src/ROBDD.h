//
// ROBDD.h -- definition of the namespace "ROBDD".
//
//    This file is part of the featsel program
//    Copyright (C) 2015  Marcelo S. Reis, Gustavo Estrela
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

#ifndef ROBDD_H_
#define ROBDD_H_

#include "global.h"
#include "ElementSet.h"
#include "Vertex.h"
#include "ElementSubset.h"
#include "CostFunction.h"

class ROBDD
{

protected:

  // The set of elements this ROBDD will represent
  //
  ElementSet * elm_set;
  // ROBDD cardinality
  //
  unsigned int cardinality;

  // The root of the ROBDD
  //
  Vertex * root;  

  // Prints the sub-tree that has the parameter Vertex as root
  //
  void print(Vertex *);

  // Auxiliar function to ROBDD (ElementSet *, ElementSubset*)
  //
  void build (Vertex *, unsigned int, unsigned int, ElementSubset *, Vertex *, Vertex *);

  // Auxiliar function to ROBDD::reduce ()
  //
  void fill_vlist (Vertex *, list<Vertex *> **);

  // Unmarks all the vertices from the robdd
  //
  void unmark_all_vertex ();

  // Unmarks all the vertices from the subtree with root Vertex *
  //
  void unmark_all_vertex (Vertex *);

  // Deletes the subtree with root Vertex *
  //
  void delete_subtree (Vertex **, unsigned int *);

  // Returns a list with all vertex of the ROBDD with root Vertex *
  //
  Vertex ** get_all_vertex (Vertex *, unsigned int);

  // Auxiliar function to get_all_vertex that traverse the robdd assgning
  // pointers of the vertex to the vector vertice
  //
  void fill_vertice (Vertex **, int *, Vertex *);

  // Auxiliar function to add_lower_interval (). This method builds a tree of
  // a ROBDD that has ones for subsets covered by the subset passed by 
  // argument.
  //
  Vertex * build_interval (unsigned int, unsigned int *, ElementSubset *, Vertex *, Vertex *, bool);

  // Defines the structure used to save vertice on the reducing procedure
  //
  typedef struct MyVerticeEntry
  {
    int lo_id;
    int hi_id;
    Vertex * v;
    bool operator < (const MyVerticeEntry& y);
  } VerticeEntry;


public:

  // Builds a ROBDD representing the function "0"
  //
  ROBDD (ElementSet *);

  // Creates a ROBDD represented by the tree with root root.
  //
  ROBDD (ElementSet * set, Vertex * root, int card);

  // Builds a ROBDD representing the function "0" using the ordering in 
  // unsigned int *
  //
  ROBDD (ElementSet *, unsigned int *);

  // Builds an ROBDD where the only path valued "1", is the path representing
  // the subset
  //
  ROBDD (ElementSet *, ElementSubset *);

  // Default destructor.
  //
  virtual ~ROBDD ();  

  // Returns the root vertex
  //
  Vertex * get_root ();

  // Prints the entire ROBDD
  //
  void print ();

  // Reduces the OBDD
  //
  void reduce ();

  // Makes the union of this ROBDD and the one passed by argument
  //
  void union_to (Vertex *);

  // Makes the union of the subtrees passed by argument
  //
  Vertex * union_step (Vertex * v1, Vertex * v2, 
    map<pair<Vertex *, Vertex *>, Vertex *> *, unsigned int *, Vertex *, Vertex *);

  // Adds to the ROBDD all the subsets covered (if bool false) or that covers (if bool
  // true) by ElementSubset *
  //
  void add_interval (ElementSubset *, bool);

  // Adds to the ROBDD a subset
  //
  void add_subset (ElementSubset *);

  // Returns true if the ElementSubset * path of the robdds leads to a 1
  //
  bool contains (ElementSubset *);

  // Returns a random subset that is evaluated by zero.
  //
  ElementSubset * get_random_zero_evaluated_element ();

  // Returns the ElementSet used to build the ROBDD
  //
  ElementSet * get_element_set ();

  // Returns the number of vertice of the ROBDD
  //
  unsigned int get_cardinality ();

};

#endif /* ROBDD_H_ */
