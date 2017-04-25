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

  if (n > 0)
  {
    number_of_rows = set->get_element (0)->get_number_of_values ();
    correlation = new double * [n];

    for (unsigned int i = 0; i < n; i++)
    {
      correlation[i] = new double [n];
      for (unsigned int j = 0; j < n; j++)
        correlation[i][j] = -1;
    }
  }
}


CFS::~CFS ()
{
  if (n > 0)
  {
    for (unsigned int i = 0; i < set->get_set_cardinality (); i++)
      delete [] correlation[i];
    delete [] correlation;
  }
}


void CFS::compute_entropies
(unsigned int i, unsigned int j, double * H_X1, double * H_X2, double * H_X1_X2)
{
  double * Pr_X1, * Pr_X2, ** Pr_X1_X2, 
         m = number_of_rows;

  Pr_X1 = new double [set->get_element(i)->get_max_value() + 1];

  Pr_X1_X2 = new double * [set->get_element(i)->get_max_value() + 1];

  if (i == j) // X2 is the label Y.
  {
    Pr_X2 = new double [set->get_number_of_labels ()];

    for (unsigned int ii = 0; ii <= set->get_element(i)->get_max_value(); ii++)
    { 
      Pr_X1_X2[ii] = new double [set->get_number_of_labels ()];
      Pr_X1[ii] = 0;
      for (unsigned int jj = 0; jj < set->get_number_of_labels (); jj++)
      {
        Pr_X1_X2[ii][jj] = 0;
        Pr_X2[jj] = 0;
      }
    }

    for (unsigned int k = 0; k < number_of_rows; k++)
    {
      unsigned int X_value = set->get_element (i)->get_element_value (k);

      for (unsigned int label = 0; label < set->get_number_of_labels(); label++)
      {
        double frequency = set->get_element (n + label)->get_element_value (k);
        Pr_X1_X2 [X_value][label] += frequency;
        Pr_X1 [X_value] += frequency;
        Pr_X2 [label] += frequency; 
      }
    }

    for (unsigned int jj = 0; jj < set->get_number_of_labels (); jj++)
      if (Pr_X2[jj] > 0)
        *H_X2 -= (Pr_X2[jj]/m) * (log (Pr_X2[jj]/m) / log (2));

    for (unsigned int ii = 0; ii <= set->get_element(i)->get_max_value(); ii++)
    {
      if (Pr_X1[ii] > 0)
        *H_X1 -= (Pr_X1[ii]/m) * (log (Pr_X1[ii]/m) / log (2));

      for (unsigned int jj = 0; jj < set->get_number_of_labels (); jj++)
        if (Pr_X1_X2 [ii][jj] > 0)
          *H_X1_X2 -= (Pr_X1_X2[ii][jj]/m) *
                      (log (Pr_X1_X2[ii][jj]/m) / log (2));

      delete[] Pr_X1_X2 [ii];
    }
  }

  else // X2 is a feature.
  {
    Pr_X2 = new double [set->get_element(j)->get_max_value() + 1];

    for (unsigned int ii = 0; ii <= set->get_element(i)->get_max_value(); ii++)
    { 
      Pr_X1_X2[ii] = new double [set->get_element(j)->get_max_value() + 1];
      Pr_X1[ii] = 0;
      for (unsigned int jj =0; jj <= set->get_element(j)->get_max_value(); jj++)
      {
        Pr_X1_X2[ii][jj] = 0;
        Pr_X2[jj] = 0;
      }
    }

    for (unsigned int k = 0; k < number_of_rows; k++)
    {
      unsigned int X1_value = set->get_element (i)->get_element_value (k);
      unsigned int X2_value = set->get_element (j)->get_element_value (k);
      Pr_X1_X2 [X1_value][X2_value]++;
      Pr_X1[X1_value]++;
      Pr_X2[X2_value]++;
    }

    for (unsigned int jj =0; jj <= set->get_element(j)->get_max_value(); jj++)
      if (Pr_X2[jj] > 0)
        *H_X2 -= (Pr_X2[jj]/m) * (log (Pr_X2[jj]/m) / log (2));

    for (unsigned int ii = 0; ii <= set->get_element(i)->get_max_value(); ii++)
    {
      if (Pr_X1[ii] > 0)
        *H_X1 -= (Pr_X1[ii]/m) * (log (Pr_X1[ii]/m) / log (2));

      for (unsigned int jj =0; jj <= set->get_element(j)->get_max_value(); jj++)
        if (Pr_X1_X2 [ii][jj] > 0)
          *H_X1_X2 -= (Pr_X1_X2[ii][jj]/m) *
                      (log (Pr_X1_X2[ii][jj]/m) / log (2));

      delete[] Pr_X1_X2 [ii];
    }
  }

  delete[] Pr_X1;
  delete[] Pr_X2;
  delete[] Pr_X1_X2;
}  


double CFS::symmetrical_uncertainity (unsigned int i, unsigned int j)
{ 
  // SU(X1,X2) = 2 * ((H(X1) + H(X2) - H(X1,X2)) / (H(X1) + H(X2))),
  //
  // where X1 is a feature and X2 is either another feature or the class Y.

  double H_X1 = 0, H_X2 = 0, H_X1_X2 = 0;

  // Compute H(X1), H(X2) and H(X1,X2).
  //
  compute_entropies (i, j, &H_X1, &H_X2, &H_X1_X2);  

  if ((H_X1 + H_X2) == 0)
    return 0;
  else
    return 2 * ((H_X1 + H_X2 - H_X1_X2) / (H_X1 + H_X2));
}


double CFS::evaluateSubset (ElementSubset * X)
{
  // Merit(X) = (k * avg_f_c_corr) / sqrt (k + k * (k - 1) * avg_f_f_corr)
  //  
  double numerator = 0, 
         denominator = X->get_subset_cardinality (); // k value.

  // Compute the numerator: 
  // 
  // (k * avg_f_c_corr) == k * 1/k * sum(corr[i][i]) == sum(corr[i][i]).
  //
  for (unsigned int i = 0; i < n; i++)
  {
    if (X->has_element (i))
    {
      if (correlation[i][i] == -1)
        correlation[i][i] = symmetrical_uncertainity (i, i);
      numerator += correlation[i][i]; 
    }
  }

  // Compute the denominator:
  //
  // sqrt (k + k * (k - 1) * avg_f_f_corr)
  //
  for (unsigned int i = 0; i < n; i++)
  {
    if (X->has_element (i))
    {
      for (unsigned int j = i + 1; j < n; j++)
      {
        if (X->has_element (j))
        {
          if (correlation[i][j] == -1)
            correlation[i][j] = correlation[j][i] 
                              = symmetrical_uncertainity (i, j);
          denominator += 2 * correlation[i][j]; 
        }
      }
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

  double cost = 0;

  number_of_calls_of_cost_function++;

  if (set->get_set_cardinality () == 0)
    return cost;

  else if (X->get_subset_cardinality () == 0)
    return INFTY;

  cost = - evaluateSubset (X);

  gettimeofday (& end, NULL);
  elapsed_time_of_all_calls_of_the_cost_function += diff_us (end, begin);

  // Threshold is a maximum number of calls of the cost function
  //
  if ((has_max_number_of_calls_set) &&
      (number_of_calls_of_cost_function >=
       max_number_of_calls_of_cost_function))
    reached_threshold = true;

  return cost;
}
