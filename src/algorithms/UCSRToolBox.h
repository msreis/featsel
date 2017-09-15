//
// UCSRToolBox.cpp -- definition of the namespace "UCSRToolBox".
//
//    This file is part of the featsel program
//    Copyright (C) 2016 Gustavo E. Matos
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


#ifndef UCSR_TOOLBOX_H_
#define UCSR_TOOLBOX_H_

#include "../ROBDD.h"
#include "../CostFunction.h"
#include "../Collection.h"


namespace UCSRToolBox
{
    //
    // Node data structure
    //
    struct MyNode
    {
        ElementSubset * vertex,      // the subset contained in the node
                      * upper_unverified, // upper adjacent unverified subsets
                      * lower_unverified; // lower adjacent unverified subsets
    };

    typedef struct MyNode Node;

    // Receives a subset A, a collection of restrictions and add A into the latter.
    //
    void update_lower_restriction (ROBDD *, ElementSubset *);


    // Receives a subset A, a collection of restrictions and add A into the latter.
    //
    void update_upper_restriction (ROBDD *, ElementSubset *);


    // Partial pruning minima exhausting subroutines
    //

    // DFS subroutine
    //
    void DFS (Node *, Collection *, ROBDD *, CostFunction *);


    // Visits a node X from node Y
    // If cost(X) > cost (Y) nothing happens.
    // Otherwise, X is added to the collection L and if direction == 1 (Y covers X) we
    // add the interval [Y, S] to restrictions; if direction == 0 we add the interval
    // [0, Y] to restrictions.
    //
    void visit_adjacent (ROBDD *, Collection *, Node **, Node *, unsigned int direction,
                       CostFunction *, list<ElementSubset *> *, list<ElementSubset *> *);

    // Receives a list of subsets l, an ROBDD and a direction.
    // If direction = 1 (0), then l contains lower (upper) visited greater subsets.
    // So, for each l[i], we add the interval [0, l[i]] ([l[i], S]) to restrictions.
    void restrict_visited_adjacents (ROBDD *, list<ElementSubset *> *, unsigned int);


    // Deletes the content of a node; it assumes that all variables are pointers to
    // elements that were actually created.
    //
    void delete_node (Node *);


    // Creates a node, containing the ElementSubset X.
    // It returns a pointer to the new node.
    //
    Node * create_node (ElementSubset * X);


    // Receives the collections of restrictions, a graph, and a node M.
    //
    // Returns:
    // - a node N such that N[vertex] = X is an unvisited adjacent to the node's element
    // - if direction = 0 returns a upper adjacent; if direction = 1 returns a lower
    // adjacent
    // - NULL, if there are no such X (in this case, the value of the integer doesn't care).
    //
    Node * select_ul_unvisited_adjacent (ROBDD *, Node *, unsigned int);


    // Returns true if a given vertex belongs to a node list, and false otherwise.
    //
    bool list_has_vertex (list<Node *> *, ElementSubset *);


    // Deletes all the element subsets of a list and cleans the list
    //
    void clean_adjacent_list (list<ElementSubset *> *);


} // end of namespace

#endif /* UCSR_TOOLBOX_H_ */
