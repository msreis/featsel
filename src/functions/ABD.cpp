//
// ABD.cpp -- implementation of the class "ABD".
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

#include "ABD.h"


ABD::ABD (ElementSet * a_set)
{
  set = a_set;
  c_max = C_MAX_VALUE;
  n = set->get_set_cardinality ();

  if (n > 0)
  {
    W = new double * [n];

    F0 = new ElementSubset ("F0", set);

    for (unsigned int i = 0; i < n; i++)
    {
      // Values of the positive-definite matrix.
      //
      W[i] = new double [n];
      for (unsigned int j = 0; j < n; j++)
         W[i][j] = set->get_element (i)->get_element_value (j);

      // For each element i, if its value with index n is "1", then i 
      // belongs to the global optimum F0.
      //
      if (set->get_element (i)->get_element_value (n) == 1)
        F0->add_element (i);
    }
  }
}


ABD::~ABD ()
{
  if (n > 0)
  {
    delete F0;
    for (unsigned int i = 0; i < n; i++)
      delete[] W[i];
    delete [] W;
  }
}


double ABD::compute_products (double * F_minus_F0)
{
  // first_product = (F - F0)^T * W.
  //
  double * first_product = new double [n];
  for (unsigned int j = 0; j < n; j++)    // slide through the columns
  {
    first_product[j] = 0;
    for (unsigned int i = 0; i < n; i++)  // slide through the rows
    {
      first_product[j] += F_minus_F0[i] * W[i][j];
    }
  }

  // second_product = first_product * (F - F0).
  //
  double second_product = 0;
  for (unsigned int i = 0; i < n; i++)
    second_product += first_product[i] * F_minus_F0[i];

  delete first_product;
  return second_product;
}


// c(F) = c_max * (1 - exp (-1/2 * (F - F0)^T * W * (F - F0) ) ). 
//
double ABD::cost (ElementSubset * F)
{
  timeval begin, end;
  gettimeofday (& begin, NULL);

  double cost = 0;

  number_of_calls_of_cost_function++;

  if (set->get_set_cardinality () == 0)
    return cost;

  double * F_minus_F0 = new double [n];

  for (unsigned int i = 0; i < n; i++)
    if (((F0->has_element (i)) && (F->has_element (i))) || 
        (!F0->has_element (i)))
      F_minus_F0[i] = 0;
    else
      F_minus_F0[i] = 1;

  cost = 1 - exp ((-0.5) * compute_products (F_minus_F0));

  delete F_minus_F0;

  gettimeofday (& end, NULL);
  elapsed_time_of_cost_function_calls += diff_us (end, begin);

  // Threshold is a maximum number of calls of the cost function
  //
  if ((has_max_number_of_calls_set) &&
      (number_of_calls_of_cost_function >=
       max_number_of_calls_of_cost_function))
    reached_threshold = true;

  return cost;
}
