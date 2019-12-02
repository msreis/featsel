//
// CostFunction.h -- definition of the class "CostFunction".
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

#ifndef COST_FUNCTION_H_
#define COST_FUNCTION_H_

#include "global.h"
#include "ElementSet.h"
#include "ElementSubset.h"

#include <sys/time.h>
#include <unistd.h>

// The sleep time (in microseconds), in order to guarantee that the
// computation of the "cost" method takes at least SLEEP_TIME usecs.
// (this is useful in tests where it is important to separate the
// demanded time of the cost function from the rest of the program).
//
#define SLEEP_TIME 1


class CostFunction
{

protected:
    
  // Struct used to store a sample class
  //
  typedef map<unsigned int, unsigned int> SampleLabels;


  // Return the difference (in microseconds) between t2 and t1
  //
  int diff_us (timeval t1, timeval t2);

  // Store the time (in microseconds) spent by all
  // calls of the cost function
  //
  int elapsed_time_of_cost_function_calls;

  // True if this has max number of calls of the cost function, false otherwise
  //
  bool has_max_number_of_calls_set;

  // True if this has threshold value, false otherwise
  //
  bool has_threshold_set;

  // Maximum number of calls that the "cost" method.
  // is allowed to perform. Used in heuristic mode 2.
  //
  unsigned int max_number_of_calls_of_cost_function;

  // The number of calls of the "cost" method.
  //
  unsigned int number_of_calls_of_cost_function;

  // True if this cost function achieves the 'threshold' value
  // during the heuristic mode execution, and false otherwise.
  //
  bool reached_threshold;

  // The set that is the domain of the cost function
  //
  ElementSet * set;

  // Threshold value, used in heuristic mode 1
  // (pre-processing).
  //
  double threshold;

public:

  // Default constructor.
  //
  CostFunction ();


  // Default destructor.
  //
  virtual ~CostFunction ();


  // Store some elapsed time for all calls of the cost function. This
  // method is usesul to establish a basal time for unexpensive cost functions.
  //
  void add_elapsed_time (int microseconds);


  // Return the value of c(X), where X is a subset.
  //
  virtual double cost (ElementSubset *) = 0;


  // Return the number of times the "cost" method was called.
  //
  unsigned int get_number_of_calls_of_cost_function ();


  // Return the elapsed time of all calls of the cost function
  //
  int get_the_elapsed_time_of_the_calls_of_this_cost_function ();


  // Return the maximum number of calls of the cost function
  //
  unsigned int get_max_number_of_calls_of_cost_function ();


  // Return the threshold value
  //
  double get_threshold ();


  // Return true if this cost function is limited either by a 'threshold'
  // or a maximum number of calls of the cost function, and false otherwise.
  //
  bool has_threshold ();


  // Return true if this cost function has computed a value that is
  // equal or lower than the 'threshold' and false otherwise.
  //
  bool has_reached_threshold ();


  // Set the maximum number of calls of the cost function
  //
  void set_max_number_of_calls_of_cost_function (unsigned int);


  // Set the threshold value
  //
  void set_threshold (double);

};

#endif /* COST_FUNCTION_H_ */
