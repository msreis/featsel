//
// DatParserDriver.cpp -- implementation of the class "DatParserDriver".
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

# include "DatParserDriver.h"
# include "../global.h"

using namespace std;

DatParserDriver::DatParserDriver ()
{
  list_of_elements = NULL;
  number_of_elements = 0;
  number_of_labels = 0;
}


DatParserDriver::~DatParserDriver ()
{
  if (list_of_elements != NULL)
  {
    for (unsigned int i = 0; i < number_of_elements + number_of_labels; i++)
      delete list_of_elements[i];
    delete [] list_of_elements;
  }
}

int DatParserDriver::parse (unsigned int n, unsigned int l, string f)
{
  // Example of a line of a .dat file with two subsets of a set of 5 elements
  // and three labels:
  //
  // 0 1 1 0 1 11 27 1212\n
  // 1 0 0 1 0  7 320  0\n
  //
  // For each row, the first five digits are the characteristic vector of the
  // subset, and the last three numbers are the number of occurrences of the
  // labels "0", "1" and "2" to the respective subset.
  //
  unsigned int i, current_value, seen_labels;
  long begin, end;
  ifstream my_file (f.c_str ());

  number_of_elements = n;

  number_of_labels = l;

  begin = my_file.tellg ();
  my_file.seekg (0, ios::end);
  end = my_file.tellg ();
  my_file.seekg (0, ios::beg);

  // A conservative estimation, since the values of the last two numbers
  // often have more than 2 digits.
  //
  max_number_of_values = (end - begin) /
                         (2 * (number_of_elements + number_of_labels) + 1);

  // The extra positions are to store the classification for a given subset,
  // which requires two index per label (label index, number of 
  // occurrences)
  //
  list_of_elements = new Element * [number_of_elements + 2 * number_of_labels + 1];
  if (list_of_elements == 0)
  {
    cout << "Error in ElementSet: could not allocate memory for " <<
    "ElementSet elements!" << endl;
    return 1;
  }

  for (i = 0; i < number_of_elements + 2 * number_of_labels + 1; i++)
    list_of_elements [i] = new Element (max_number_of_values, "");

  i = 0;
  seen_labels = 0;
  while (my_file >> current_value)
  {
    if (i < number_of_elements)
    {
      list_of_elements[i]->add_element_value (current_value);
    }
    else if (current_value > 0)
    {
      unsigned int l_idx = number_of_elements + 1 + seen_labels * 2;
      list_of_elements[l_idx]->add_element_value 
        (i - number_of_elements);
      list_of_elements[l_idx + 1]->add_element_value (current_value);
      //cout << "current value = " << current_value << endl;
      seen_labels++;
      //unsigned int k = list_of_elements[l_idx]->get_number_of_values ();
      //cout << "Sample " << k << " has " << current_value << " occurrences of label " << i - number_of_elements << endl;
      //cout << "l_idx = " << l_idx << endl;
    }
    i++;
    if (i == (number_of_elements + number_of_labels))
    {
      list_of_elements[number_of_elements]->add_element_value 
        (seen_labels);
      i = 0;
      for (unsigned int j = number_of_elements + 1 + seen_labels * 2;
          j < number_of_elements + 2 * number_of_labels + 1; j++)
        list_of_elements[j]->add_element_value (0);
      seen_labels = 0;
  
      //unsigned int k = list_of_elements[0]->get_number_of_values () - 1;
      //for (unsigned int j = 0; j < number_of_labels * 2 + 1 + number_of_elements; j++)
      //{
        //cout << list_of_elements[j]->get_element_value (k) << " ";
        //if (j == number_of_elements - 1)
        //{
          //cout << "\t";
        //}
      //}
      //cout << endl;
    }
  }
  my_file.close ();

  //for (unsigned int j = 0; 
      //j < list_of_elements[0]->get_number_of_values (); j++)
  //{
    //seen_labels = 
      //list_of_elements[number_of_elements]->get_element_value (j);
    //cout << "Sample " << j << " has " << seen_labels << " labels" << endl;
    //cout << "\t";
    //for (unsigned l = 0; l < seen_labels; l++)
    //{
      //cout << list_of_elements[number_of_elements + 1 + 2 * l]->get_element_value (j) << ": " << list_of_elements[number_of_elements + 2 + 2 * l]->get_element_value (j) << " ";
    //}
    //cout << endl;
  //}
  //cout << "nof_values = " << list_of_elements[59]->get_number_of_values () << endl;

  // After-processing: we chance labels occurrence storage to a
  // (label index, frequency) fashion
  //
  return 0;
}


