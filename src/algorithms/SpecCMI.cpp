//
// SpecCMI.cpp -- implementation of the class "SpecCMI".
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

# include "SpecCMI.h"

# include <oct.h>
# include <builtin-defun-decls.h>

SpecCMI::SpecCMI ()
{
  list_of_visited_subsets = new Collection ();
  cost_function = NULL;
  Q = NULL;
}


SpecCMI::~SpecCMI ()
{
  if (list_of_visited_subsets != NULL)
  delete list_of_visited_subsets;
  if (Q != NULL)
  {
    for (unsigned int i = 0; i < set->get_set_cardinality (); i++)
      delete [] Q[i];
    delete [] Q;
  }
}

void SpecCMI::compute_Q_matrix ()
{
  int index = 0, n = set->get_set_cardinality ();
  ElementSubset X ("subset", set);

  this->Q = new double * [n];
  
  for (int i = 0; i < n; i++)
  {
    this->Q[i] = new double [n];    

    for (int j = 0; j < n; j++)
    {
      X.add_element (i);
      X.add_element (j);
      this->Q[i][j] = cost_function->cost (&X);
      index++;
      X.remove_element (i);
      X.remove_element (j);
    }
  }
}


double SpecCMI::get_Q_matrix_value (unsigned int i, unsigned int j)
{
  return Q[i][j];
}


double * SpecCMI::Rayleigh (double epsilon, double mu, double * x_0)
{
  int n = set->get_set_cardinality ();

  octave_value_list in;     // Input/Output lists to exchange data with 
  octave_value_list out;    // octace native functions.

  Matrix A  = Matrix (n, n),
         B  = Matrix (n, n),
         I  = Matrix (n, n);

  ColumnVector x = ColumnVector (n),
               y = ColumnVector (n);

  for (octave_idx_type i = 0; i < n; i++)
  {
    x(i) = x_0[i];

    for (octave_idx_type j = 0; j < n; j++)
      A(i,j) = this->Q[i][j];
  }

  // Compute an identity matrix with n rows and n columns.
  //
  in(0) = n;                    
  out = Feye (in, 1);
  I = out(0).matrix_value ();

  // Compute the norm of row vector x.
  //
  in(0) = x;
  out = Fnorm (in, 1);
  x = x / out(0).double_value ();

  // Compute y = (A - mu * I)^(-1) * x.
  //
  B = A - (mu * I);
  y = B.solve (x); //  B \ x  <=>  B^(-1) * x

  // Compute y' * x.
  //
  double lambda = y.transpose () * x;

  mu = mu + (1 / lambda);

  // Compute err = ||y - lambda * x|| / ||y||.
  //
  in(0) = y - lambda * x;
  out = Fnorm (in, 1);
  double err = out(0).double_value ();
  in(0) = y;
  out = Fnorm (in, 1);
  err = err / out(0).double_value ();

  while (err > epsilon)
  {
    in(0) = y;
    out = Fnorm (in, 1);
    x = y / out(0).double_value ();  // x = y / ||y||.

    B = A - (mu * I);
    y = B.solve (x);

    lambda = y.transpose () * x;

    mu = mu + (1 / lambda);
 
    // Compute err = ||y - lambda * x|| / ||y||.
    //
    in(0) = y - lambda * x;
    out = Fnorm (in, 1);
    err = out(0).double_value ();
    in(0) = y;
    out = Fnorm (in, 1);
    err = err / out(0).double_value ();
  }

  double * result = new double [n];

  for (octave_idx_type i = 0; i < n; i++)
    result[i] = x(i);

  return result;
}


double * SpecCMI::rank_features ()
{
  compute_Q_matrix ();

  double epsilon = 0.0001;    // Precision of the Rayleigh quotient iteration.

  double      mu = 0;         // Initial estimate for the dominant eigenvalue.

  // Initial estimate for a dominant eigenvector.
  //
  double * x = new double [set->get_set_cardinality ()];
  for (unsigned int i = 0; i < set->get_set_cardinality (); i++)
    x[i] = 1;

  double * result = Rayleigh (epsilon, mu, x);

  delete [] x;

  return result;
}


void SpecCMI::get_minima_list (unsigned int max_size_of_minima_list)
{
  timeval begin_program, end_program;
  gettimeofday (& begin_program, NULL);

  compute_Q_matrix ();

  // Result contains the rank of each feature. Now we need to, starting by the
  // empty set, include one feature at a time and compute the cost function.
  // The subset with lowest cost in this process will be returned as the
  // best one according to the SPEC-CMI algorithm ranking coupled with the
  // chosen cost function (e.g. regular Mutual Information).
  // 
  double * result = rank_features ();

  map <double, unsigned int> feature_queue;
  map <double, unsigned int>::iterator it;

  for (unsigned int i = 0; i < set->get_set_cardinality (); i++)
  {
    result[i] = abs (result[i]); // TODO: must check Theorem 2 and 
                                 // other results in Vinh et al. (2014).
    feature_queue.insert (pair<double, unsigned int>(result[i], i));
  }

  ElementSubset X ("", set), * Y;

  Y = new ElementSubset ("", set);
  Y->copy (&X);
  Y->cost = cost_function->cost (Y);   
  list_of_minima.push_back (Y); 

  // map C++ container has its keys ordered by default.
  //
  for (it = feature_queue.begin (); it != feature_queue.end (); it++)
  {
    X.add_element (it->second);
    Y = new ElementSubset ("", set);
    Y->copy (&X);
    Y->cost = cost_function->cost (Y);   
    list_of_minima.push_back (Y); 
  }

  delete [] result;

  number_of_visited_subsets =
  cost_function->get_number_of_calls_of_cost_function ();
  clean_list_of_minima (max_size_of_minima_list);

  gettimeofday (& end_program, NULL);
  elapsed_time_of_the_algorithm = diff_us (end_program, begin_program);

}


