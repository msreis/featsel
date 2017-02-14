//
// Solver.h -- definition of the class "Solver".
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

//
// This is an abstract class; solver implementations should inherit this class.
//

#ifndef SOLVER_H_
#define SOLVER_H_

#include "global.h"
#include "ElementSet.h"
#include "ElementSubset.h"
#include "Collection.h"
#include "CostFunction.h"


class Solver
{

protected:

  // Name of the used cost function.
  //
  CostFunction * cost_function;

  // Return the difference between times in microseconds
  //
  int diff_us (timeval, timeval);

  // Store the time (in microseconds) spent by the
  // execution of the solver
  //
  int elapsed_time_of_the_algorithm;

  // Store the the number of iterations of a given solver
  //
  unsigned int number_of_iterations;

  // Functor with operator () as a comparison function.
  //
  struct Functor
  {
    CostFunction * cf; // It is not computed if the cost is already stored
    bool operator () (ElementSubset * first, ElementSubset * second)
    {
      if (first->cost < second->cost)
        return true;
      else
        return false;
    }
  };

  // Clean the minima list up to 'max_number_of_minima'
  //
  void clean_list_of_minima (unsigned int);

  // Collection of the minima subsets found by the solver.
  //
  list<ElementSubset *> list_of_minima;

  // Collection of the subsets visited by the search.
  //
  Collection * list_of_visited_subsets;

  // Number of visited subsets of the search.
  //
  unsigned int number_of_visited_subsets;

  // The set used in the search of the minima subsets.
  //
  ElementSet * set;

  // True if the search must keep the visited subsets, false otherwise.
  //
  bool store_visited_subsets;

public:

  // Default constructor.
  //
  Solver ();

  // Default destructor.
  //
  virtual ~Solver ();

  // Generate a list of minima with up to n subsets (abstract method).
  //
  virtual void get_minima_list (unsigned int) = 0;

  // Return the size of the obtained list of minima.
  //
  unsigned int number_of_minima ();

  // Return the elapsed time of the solver execution.
  //
  int get_elapsed_time_of_the_algorithm ();

  // Return the number of iterations of a given algorithm.
  //
  unsigned int get_number_of_iterations ();

  // Return a string with all the subsets of the list of minima.
  // (e.g. returns " [010] [101] [111] ").
  //
  string print_list_of_minima ();

  // Return a string with all the subsets visited by the search.
  // (e.g. returns " [000] [001] [011] ... ").
  //
  string print_list_of_visited_subsets ();

  // Return the number of visited subsets during a search.
  //
  unsigned int print_number_of_visited_subsets ();

  // Return a string with all the subsets of the list of minima with their
  // respective cost (one subset-cost per line).
  // (e.g. returns " [010] : 1.12
  //                 [101] : 0.73
  //                 [111] : 0.12 ").
  //
  string print_search_report ();

  // Sets the used set, the cost function, and 'true' if it should store the
  // visited subsets, 'false' otherwise.
  //
  void set_parameters (CostFunction *, ElementSet *, bool);

  // Given the subset X, returns the calculated subset cost.
  //
  float subset_cost (ElementSubset *);

  // If "store_visited_subsets" is true, then this method stores a copy of X
  // into list_of_visited_subsets.
  //
  void store_visited_subset (ElementSubset * X);

  // This method stores a copy of X
  // into list_of_minima.
  //
  void store_minimum_subset (ElementSubset * X);

  // Returns a pointer to the list of visited subsets
  //
  Collection * get_list_of_visited_subsets ();

  // Returns a copy of list_of_minima
  //
  list<ElementSubset *> get_list_of_minima ();

};

#endif /* SOLVER_H_ */
