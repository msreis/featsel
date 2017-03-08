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
  m = 0;
  H_Y = -1;

  if (n > 0)
  {
    number_of_rows = set->get_element (0)->get_number_of_values ();
    correlation = new double * [n];

    H_X = new double [n];

    for (unsigned int i = 0; i < n; i++)
    {
      H_X[i] = -1;
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
    delete [] H_X;

    for (unsigned int i = 0; i < set->get_set_cardinality (); i++)
      delete [] correlation[i];
    delete [] correlation;
  }
}


void CFS::compute_H_Y ()
{
  // Computes H(Y):
  //
  double * Pr_Y = new double [set->get_number_of_labels ()];

  for (unsigned int i = 0; i < set->get_number_of_labels (); i++)
    Pr_Y[i] = 0;

  for (unsigned int k = 0; k < number_of_rows; k++)
  {  
    // Count frequency of each label.
    //
    for (unsigned int i = 0; i < set->get_number_of_labels (); i++)
    {
      Pr_Y[i] += set->get_element (n + i)->get_element_value (k);
            m += set->get_element (n + i)->get_element_value (k);
    }
  }

  H_Y = 0;

  for (unsigned int i = 0; i < set->get_number_of_labels (); i++)
    if (Pr_Y[i] > 0)
      H_Y -= (Pr_Y[i] / m) * (log (Pr_Y[i] / m) / log (2));

  delete [] Pr_Y;
}


void CFS::compute_H_X (unsigned int i)
{
  // Computes H(X):
  //
  map <unsigned int, double> Pr_X;
  map <unsigned int, double>::iterator it;

  for (unsigned int k = 0; k < number_of_rows; k++)
  {  
    // Count frequency of each label.
    //
    unsigned int frequency = 0;

    for (unsigned int j = 0; j < set->get_number_of_labels (); j++)
       frequency += set->get_element (n + j)->get_element_value (k);

    unsigned int value = set->get_element (i)->get_element_value (k);
  
    it = Pr_X.find (value);

    if (it == Pr_X.end ())
      Pr_X.insert (pair<unsigned int, unsigned int>(value, frequency));
    else
      Pr_X[value] += frequency;
  }

  H_X[i] = 0;

  for (it = Pr_X.begin (); it != Pr_X.end (); it++)
    if (it->second > 0)
      H_X[i] -= (it->second / m) * (log (it->second / m) / log (2));
}


double CFS::compute_H_X1_X2 (unsigned int i, unsigned int j)
{
  map <string, double> Pr_X1_X2;
  map <string, double>::iterator it;

  if (i == j) // X2 is the label Y.
  {
    for (unsigned int k = 0; k < number_of_rows; k++)
    {
      unsigned int X_value = set->get_element (i)->get_element_value (k);

      for (unsigned int label = 0; label < set->get_number_of_labels(); label++)
      {
        double frequency = set->get_element (n + label)->get_element_value (k);

        if (frequency > 0)
        {
          string X_Y_row_value;
          ostringstream oss;
          oss << X_value << " " << label;
          X_Y_row_value.append (oss.str ());

          it = Pr_X1_X2.find (X_Y_row_value);

          if (it == Pr_X1_X2.end ())
            Pr_X1_X2.insert (pair<string, double>(X_Y_row_value, frequency));
          else
            Pr_X1_X2 [X_Y_row_value] += frequency;
        }
      }
    }
  }
  else // X2 is a feature.
  {
    for (unsigned int k = 0; k < number_of_rows; k++)
    {
      unsigned int X1_value = set->get_element (i)->get_element_value (k);
      unsigned int X2_value = set->get_element (j)->get_element_value (k);

      string X1_X2_row_value;
      ostringstream oss;
      oss << X1_value << " " << X2_value;
      X1_X2_row_value.append (oss.str ());

      it = Pr_X1_X2.find (X1_X2_row_value);

      if (it == Pr_X1_X2.end ())
        Pr_X1_X2.insert (pair<string, double>(X1_X2_row_value, 1));
      else
        Pr_X1_X2 [X1_X2_row_value]++;
    }
  }

  double H_X1_X2 = 0;

  for (it = Pr_X1_X2.begin (); it != Pr_X1_X2.end (); it++)
    H_X1_X2 -= (it->second / m) * (log (it->second / m) / log (2));  

  return H_X1_X2;
}  


double CFS::symmetrical_uncertainity (unsigned int i, unsigned int j)
{ 
  if (H_Y == -1)
    compute_H_Y ();

  // SU(X1,X2) = 2 * ((H(X1) + H(X2) - H(X1,X2)) / (H(X1) + H(X2))),
  //
  // where X1 is a feature and X2 is either another feature or the class Y.

  if (H_X[i] == -1)
    compute_H_X (i);

  double H_X1 = H_X[i];

  // Compute the joint entropy of X1 and X2.
  //
  double H_X1_X2 = compute_H_X1_X2 (i, j);  

  double H_X2;

  if (i == j)
    H_X2 = H_Y;
  else
  {
    if (H_X[j] == -1)
      compute_H_X (j);
    H_X2 = H_X[j];
  }

  if ((H_X1 + H_X2) == 0)
    return 0;
  else
    return 2 * ((H_X1 + H_X2 - H_X1_X2) / (H_X1 + H_X2));
}


double CFS::evaluateSubset (ElementSubset * X)
{
  // Merit(X) = (k * avg_f_c_corr) / sqrt (k + k * (k - 1) * avg_f_f_corr)
  //  
  double   numerator = 0, 
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


float CFS::cost (ElementSubset * X)
{
  timeval begin, end;
  gettimeofday (& begin, NULL);

  float cost = 0;

  number_of_calls_of_cost_function++;

  if (set->get_set_cardinality () == 0)
    return cost;

  else if (X->get_subset_cardinality () == 0)
    return FLT_MAX;

  cost = (float) - evaluateSubset (X);

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
