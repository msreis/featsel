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

SpecCMI::SpecCMI ()
{
  list_of_visited_subsets = new Collection ();
  cost_function = NULL;                        // Main cost function.
  cmi = NULL;                                  // Cost function of the Q matrix.
  Q = NULL;
  k = 10;                          // Starting value at Xuan Vinh et al. (2014).
}


SpecCMI::SpecCMI (unsigned int number_of_features)
{
  list_of_visited_subsets = new Collection ();
  cost_function = NULL;                        // Main cost function.
  cmi = NULL;                                  // Cost function of the Q matrix.
  Q = NULL;
  k = number_of_features;
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


// Implemented pseudocode of Gauss--Jordan elimination:
// (https://en.wikipedia.org/wiki/Gaussian_elimination#Pseudocode)
//
// for k = 1 ... min(m,n):
//   Find the k-th pivot:
//   i_max  := argmax (i = k ... m, abs(A[i, k]))
//   if A[i_max, k] = 0
//     error "Matrix is singular!"
//   swap rows(k, i_max)
//   Do for all rows below pivot:
//   for i = k + 1 ... m:
//     f := A[i, k] / A[k, k]
//     Do for all remaining elements in current row:
//     for j = k + 1 ... n:
//       A[i, j]  := A[i, j] - A[k, j] * f
//     Fill lower triangular matrix with zeros:
//     A[i, k]  := 0
//
double * SpecCMI::Gauss_Jordan_elimination (unsigned int n, double ** A)
{
  for (unsigned int k = 0; k < n; k++)
  {
    // Compute the argmax.
    //
    unsigned int i_max = n;
    double v_max = 0;
    for (unsigned int i = k; i < n; i++)
      if (abs (A[i][k]) >= v_max)
      {
        i_max = i;
        v_max = abs (A[i][k]);
      }

    if (A[i_max][k] == 0)  // The matrix is singular.
      return NULL;

    // Swap rows k and i_max.
    //
    for (unsigned j = 0; j <= n; j++)
    {
      double aux;
      aux = A[k][j];
      A[k][j] = A[i_max][j];
      A[i_max][j] = aux;
    }

    // Process the rows below pivot.
    //
    for (unsigned int i = k + 1; i < n; i++)
    {
      double f = A[i][k] / A[k][k];

      for (unsigned int j = k + 1; j <= n; j++)
      {
        A[i][j] = A[i][j] - A[k][j] * f;
      }
      A[i][k] = 0;
    }
  }

  double * x = new double [n];
  for (unsigned int i = 0; i < n; i++)
    x[i] = 0; 

  // Retrieve the values of x through back substitution.
  //
  x[n-1] = A[n-1][n] / A[n-1][n-1];
  for (int i = n - 2; i >= 0; i--)
  {
    x[i] = A[i][n];
    for (unsigned int j = 0; j < n; j++)
      if ((unsigned int) i != j)
        x[i] -= A[i][j] * x[j];
    x[i] /= A[i][i];
  }

  return x;
}


// Algorithm 8.5.3 of the book "Numerical Linear Algebra and Applications",
// from Biswa Datta.
// 
double * SpecCMI::Rayleigh (double epsilon, double max_num_iter, double * x_0)
{
  unsigned int n = set->get_set_cardinality ();
  double err = epsilon + 1, num_iter = 0, * x, * y, ** A;

  A = new double * [n];
  x = new double [n];
  for (unsigned int i = 0; i < n; i++)
  {
    A[i] = new double [n + 1];
    for (unsigned int j = 0; j < n; j++)
      A[i][j] = Q[i][j];
    x[i] = x_0[i];
  }

  while ((err > epsilon) && (num_iter <= max_num_iter))
  {
    num_iter++;

    // sigma = (x' * Q * x) / (x' * x);
    //
    double num = 0, den = 0;
    for (unsigned int j = 0; j < n; j++)
    {
      double aux = 0;
      for (unsigned int i = 0; i < n; i++)
        aux += x[i] * A[i][j];
      num += aux * x[j];
      den += x[j] * x[j];
    }
    double sigma = num / den; 

    // y  = (Q - sigma * I)^(-1) * x
    //
    for (unsigned int i = 0; i < n; i++)
    {
      for (unsigned int j = 0; j < n; j++)
        A[i][j] = Q[i][j];
      A[i][i] -= sigma;
      A[i][n] = x[i];     // Augmented matrix.
    }      

    y = Gauss_Jordan_elimination (n, A);

    if (y == NULL)
    {
      if (VERBOSE)
        cout << "Warning: iteration " << num_iter
             << " yielded a singular matrix!" << endl;
      num_iter = max_num_iter + 1; // Force the end of the loop.
    } 
    else
    {
      // x = y / ||y||.
      //
      double norm_y, sum = 0;
      for (unsigned int i = 0; i < n; i++)
        sum += y[i] * y[i];
      norm_y = sqrt (sum);
      for (unsigned int i = 0; i < n; i++)
        x[i] = y[i] / norm_y;

      delete[] y;

      // Compute err = ||(A - sigma * I) * x||.
      //
      sum = 0;
      for (unsigned int i = 0; i < n; i++)
      {
        double value = 0;
        for (unsigned int j = 0; j < n; j++)
        {
          if (i == j)
            value += (A[i][i] - sigma) * x[j];
          else
            value += A[i][j] * x[j];
        }
        sum += value * value;
      }
      err = sqrt (sum);
    }
  }

  if ((num_iter >= max_num_iter) && (VERBOSE))
    cout << "Warning: Rayleigh algorithm did not converge!" << endl;

  for (unsigned int i = 0; i < n; i++)
    delete[] A[i];

  delete[] A;

  return x;
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
    x[i] =  sqrt (1 / (double) n);

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
  unsigned int i = k;

  // map C++ container has its keys ordered by default.
  //
  for (it = feature_queue.rbegin (); 
       (i >= 1) && (it != feature_queue.rend ()); it++)
  {
    /* Remove the comment tags to print into STDOUT the chain [\emptyset, X_k],
       where X_k is the subset with k best-ranked features.

    Y = new ElementSubset ("", set);
    Y->copy (&X);
    Y->cost = cost_function->cost (Y);   
    list_of_minima.push_back (Y); 

    */
    X.add_element (it->second); 
    i--;
  }

  Y = new ElementSubset ("", set);
  Y->copy (&X);
  Y->cost = cost_function->cost (Y);   
  list_of_minima.push_back (Y); 

  delete [] result;

  number_of_visited_subsets =
  cost_function->get_number_of_calls_of_cost_function ();
  clean_list_of_minima (max_size_of_minima_list);

  gettimeofday (& end_program, NULL);
  elapsed_time_of_the_algorithm = diff_us (end_program, begin_program);

}


