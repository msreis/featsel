//
// CFS.cpp -- implementation of the class "CFS".
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

#include "CFS.h"

CFS::CFS (ElementSet * a_set)
{
  set = a_set;
  n = set->get_set_cardinality ();
  max_feature_value = 0;

  if (n > 0)
  {
    number_of_rows = set->get_element (0)->get_number_of_values ();

    for (unsigned int i = 0; i < n; i++)
      if (set->get_element (i)->get_max_value () > max_feature_value)
        max_feature_value = set->get_element (i)->get_max_value ();

    Pr_Y = new double [set->get_number_of_labels ()];
    Pr_X = new double [max_feature_value + 1];

    Pr_X1_X2 = new double * [max_feature_value + 1];
    Pr_X1_Y  = new double * [max_feature_value + 1];
    for (unsigned int i = 0; i <= max_feature_value; i++)
    {
      Pr_X [i] = 0;

      Pr_X1_X2 [i] = new double [max_feature_value + 1];
      for (unsigned int j = 0; j <= max_feature_value; j++)
        Pr_X1_X2 [i][j] = 0;
      Pr_X1_Y  [i] = new double [set->get_number_of_labels ()];
      for (unsigned int j = 0; j < set->get_number_of_labels (); j++)
        Pr_X1_Y [i][j] = 0;
    }

    correlation = new double * [n];
    H = new double [n+1];
    for (unsigned int i = 0; i < n; i++)
    {
      H[i] = -1;
      correlation[i] = new double [n];
      for (unsigned int j = 0; j < n; j++)
        correlation[i][j] = -1;
    }
    H[n] = compute_entropy (n);
  }
}


CFS::~CFS ()
{
  if (n > 0)
  {
    delete [] H;
    delete [] Pr_X;
    delete [] Pr_Y;

    for (unsigned int i = 0; i < set->get_set_cardinality (); i++)
        delete [] correlation [i];
    delete [] correlation;

    for (unsigned int i = 0; i <= max_feature_value; i++)
    {
      delete [] Pr_X1_X2 [i];
      delete [] Pr_X1_Y [i];
    }
    delete [] Pr_X1_X2;
    delete [] Pr_X1_Y;
  }
}


double CFS::compute_entropy_term (double x)
{
  if (x <= 0)
    return 0;
  else
    return x * log (x);
}


double CFS::compute_entropy (unsigned int i)
{
  double m = number_of_rows;

  if (i == n) // i is the label Y.
  {
    double sum_Y = 0;

    for (unsigned int label = 0; label < set->get_number_of_labels(); label++)
      Pr_Y [label] = 0;

    for (unsigned int k = 0; k < number_of_rows; k++)
      for (unsigned int label = 0; label < set->get_number_of_labels(); label++)
        Pr_Y [label] += set->get_element (n + label)->get_element_value (k);

    for (unsigned int label = 0; label < set->get_number_of_labels (); label++)
        sum_Y -= compute_entropy_term (Pr_Y [label]);

    return (sum_Y + compute_entropy_term (m)) / (m * log (2));
  }
  else // i is a feature
  {
    double sum_X = 0;      

    for (unsigned int k = 0; k < number_of_rows; k++)
        Pr_X [set->get_element (i)->get_element_value (k)]++;

    for (unsigned int ii = 0; ii <= max_feature_value; ii++)
    {
      sum_X -= compute_entropy_term (Pr_X [ii]);
      Pr_X [ii] = 0; // clean up for the next call.
    }
    return (sum_X + compute_entropy_term (m)) / (m * log (2));
  }
}


