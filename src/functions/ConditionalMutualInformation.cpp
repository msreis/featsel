//
// ConditionalMutualInformation.cpp -- implementation of the class
//                                     "ConditionalMutualInformation".
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

#include "ConditionalMutualInformation.h"


ConditionalMutualInformation::ConditionalMutualInformation (ElementSet * a_set)
{
  set = a_set;
  Pr_Y = NULL;
}


ConditionalMutualInformation::~ConditionalMutualInformation ()
{
  // Empty destructor.
  if (Pr_Y != NULL)
    delete[] Pr_Y;
}


ConditionalMutualInformation * ConditionalMutualInformation::get_copy ()
{
  return new ConditionalMutualInformation (this->set);
}

// Compute I(x; Y) = \sum_{x,Y} P(x, Y) log P(x, Y) / (P(x) P(Y)).
//
double ConditionalMutualInformation::mutual_information (unsigned int x1)
{
  double result = 0;

  // Table of frequencies and its pointer.
  //
  map<unsigned int, SampleLabels *> table;
  map<unsigned int, SampleLabels *>::iterator it;
  SampleLabels::iterator l_it;

  // Number of samples m and set cardinality n.
  //
  unsigned int number_of_rows = set->get_element (0)->get_number_of_values (),
               m = 0; // Remember that m is the number of samples, and the way
                      // .DAT is implemented it allows more than one per row.

  for (unsigned int k = 0; k < number_of_rows; k++)
  {
    SampleLabels * row;
   
    // Count frequency of each label for the current observation x1.
    //
    pair<unsigned int, SampleLabels *> sample_entry;
    sample_entry = set->get_sample_labels_map (k);
    m += sample_entry.first;
    row = sample_entry.second;

    // x1 observation value.
    //
    unsigned int x1_row_value = set->get_element (x1)->get_element_value (k);
    it = table.find (x1_row_value);

    if (it == table.end ())
    {
      table.insert (pair<unsigned int, SampleLabels *> (x1_row_value, 
            row));
    }
    else
    {
      for (l_it = row->begin (); l_it != row->end (); l_it++)
      {
        unsigned int y, y_freq;
        y = l_it->first;
        y_freq = l_it->second;
        (*table[x1_row_value])[y] += y_freq;
      }
      delete row;
    }
  }

  // Compute P(Y = y) for all y in Y.
  //
  if (Pr_Y == NULL)
  {
    Pr_Y = new double[set->get_number_of_labels ()];
    for (unsigned int i = 0; i < set->get_number_of_labels (); i++)
      Pr_Y[i] = 0;
    for (it = table.begin (); it != table.end (); it++)
      for (l_it = it->second->begin (); l_it != it->second->end (); 
          l_it++)
        Pr_Y[l_it->first] += l_it->second;
    for (unsigned int i = 0; i < set->get_number_of_labels (); i++)
      Pr_Y[i] /= (double) m;
  }
  
  // sum_{y \in Y}, iterates over all labels.
  //
  for (unsigned int i = 0; i < set->get_number_of_labels (); i++)
  {
    //
    // sum_{x \in x1}, iterates over the observed values of x1.
    //
    for (it = table.begin (); it != table.end (); it++)
    {
      // P(x1=x)
      //
      double Pr_X = 0;
      for (l_it = it->second->begin (); l_it != it->second->end (); 
          l_it++)
        Pr_X += l_it->second;
      Pr_X /= (double) m;

      // Pr(x1=x, Y=y)
      //
      double Pr_X_and_Y = 0;
      if (it->second->find (i) != it->second->end ())
        Pr_X_and_Y = (double) (*(it->second))[i] / (double) m;

      // P(x1=x, Y=y) log P(x1=x, Y=y) / (P(x1=x) P(Y=y)).
      //
      if ((Pr_X_and_Y != 0) && (Pr_X != 0) && (Pr_Y[i] != 0))
        result += Pr_X_and_Y * (log (Pr_X_and_Y / (Pr_X * Pr_Y[i])) /
                                log ((double) set->get_number_of_labels ()));
    }
  }

  for (it = table.begin (); it != table.end (); it++)
    delete it->second;

  return result;
}


