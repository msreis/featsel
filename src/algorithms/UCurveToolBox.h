//
// UCurveToolBox.h -- definition of the namespace "UCurveToolBox".
//
//    This file is part of the featsel program
//    Copyright (C) 2010  Marcelo S. Reis
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

#ifndef UCURVETOOLBOX_H_
#define UCURVETOOLBOX_H_

#include "../Collection.h"
#include "../CostFunction.h"


namespace UCurveToolBox
{
  //
  // Node data structure
  //
  struct MyNode
  {
    ElementSubset * vertex,      // the subset contained in the node
            * unverified,  // the unverified elements of vertex
            * upper_flag,  // "upper" and "lower" restriction flags
            * lower_flag;
  };

  typedef struct MyNode Node;

  // Returns a maximal subset, say X, with the following properties:
  // - X does not contain a subset from this collection;
  // - there is no subset Y | Y contains X and Y does not contain a
  //   subset from this collection.
  //
  ElementSubset * get_maximal_subset (Collection *, Collection *,
   ElementSet *);

  // Returns a minimal subset, say X, with the following properties:
  // - X is not contained by a subset from this collection;
  // - there is no subset Y | Y is contained by X and Y is not contained
  //   by a subset from this collection.
  //
  ElementSubset * get_minimal_subset (Collection *, Collection *,
   ElementSet *);

  // Receives a subset A, a collection of restrictions and add A into
  // the latter.
  //
  void update_lower_restriction (Collection *, ElementSubset *);

  // Receives a subset A, a collection of restrictions and add A into
  // the latter.
  //
  void update_upper_restriction (Collection *, ElementSubset *);


  // Partial pruning minima exhausting subroutines
  //

  // DFS subroutine
  //
  void DFS (Node *, Collection *, Collection *, Collection *,
   CostFunction *);

  // Prunes the elements of the minima exhausting graph that were lower
  // removed from the search space by the ElementSubset from the Node A.
  //
  void prune_lower_elements (map<string, Node *> *, list<Node *> *,
   Node *);

  // Prunes the elements of the minima exhausting graph that were upper
  // removed from the search space by the ElementSubset from the Node A.
  //
  void prune_upper_elements (map<string, Node *> *, list<Node *> *,
   Node *);

  // Deletes the content of a node; it assumes that all variables are
  // pointers to elements that were actually created.
  //
  void delete_node (Node *);

  // Creates a node, containing the ElementSubset X. It returns a
  // pointer to the new node.
  //
  Node * create_node (ElementSubset *);

  // Receives the collections of restrictions, a graph, and a node M.
  //
  // Returns:
  // - a node N such that N[vertex] is an unvisited adjacent to the
  //   node's element
  // - NULL, if there are no such X (in this case, the value of the
  //   integer doesn't care).
  //
  Node * select_an_unvisited_adjacent (map<string, Node *> *,
   Collection *, Collection *, Node *, unsigned int *);

  // Returns true if a given vertex belongs to a node list, and false
  // otherwise.
  //
  bool list_has_vertex (list<Node *> *, ElementSubset *);

} // end of namespace

#endif /* UCURVETOOLBOX_H_ */
