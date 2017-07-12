//
// TailorConvexHull.cpp -- implementation of the class "TailorConvexHull".
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

#include "TailorConvexHull.h"


TailorConvexHull::TailorConvexHull (ElementSet * a_set)
{
  set = a_set;
  cloth_size = set->get_set_value ();
}


TailorConvexHull::~TailorConvexHull ()
{
  // Empty destructor.
}


unsigned int TailorConvexHull::search_greatest_maximal_convex_hull
  (unsigned int * cloth)
{
  int i, greatest_convex_hull = 0, current_convex_hull = 0;
  for (i = 0; i < cloth_size; i++)
  {
    if (cloth [i] == 0)
    {
      current_convex_hull++;
      if (current_convex_hull > greatest_convex_hull)
        greatest_convex_hull = current_convex_hull;
    }
    else
      current_convex_hull = 0;
  }
  return greatest_convex_hull;
}


bool TailorConvexHull::piece_fits_in_cloth
  (unsigned int piece_index, unsigned int cloth_index, unsigned int * cloth)
{
  Element * piece = set->get_element (piece_index);
  int               i = cloth_index,
      remaining_piece = atoi (piece->get_element_name ().c_str ());

  while ( (i < cloth_size) && (remaining_piece > 0) )
  {
    if ((cloth [i] == 1) && (piece->get_element_value (i - cloth_index) == 1))
      return false;
    remaining_piece--;
    i++;
  }
  if (remaining_piece == 0)
    return true;
  else
    return false;
}


void TailorConvexHull::insert_piece
  (unsigned int piece_index, unsigned int cloth_index, unsigned int * cloth)
{
  Element * piece = set->get_element (piece_index);
  int               i = cloth_index,
      remaining_piece = atoi (piece->get_element_name ().c_str ());
  while ((i < cloth_size) && (remaining_piece > 0))
  {
    if ((cloth [i] == 1) && (piece->get_element_value (i - cloth_index) == 1))
      cout << "Error: trying to insert a piece that does not fit!\n";
    else if (piece->get_element_value (i - cloth_index) == 1)
      cloth [i] = 1;
    remaining_piece--;
    i++;
  }
}


void TailorConvexHull::remove_piece
  (unsigned int piece_index, unsigned int cloth_index, unsigned int * cloth)
{
  Element * piece = set->get_element (piece_index);
  int i = cloth_index,
  remaining_piece = atoi (piece->get_element_name ().c_str ());
  while ((i < cloth_size) && (remaining_piece > 0))
  {
    if ((cloth [i] == 0) && (piece->get_element_value (i - cloth_index) == 1))
      cout << "Error: trying to remove a non-inserted piece!\n";
    else if (piece->get_element_value (i - cloth_index) == 1)
      cloth [i] = 0;
    remaining_piece--;
    i++;
  }
}


struct struct_piece_node
{
  unsigned int piece_number;
  int cloth_index;
};
typedef struct_piece_node piece_node;


double TailorConvexHull::branch_and_bound (ElementSubset * X)
{
  unsigned int i, j, * cloth, * pieces,
               number_of_visited_nodes_during_branch_and_bound = 0;
  list<piece_node *> Stack;
  piece_node * current_piece, * next_piece;
  bool fit;

  double current_cost, best_cost;   // By definition of the cost function,
	                           // the cost of the cloth with no pieces is 1.
  if (X->is_empty ())
    return 1;

  cloth = new unsigned int [cloth_size + 1],
  pieces = new unsigned int [X->get_subset_cardinality () + 1];

  // Initializes an empty cloth.
  //
  for (i = 0; i < (unsigned int) cloth_size; i++)
    cloth[i] = 0;

  // Load the pieces indexes into the array.
  //
  j = 0;  // first piece has label zero
  for (i = 0; i < set->get_set_cardinality (); i++)
    if (X->has_element (i) )
    {
      pieces[j] = i;
      j++;
    }

  number_of_visited_nodes_during_branch_and_bound++;

  current_piece = new piece_node;   // Push the 1st piece into the stack.

  current_piece->piece_number = 0;  // First piece has index zero.

  current_piece->cloth_index = -1;  // First position of the cloth is zero.

  Stack.push_back (current_piece);  // -1 means that the piece wasn't fit into
                                    // the cloth yet.

  best_cost = -1; // If this value remains after the branch-and-bound procedure,
                  // then there is no match of the X pieces into the cloth.

  while (! Stack.empty ())
  {
    current_piece = Stack.back ();

    if (current_piece->cloth_index > -1)
    {
      remove_piece (pieces [current_piece->piece_number],
                    current_piece->cloth_index, cloth);

      current_piece->cloth_index++;
    }
    else
      current_piece->cloth_index = 0;

    fit = false;

    while ((current_piece->cloth_index < cloth_size) && (! fit))
    {
      fit = piece_fits_in_cloth (pieces [current_piece->piece_number],
                                 current_piece->cloth_index, cloth);
      if (! fit)
        current_piece->cloth_index++;
    }

    if (current_piece->cloth_index == cloth_size)
    {
      Stack.pop_back ();
      delete current_piece;
    }
    else
    {
      insert_piece (pieces [current_piece->piece_number],
                    current_piece->cloth_index, cloth);

      if (current_piece->piece_number == (X->get_subset_cardinality () - 1))
      {
        current_cost = ((double) current_piece->piece_number + 1) +
                       ((double) search_greatest_maximal_convex_hull (cloth) /
                        (double) cloth_size);
        if (current_cost > best_cost)
          best_cost = current_cost;
      }
      else
      {
        number_of_visited_nodes_during_branch_and_bound++;
        next_piece = new piece_node;
        next_piece->piece_number = current_piece->piece_number + 1;
        next_piece->cloth_index = -1;
        Stack.push_back (next_piece);
      }

    } // else

  } // while (! Stack.empty ())

  delete [] cloth;
  delete [] pieces;
  return best_cost;
}


double TailorConvexHull::cost (ElementSubset * X)
{
  timeval begin, end;
  gettimeofday (& begin, NULL);

  double cost;

  number_of_calls_of_cost_function++;

  if (set->get_set_cardinality () == 0)
    return 0;

  cost = branch_and_bound (X);

  gettimeofday (& end, NULL);
  elapsed_time_of_cost_function_calls += diff_us (end, begin);

  // Threshold is a maximum number of calls of the cost function.
  //
  if ((has_max_number_of_calls_set) &&
      (number_of_calls_of_cost_function >=
       max_number_of_calls_of_cost_function))
    reached_threshold = true;

  if (cost < 0)
    return INFTY;   // Infinity.
  else
    return (0 - cost);
}