// Compute I(x1; Y | x2) =
//      \sum_{x1,Y,x2} P(x1, Y, x2) log (P(x1, Y, x2) P(x2))/(P(x1,x2) P(Y,x2)).
//
double ConditionalMutualInformation::conditional_mutual_information
  (unsigned int x1, unsigned int x2)
{
  double result = 0;
  unsigned int m = 0; // number of samples.

  // Table of frequencies and its pointer.
  //
  map<string, SampleLabels *> table_x1_x2_Y;
  map<string, SampleLabels *>::iterator it;
  map<unsigned int, SampleLabels *> table_x1_Y;
  map<unsigned int, SampleLabels *>::iterator it1;
  map<unsigned int, SampleLabels *> table_x2_Y;
  map<unsigned int, SampleLabels *>::iterator it2;
  SampleLabels::iterator l_it;

  // Number of samples m and set cardinality n.
  //
  unsigned int number_of_rows = set->get_element (0)->get_number_of_values ();

  for (unsigned int k = 0; k < number_of_rows; k++)
  {
    SampleLabels * row;

    // Count frequency of each label for the current observation of x1 and x2.
    //
    pair<unsigned int, SampleLabels *> sample_entry;
    sample_entry = set->get_sample_labels_map (k);
    m += sample_entry.first;
    row = sample_entry.second;

    // <x1,x2> observation value.
    //
    unsigned int x1_value = set->get_element (x1)->get_element_value (k);
    unsigned int x2_value = set->get_element (x2)->get_element_value (k);
    string x1_x2_row_value;
    ostringstream oss;
    oss << x1_value << " " << x2_value;
    x1_x2_row_value.append (oss.str ());
    it = table_x1_x2_Y.find (x1_x2_row_value);
    if (it == table_x1_x2_Y.end ())
    {
      SampleLabels * new_row = new SampleLabels ();
      *new_row = *row;
      table_x1_x2_Y.insert (pair<string, SampleLabels *> 
          (x1_x2_row_value, new_row));
    }
    else
    {
      for (l_it = row->begin (); l_it != row->end (); l_it++)
      {
        unsigned int y, y_freq;
        y = l_it->first;
        y_freq = l_it->second;
        (*table_x1_x2_Y[x1_x2_row_value])[y] += y_freq;
      }
    }

    // x1 only observation value.
    //
    it1 = table_x1_Y.find (x1_value);
    if (it1 == table_x1_Y.end ())
    {
      SampleLabels * new_row = new SampleLabels ();
      *new_row = *row;
      table_x1_Y.insert (pair<unsigned int, SampleLabels *>(x1_value, 
            new_row));
    }
    else
    {
      for (l_it = row->begin (); l_it != row->end (); l_it++)
      {
        unsigned int y, y_freq;
        y = l_it->first;
        y_freq = l_it->second;
        (*table_x1_Y[x1_value])[y] += y_freq;
      }
    }

    // x2 only observation value.
    //
    it2 = table_x2_Y.find (x2_value);
    if (it2 == table_x2_Y.end ())
    {
      SampleLabels * new_row = new SampleLabels ();
      *new_row = *row;
      table_x2_Y.insert (pair<unsigned int, SampleLabels *>(x2_value, 
            new_row));
    }
    else
    {
      for (l_it = row->begin (); l_it != row->end (); l_it++)
      {
        unsigned int y, y_freq;
        y = l_it->first;
        y_freq = l_it->second;
        (*table_x2_Y[x2_value])[y] += y_freq;
      }
    }
    delete row;
  }

  // sum_{y \in Y}, iterates over all labels.
  //
  for (unsigned int i = 0; i < set->get_number_of_labels (); i++)
  {
    SampleLabels * x2_labels, * x1x2_labels;
    //
    // sum_{x \in x1}, iterates over the observed values of x1.
    //
    for (it1 = table_x1_Y.begin (); it1 != table_x1_Y.end (); it1++)
    {
      //
      // sum_{x \in x2}, iterates over the observed values of x2.
      //
      for (it2 = table_x2_Y.begin (); it2 != table_x2_Y.end (); it2++)
      {

        // P(x1=x, Y=y, x2=x')
        //
        double Pr_X1_Y_X2 = 0;
        ostringstream oss;
        oss << it1->first << " " << it2->first;
        string x1_x2_value;
        x1_x2_value.append (oss.str ());
        it = table_x1_x2_Y.find (x1_x2_value);
        if (it != table_x1_x2_Y.end ())
          if (it->second->find (i) != it->second->end ())
            Pr_X1_Y_X2 = (*(it->second))[i] / (double) m;

        // P(x2=x')
        //
        double Pr_X2 = 0;
        x2_labels = it2->second;
        for (l_it = x2_labels->begin (); l_it != x2_labels->end (); 
            l_it++)
          Pr_X2 += l_it->second;
        Pr_X2 /= (double) m;

        // P(X1=x, X2=x')
        //
        double Pr_X1_X2 = 0;
        if (it != table_x1_x2_Y.end ())
        {
          x1x2_labels = it->second;
          for (l_it = x1x2_labels->begin (); 
              l_it != x1x2_labels->end (); l_it++)
            Pr_X1_X2 += l_it->second;
        }
        Pr_X1_X2 /= (double) m;

        // P(Y=y, X2=x')
        //
        double Pr_Y_X2 = (double) (*(it2->second))[i] / (double) m;

        // P(x1=x, Y=y, x2=x') log (P(x1=x, Y=y, x2=x') P(x2=x'))/
        //                          (P(x1=x,x2=x') P(Y=y,x2=x'))
        //
        if ((Pr_X1_Y_X2 != 0) && (Pr_X2 != 0) &&
            (Pr_X1_X2 != 0) && (Pr_Y_X2 != 0))
          result += Pr_X1_Y_X2 * (log ((Pr_X1_Y_X2 * Pr_X2)/
            (Pr_X1_X2 * Pr_Y_X2)) / log ((double) set->get_number_of_labels ()));
      }
    }
  }

  for (it = table_x1_x2_Y.begin (); it != table_x1_x2_Y.end (); it++)
    delete it->second;
  for (it1 = table_x1_Y.begin (); it1 != table_x1_Y.end (); it1++)
    delete it1->second;
  for (it2 = table_x2_Y.begin (); it2 != table_x2_Y.end (); it2++)
    delete it2->second;
  return result;
}


