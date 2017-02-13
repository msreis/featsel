//
// MeanConditionalEntropy.cpp -- implementation of the class
// "MeanConditionalEntropy".
//
//    This file is part of the featsel program
//    Copyright (C) 2015  Marcelo S. Reis
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
#include <cfloat>


MeanConditionalEntropy::MeanConditionalEntropy (ElementSet * a_set)
{
  set = a_set;

  // Penalized Mean Conditional Entropy reads from a .DAT and
  // is used for W-operator feature selection.
  //
  if (! (set->get_set_cardinality() == 0))
    t = set->get_element (0)->get_number_of_values ();
}


MeanConditionalEntropy::~MeanConditionalEntropy ()
{
  // Empty destructor.
}


float MeanConditionalEntropy::cost (ElementSubset * X)
{
  timeval begin, end;
  gettimeofday (& begin, NULL);
  usleep (SLEEP_TIME);
  float cost = 0;
  number_of_calls_of_cost_function++;

  //

  if (! (X->get_set_cardinality() == 0))
  {
    // Penalized Mean Conditional Entropy
    //
    if (X->get_subset_cardinality() > 0)
      cost = calculate_MCE (X);
    else
      cost = INFINITY_COST;  // infinity
  }

  //

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

/* ------------------------------------------------------------------------- */

//
// The following functions are used to get a MCE value:
//

float MeanConditionalEntropy::calculate_MCE (ElementSubset * X)
{
  float Pr_X_is_x, cost = 0;
  map <string, ElementSubset *>::iterator it;
  ElementSet S ("S", X->get_subset_cardinality (), 1);

  // calculates the distribution of X = x through the samples
  //
  calculate_distributions_from_the_samples (& S, X);

  // calculates the MCE and clear the table of distribution of X taken from
  // the samples
  //
  for (it = samples.begin (); it != samples.end (); it++)
  {
    if (it->second->cost > 1)
    {
      // Pr(X=x) * H(Y|X=x)
      //
      Pr_X_is_x = it->second->cost / t;
      cost += Pr_X_is_x * calculate_conditional_entropy (it->second);
    }
    else
      // if X=x has only one occurrence, it is penalized with 1 / t
      //
      cost += (float) 1 / t;

    delete it->second;
  }
  samples.clear ();

  return cost;
}


float MeanConditionalEntropy::calculate_conditional_entropy (ElementSubset * x)
{
  unsigned int y;
  float  Pr_Y_is_y_given_x,
         H_of_Y_given_x = 0;

  for (y = 0; y < set->get_number_of_labels (); y++)
  {
    Pr_Y_is_y_given_x = (float) x->Y[y] /
                        x->cost; // cost stores the sum of Y[i..k]
    if (Pr_Y_is_y_given_x > 0)
      H_of_Y_given_x -= Pr_Y_is_y_given_x *
      (log (Pr_Y_is_y_given_x) / log ((float) set->get_number_of_labels ()));
  }

  return H_of_Y_given_x;
}


void MeanConditionalEntropy::calculate_distributions_from_the_samples
(ElementSet * S, ElementSubset * X)
{
  unsigned int i, j, k;
  ElementSubset A ("", S), * B;
  map <string, ElementSubset *>::iterator it;

  for (j = 0; j < set->get_element (0)->get_number_of_values (); j++)
  {
    A.set_empty_subset ();
    k = 0;  // Index of the elements of the subset A,
            // given the index of the elements of X
    for (i = 0; i < set->get_set_cardinality (); i++)
    {
      if (X->has_element (i))
      {
        if (set->get_element (i)->get_element_value (j) == 1)
          A.add_element (k);
        k++;
      }
    }

    it = samples.find (A.print_subset ());

    // First occurrence of the subset
    //
    if ((it == samples.end ()))
    {
      B = new ElementSubset ("", S);
      B->copy (& A);
      B->Y = new (nothrow) unsigned int [set->get_number_of_labels ()];
      B->cost = 0;
   
      for (k = 0; k < set->get_number_of_labels (); k++)
      {
        B->Y[k] = set->get_element
        (set->get_set_cardinality () + k)->get_element_value (j);
        B->cost += B->Y[k];
      }
      samples.insert (pair<string, ElementSubset *> (B->print_subset (), B));

    }

    // Increment the occurrence of X
    //
    else
    {
      for (k = 0; k < set->get_number_of_labels (); k++)
      {
        it->second->Y[k] += set->get_element
        (set->get_set_cardinality () + k)->get_element_value (j);
        it->second->cost += set->get_element
        (set->get_set_cardinality () + k)->get_element_value (j);
      }
    }
  }
}
