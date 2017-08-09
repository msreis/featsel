//
// MeanConditionalEntropyMock.cpp -- implementation of the mock class
// "MeanConditionalEntropyMock".
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

#include "MeanConditionalEntropyMock.h"


MeanConditionalEntropyMock::MeanConditionalEntropyMock (ElementSet * set) :
MeanConditionalEntropy (set)
{
  // Empty constructor.
}


MeanConditionalEntropyMock::~MeanConditionalEntropyMock ()
{
  // Empty destructor.
}


string MeanConditionalEntropyMock::print_label_frequencies (ElementSubset * X)
{
  map <string, unsigned int *>::iterator it;
  string printed_samples ("");
  std::ostringstream value;

  calculate_distributions_from_the_samples (X);

  for (it = samples.begin (); it != samples.end (); it++)
  {
    printed_samples.append (it->first);
    printed_samples.append (": ");
    value <<  (int) it->second[0];
    printed_samples.append (value.str ());
    value.str ("");
    printed_samples.append (" ");
    value <<  (int) it->second[1];
    printed_samples.append (value.str ());
    value.str ("");
    printed_samples.append ("\n");
    delete[] it->second;
  }
  samples.clear ();

  return printed_samples;
}


string MeanConditionalEntropyMock::print_W_operator_samples_frequencies
(ElementSubset * X)
{
  map <string, unsigned int *>::iterator it;
  string printed_samples ("");
  std::ostringstream value;

  calculate_distributions_from_the_samples (X);

  for (it = samples.begin (); it != samples.end (); it++)
  {
    printed_samples.append (it->first);
    printed_samples.append (": ");
    value <<  (int) it->second[0] + it->second[1];
    delete[] it->second;
    it->second = NULL;
    printed_samples.append (value.str ());
    value.str ("");
    printed_samples.append ("\n");
  }

  samples.clear ();

  return printed_samples;
}


string MeanConditionalEntropyMock::print_conditional_entropy (ElementSubset * X)
{
  map <string, unsigned int *>::iterator it;
  string printed_samples ("");
  std::ostringstream value;

  m = 0;

  calculate_distributions_from_the_samples (X);

  for (it = samples.begin (); it != samples.end (); it++)
  {
    double Pr_X_is_x = 0;
    for (unsigned int i = 0; i < set->get_number_of_labels (); i++)
      Pr_X_is_x += (double) it->second[i] / (double) m;

    printed_samples.append (it->first);
    printed_samples.append (": ");
    value <<  calculate_conditional_entropy (it->second, Pr_X_is_x);
    delete[] it->second;
    printed_samples.append (value.str ());
    value.str ("");
    printed_samples.append ("\n");
  }

  samples.clear ();

  return printed_samples;
}

