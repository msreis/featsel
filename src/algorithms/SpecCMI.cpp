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

#include "SpecCMI.h"


SpecCMI::SpecCMI ()
{
  list_of_visited_subsets = new Collection ();
  cost_function = NULL;
}


SpecCMI::~SpecCMI ()
{
  if (list_of_visited_subsets != NULL)
  delete list_of_visited_subsets;
}


void SpecCMI::set_k (unsigned int k_value)
{
  k = k_value;
}


void SpecCMI::compute_Q_matrix ()
{
  int index = 0, n = set->get_set_cardinality ();
  ElementSubset X ("subset", set);

  double * data = new double [n * n];
  
  for (int i = 0; i < n; i++)
    for (int j = 0; j < n; j++)
    {
      X.add_element (i);
      X.add_element (j);
      data[index] = cost_function->cost (&X);
      index++;
      X.remove_element (i);
      X.remove_element (j);
    }

  ae_int_t irows = n, icols = n;
  Q.setcontent (irows, icols, data);

  delete [] data;
}


void SpecCMI::get_minima_list (unsigned int max_size_of_minima_list)
{
  int n = set->get_set_cardinality ();

  timeval begin_program, end_program;
  gettimeofday (& begin_program, NULL);

  // IMPORTANT: this solver minimizes the following function:
  //     f(x) = 0.5*x'*A*x + b'*x.
  //
  // Note that quadratic term has 0.5 before it. So if you want to minimize
  // quadratic function, you should rewrite it in such way that quadratic term
  // is multiplied by 0.5 too.
  //
  // For example, if the function is f(x)=-(x0^2+x1^2), then we rewrite it as 
  //     f(x) = 0.5*(-2*x0^2-2*x1^2)
  // and pass diag(-2,-2) as quadratic term - NOT diag(-1,-1)!
  //

  // Create solver.
  //
  minqpstate state;
  minqpcreate (n, state);

  // Set the linear constraint: x_1 + ...+ x_n = k, which generalizes a 
  // constraint composed of the
  // Euclidean norm ||x|| = sqrt(k) for x_i in {0, 1}.
  //
  string str_c = "[[1";
  for (int i = 1; i < n; i++)
  {
    str_c.append (",1");
  }
  stringstream ss; ss << k;
  str_c.append ("," + ss.str () + "]]");
  real_2d_array c = str_c.c_str ();
  integer_1d_array ct = "[0]";
  minqpsetlc (state, c, ct);

  // Set upper and lower bounds.
  //
  string str_bndl = "[0", str_bndu = "[+inf";
  for (int i = 1; i < n; i++)
  {
    str_bndl.append (",0");
    str_bndu.append (",+inf");
  }
  str_bndl.append ("]");
  str_bndu.append ("]");
  real_1d_array bndl = str_bndl.c_str ();
  real_1d_array bndu = str_bndu.c_str ();
  minqpsetbc (state, bndl, bndu);

  // Upload the matrix of the quadratic term; since it is symmetric, the matrix
  // Q must have filled only its upper triangle.
  //
  compute_Q_matrix ();
  minqpsetquadraticterm (state, Q, true);

  // Set the starting point of the optimization.
  //
  string str_x0 = "[0";
  for (int i = 1; i < n; i++)
    str_x0.append (",0");
  str_x0.append ("]");
  real_1d_array x0   = str_x0.c_str ();
  minqpsetstartingpoint (state, x0);

  // Set scale of the parameters.
  // It is strongly recommended that you set scale of your variables.
  // Knowing their scales is essential for evaluation of stopping criteria
  // and for preconditioning of the algorithm steps.
  // You can find more information on scaling at 
  // http://www.alglib.net/optimization/scaling.php
  //
  string str_s = "[1";
  for (int i = 1; i < n; i++)
    str_s.append (",1");
  str_s.append ("]");
  real_1d_array s   = str_s.c_str ();
  minqpsetscale (state, s);

  // Solve problem with BLEIC-QP solver.
  // Default stopping criteria are used.
  //
  minqpsetalgobleic (state, 0.0, 0.0, 0.0, 0);
  minqpoptimize (state);

  // Collect the results.
  //
  minqpreport rep;
  real_1d_array x;
  minqpresults (state, x, rep);

  printf ("Report: %d\n", int (rep.terminationtype));
  printf ("Result: %s\n", x.tostring (2).c_str ()); // EXPECTED: [2,2]

  ElementSubset * X;
  X = new ElementSubset ("best", set);    
  X->cost = cost_function->cost (X);   
  list_of_minima.push_back (X); 

  number_of_visited_subsets =
  cost_function->get_number_of_calls_of_cost_function ();
  clean_list_of_minima (max_size_of_minima_list);

  gettimeofday (& end_program, NULL);
  elapsed_time_of_the_algorithm = diff_us (end_program, begin_program);

}


