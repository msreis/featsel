//
// MutualInformation.cpp -- implementation of the class "MutualInformation".
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

#include "MutualInformation.h"


MutualInformation::MutualInformation (ElementSet * a_set)
{
  set = a_set;
  m = 0;
  t = 0;
  if (! (set->get_set_cardinality() == 0))
    t = set->get_element (0)->get_number_of_values ();
}


MutualInformation::~MutualInformation ()
{
  // Empty destructor.
}


double MutualInformation::cost (ElementSubset * X)
{
  timeval begin, end;
  gettimeofday (& begin, NULL);
  double cost = INFTY;

  number_of_calls_of_cost_function++;

  if (set->get_set_cardinality () == 0)
    return 0;

  if (! (X->get_set_cardinality() == 0))
  {
    if (X->get_subset_cardinality() > 0)
      cost = calculate_MI (X);
    else
      cost = INFTY;
  }

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


double MutualInformation::calculate_MI (ElementSubset * X)
{
  double MCE = 0, 
        H_Y = 0;
  map <string, unsigned int *>::iterator it;

  // Clean m value.
  //
  m = 0; 

  // Calculate the distribution of X = x and initialize sample attribute.
  //
  unsigned int * freq_Y = calculate_distributions_from_the_samples (X);

  // Calculate H(Y).
  //
  for (unsigned int i = 0; i < set->get_number_of_labels (); i++)
  {
    // H(Y) -= Pr(Y=y) * log (Pr(Y = y))
    //
    double Pr_Y_is_y = (double) freq_Y[i] / (double) m;

    H_Y -=  Pr_Y_is_y * (log (Pr_Y_is_y) /
                        log ((double) set->get_number_of_labels ()));
  }

  delete [] freq_Y;

  // Calculates the MCE and clear the table of distribution of
  // X taken from the samples.
  //
  for (it = samples.begin (); it != samples.end (); it++)
  {
    // E[H(Y|X)] = - \sum_{x in X} Pr(X=x) * H(Y|X=x)
    //
    double Pr_X_is_x = 0;
    for (unsigned int i = 0; i < set->get_number_of_labels (); i++)
      Pr_X_is_x += (double) it->second[i] / (double) m;
    
    MCE -= calculate_conditional_entropy (it->second, Pr_X_is_x);
  }

  for (it = samples.begin (); it != samples.end (); it++)
    delete [] it->second;

  samples.clear ();

  return (double) MCE - H_Y;   // I(X;Y) = H(Y)-H(Y|X) => - I(X;Y) = H(Y|X)-H(Y)
}


double MutualInformation::calculate_conditional_entropy
(unsigned int * x, double Pr_X_is_x)
{
  unsigned int y;
  double  Pr_Y_is_y_given_x,
         H_of_Y_given_x = 0;

  for (y = 0; y < set->get_number_of_labels (); y++)
  {
    Pr_Y_is_y_given_x = ((double) x[y] / m) / Pr_X_is_x;

    if (Pr_Y_is_y_given_x > 0)
      H_of_Y_given_x += Pr_Y_is_y_given_x *
      (log (Pr_Y_is_y_given_x) /
       log ((double) set->get_number_of_labels ()));
  }

  return Pr_X_is_x * H_of_Y_given_x;
}


unsigned int * MutualInformation::calculate_distributions_from_the_samples
(ElementSubset * X)
{
  unsigned int i, j, k;

  map <string, unsigned int *>::iterator it;

  unsigned int * freq_Y = new unsigned int [set->get_number_of_labels ()];
  
  for (k = 0; k < set->get_number_of_labels (); k++)
    freq_Y[k] = 0;

  for (j = 0; j < set->get_element (0)->get_number_of_values (); j++)
  {
    string observation;

    for (i = 0; i < set->get_set_cardinality (); i++)
    {
      if (X->has_element (i))
      {
        ostringstream oss;
        oss << set->get_element (i)->get_element_value (j);
        observation.append (oss.str ());
      }
      else
      {
        observation.append ("X");
      }
    }

    it = samples.find (observation);

    // First occurrence of the subset
    //
    if ((it == samples.end ()))
    {
      unsigned int * row = new unsigned int [set->get_number_of_labels ()];
   
      for (k = 0; k < set->get_number_of_labels (); k++)
      {
        row[k] = set->get_element
                 (set->get_set_cardinality () + k)->get_element_value (j);
        m         += row[k];
        freq_Y[k] += row[k];
      }
      samples.insert (pair<string, unsigned int *> (observation, row));
    }

    // Increment the occurrence of X
    //
    else
    {
      for (k = 0; k < set->get_number_of_labels (); k++)
      {
        unsigned int label_value = set->get_element
             (set->get_set_cardinality () + k)->get_element_value (j);

        it->second[k] += label_value;
        m             += label_value;
        freq_Y[k]     += label_value;
      }
    }   

    observation.clear ();
  }

  return freq_Y;
}
