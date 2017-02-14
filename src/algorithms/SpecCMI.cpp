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
  cost_function = NULL;                        // Main cost function.
  cmi = NULL;                                  // Cost function of the Q matrix.
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
  cmi = new ConditionalMutualInformation (set);

  this->Q = new double * [n];
  
  for (int i = 0; i < n; i++)
  {
    this->Q[i] = new double [n];    

    for (int j = 0; j < n; j++)
    {
      X.add_element (i);
      X.add_element (j);
      this->Q[i][j] = cmi->cost (&X);
      index++;
      X.remove_element (i);
      X.remove_element (j);
    }
  }

  delete cmi;
}


double SpecCMI::get_Q_matrix_value (unsigned int i, unsigned int j)
{
  return Q[i][j];
}


// Algorithm 8.5.3 of the book "Numerical Linear Algebra and Applications",
// from Biswa Datta.
// 
double * SpecCMI::Rayleigh (double epsilon, double max_num_iter, double * x_0)
{
  int n = set->get_set_cardinality ();
  double err = epsilon + 1;

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

  double num_iter = 0;

  while ((err > epsilon) && (num_iter <= max_num_iter))
  {
    num_iter++;

    // sigma = (x' * A * x) / (x' * x);
    //
    double sigma = (x.transpose () * A * x) / (x.transpose () * x);

    // y  = (A - sigma * I)^(-1) * x
    //
    B = A - (sigma * I);
    y = B.solve (x);

    // x = y / ||y||.
    //
    in(0) = y;
    out = Fnorm (in, 1);             // Original algorithm: Fmax instead Fnorm.
    x = y / out(0).double_value ();

    // Compute err = ||(A - sigma * I) * x||.
    //
    in(0) = B * x;
    out = Fnorm (in, 1);
    err = out(0).double_value ();
  }

  double * result = new double [n];

  for (octave_idx_type i = 0; i < n; i++)
    result[i] = x(i);

  if (num_iter >= max_num_iter)
    cout << "ERROR: Rayleigh algorithm did not converge!" << endl;

  return result;
}


double * SpecCMI::rank_features ()
{
  compute_Q_matrix ();

  unsigned int n = set->get_set_cardinality ();

  // Initial estimate for a dominant eigenvector, which must be an unit-norm
  // vector, that is, ||x|| = sqrt (x1^2 + ... + xn^2) = 1.
  //
  double * x = new double [n];
  for (unsigned int i = 0; i < n; i++)
    x[i] =  1; // sqrt (1 / n);

  double * result = Rayleigh ((double) EPSILON, MAX_NUM_ITER, x);

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
  map <double, unsigned int>::reverse_iterator it;

  for (unsigned int i = 0; i < set->get_set_cardinality (); i++)
  {
    feature_queue.insert (pair<double, unsigned int>(result[i], i));
  }

  ElementSubset X ("", set), * Y;

  Y = new ElementSubset ("", set);
  Y->copy (&X);
  Y->cost = cost_function->cost (Y);   
  list_of_minima.push_back (Y); 

  // map C++ container has its keys ordered by default.
  //
  for (it = feature_queue.rbegin (); it != feature_queue.rend (); it++)
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


