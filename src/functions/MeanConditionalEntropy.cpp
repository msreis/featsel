//
// MeanConditionalEntropy.cpp -- implementation of the class
// "MeanConditionalEntropy".
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

#include "MeanConditionalEntropy.h"

MeanConditionalEntropy::MeanConditionalEntropy (ElementSet * a_set)
{
  set = a_set;
}


MeanConditionalEntropy::~MeanConditionalEntropy ()
{
  // Empty destructor.
}


double MeanConditionalEntropy::cost (ElementSubset * X)
{
  timeval begin, end;
  gettimeofday (& begin, NULL);
  double cost = 0;
  number_of_calls_of_cost_function++;

  
  if (! (X->get_set_cardinality() == 0))
  {
    // Penalized Mean Conditional Entropy
    //
    if (X->get_subset_cardinality() > 0)
      cost = calculate_MCE (X);
    else
      cost = INFTY;  // infinity
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

/* ------------------------------------------------------------------------- */

//
// The following functions are used to get a MCE value:
//

double MeanConditionalEntropy::calculate_MCE (ElementSubset * X)
{
  double cost = 0;
  map<string, SampleLabels *>::iterator it;

  // Samples for a W-operator feature selection.
  //
  map<string, SampleLabels *> samples;

  // Total number of samples
  //
  unsigned int m = 0;
    
  // calculates the distribution of X = x through the samples
  //
  calculate_distributions_from_the_samples (X, &samples, &m);

  // calculates the MCE and clear the table of distribution of X taken from
  // the samples
  //
  for (it = samples.begin (); it != samples.end (); it++)
  {
    // Pr(X=x) * H(Y|X=x)
    //
    double Pr_X_is_x = 0;
    SampleLabels * sample_labels = it->second;
    SampleLabels::iterator label_it = sample_labels->begin ();

    for (; label_it != sample_labels->end (); label_it++)
      Pr_X_is_x += (double) label_it->second / (double) m;

    if (Pr_X_is_x > ((double) 1 / m))
    {
      cost += Pr_X_is_x * calculate_conditional_entropy (sample_labels, 
          Pr_X_is_x, m);
    }
    else
      // if X=x has only one occurrence, it is penalized with 1 / t
      //
      cost += (double) 1 / m;

  }
  
  for (it = samples.begin (); it != samples.end (); it++)
    delete it->second;

  samples.clear ();
  return cost;
}


double MeanConditionalEntropy::calculate_conditional_entropy 
(SampleLabels * sample_labels, double Pr_X_is_x, unsigned int m)
{
  double  Pr_Y_is_y_given_x, H_of_Y_given_x = 0;
  SampleLabels::iterator label_it = sample_labels->begin ();

  for (; label_it != sample_labels->end (); label_it++)
  {
    Pr_Y_is_y_given_x = ((double) label_it->second / m) / Pr_X_is_x;

    if (Pr_Y_is_y_given_x > 0)
      H_of_Y_given_x -= Pr_Y_is_y_given_x *
      (log (Pr_Y_is_y_given_x) /
       log ((double) set->get_number_of_labels ()));
  }

  return H_of_Y_given_x;
}


void MeanConditionalEntropy::calculate_distributions_from_the_samples
(ElementSubset * X, map<string, SampleLabels *> * samples, 
 unsigned int * m)
{
  unsigned int i, j, k;

  map <string, SampleLabels *>::iterator it;

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

    it = samples->find (observation);

    // First occurrence of the subset
    //
    if ((it == samples->end ()))
    {
      SampleLabels * row = new SampleLabels ();
      for (k = 0; k < set->get_number_of_labels (); k++)
      {
        unsigned int y_idx, y_freq_given_x;
        y_idx = set->get_set_cardinality () + k;
        y_freq_given_x =
         set->get_element (y_idx)->get_element_value (j);
        if (y_freq_given_x > 0)
        {
          row->insert (make_pair (k, y_freq_given_x));
          *m += y_freq_given_x;
        }
      }
      samples->insert (pair<string, SampleLabels *> (observation, row));
    }

    // Increment the occurrence of X
    //
    else
    {
      for (k = 0; k < set->get_number_of_labels (); k++)
      {
        unsigned int y_freq_given_x = set->get_element
             (set->get_set_cardinality () + k)->get_element_value (j);
        (*it->second)[k] += y_freq_given_x;
        *m               += y_freq_given_x;
      }
    }
    observation.clear ();
  }
}