double ConditionalMutualInformation::cost (ElementSubset * X)
{
  timeval begin, end;
  gettimeofday (& begin, NULL);

  number_of_calls_of_cost_function++;

  double cost;

  // c(X) = I(x; Y), X = { x }.
  //
  if (X->get_subset_cardinality () == 1)
  {
    unsigned int x1 = X->get_set_cardinality ();
    for (unsigned int i = 0; i < X->get_set_cardinality (); i++)
      if (X->has_element (i))
        x1 = i;

    cost = mutual_information (x1);
  }
  //
  // c(X) = 1/2 * (I(x1; Y | x2) + I(x2; Y | x1)), X = { x1, x2 }.
  //
  else if (X->get_subset_cardinality () == 2)
  {
    unsigned int x1 = X->get_set_cardinality (), // x1 = |S| says that it
                 x2 = X->get_set_cardinality (); // has no valid index yet.

    for (unsigned int i = 0; i < X->get_set_cardinality (); i++)
      if (X->has_element (i))
      {
        if (x1 == X->get_set_cardinality ())
          x1 = i;
        else
          x2 = i;
      }

    cost = 0.5 * (conditional_mutual_information (x1, x2) +
                  conditional_mutual_information (x2, x1));
  }
  //
  // |X| != 1 and |X| != 2
  //
  else
  {
    cost = 0;
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