double CFS::compute_joint_entropy (unsigned int i, unsigned int j)
{
  double m = number_of_rows;

  if (i == j) // X2 is the label Y.
  {
    double sum_X1_Y = 0;

    for (unsigned int k = 0; k < number_of_rows; k++)
      for (unsigned int label = 0; label < set->get_number_of_labels(); label++)
        Pr_X1_Y [set->get_element (i)->get_element_value (k)][label] +=
          set->get_element (n + label)->get_element_value (k);

    for (unsigned int ii = 0; ii <= max_feature_value; ii++)
      for (unsigned int label =0; label < set->get_number_of_labels (); label++)
      {
        sum_X1_Y -= compute_entropy_term (Pr_X1_Y [ii][label]);
        Pr_X1_Y [ii][label] = 0; // clean up for the next call.
      }

    return (sum_X1_Y + compute_entropy_term (m)) / (m * log (2));
  }

  else // X2 is a feature.
  {
    double sum_X1_X2 = 0;

    for (unsigned int k = 0; k < number_of_rows; k++)
    {
      unsigned int X1_value = set->get_element (i)->get_element_value (k);
      unsigned int X2_value = set->get_element (j)->get_element_value (k);
      Pr_X1_X2 [X1_value][X2_value]++;
    }

    for (unsigned int ii = 0; ii <= max_feature_value; ii++)
      for (unsigned int jj =0; jj <= max_feature_value; jj++)
      {
        sum_X1_X2 -= compute_entropy_term (Pr_X1_X2 [ii][jj]);
        Pr_X1_X2 [ii][jj] = 0; // clean up for the next call.
      }

    return (sum_X1_X2 + compute_entropy_term (m)) / (m * log (2));
  }
}  


double CFS::symmetrical_uncertainity (unsigned int i, unsigned int j)
{ 
  double result = 0;

  // SU(X1,X2) = 2 * ((H(Xi) + H(Xj) - H(Xi,Xj)) / (H(Xi) + H(Xj))),
  //
  // where X1 is a feature and X2 is either another feature or the class Y.
  //
  if (H[i] == -1)
    H[i] = compute_entropy (i);

  if (H[j] == -1)
    H[j] = compute_entropy (j);

  if ((H[i] + H[j]) != 0)
  {
    // Compute H(X1), H(X2) and H(X1,X2).
    //
    double H_Xi_Xj = compute_joint_entropy (i, j);  

    if (i == j)
      result = 2 * ((H[i] + H[n] - H_Xi_Xj) / (H[i] + H[n]));
    else
      result = 2 * ((H[i] + H[j] - H_Xi_Xj) / (H[i] + H[j]));
  }
  return result;
}


double CFS::evaluateSubset (ElementSubset * X)
{
  // Merit(X) = (k * avg_f_c_corr) / sqrt (k + k * (k - 1) * avg_f_f_corr)
  //  
  double numerator   = 0, 
         denominator = X->get_subset_cardinality (); // k value.

  // Compute the numerator: 
  // 
  // (k * avg_f_c_corr) == k * 1/k * sum(corr[i][i]) == sum(corr[i][i]).
  //
  list <unsigned int>::iterator it_i, it_j, tmp;

  list <unsigned int> L; 

  X->copy_list (& L);

  for (it_i = L.begin (); it_i != L.end (); it_i++)
  {
    unsigned int i = *it_i;
    if (correlation[i][i] == -1)
      correlation[i][i] = symmetrical_uncertainity (i, i);
    numerator += correlation[i][i]; 
  }

  // Compute the denominator:
  //
  // sqrt (k + k * (k - 1) * avg_f_f_corr)
  //
  for (it_i = L.begin (); it_i != L.end (); it_i++)
  {
    unsigned int i = *it_i;
   
    tmp = it_i;
    tmp++; 
   
    for (it_j = tmp; it_j != L.end (); it_j++)
    {
      unsigned int j = *it_j;

      if (correlation[i][j] == -1)
      {
        correlation[i][j] = symmetrical_uncertainity (j, i);
        correlation[j][i] = correlation[i][j];
      }
      denominator += 2 * correlation[i][j]; 
    }
  }

  if (denominator == 0.0)
    return 0.0;

  double merit = numerator / sqrt (denominator);

  if (merit < 0.0)
      merit *= -1.0;

  return merit;
}


double CFS::cost (ElementSubset * X)
{
  timeval begin, end;
  gettimeofday (& begin, NULL);

  double cost;

  number_of_calls_of_cost_function++;

  if (set->get_set_cardinality () == 0)
    cost = 0;
  else if (X->get_subset_cardinality () == 0)
    cost = INFTY;
  else
    cost = - evaluateSubset (X);

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
