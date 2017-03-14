//
// Element.h -- definition of the class "Element".
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

#ifndef ELEMENT_H_
#define ELEMENT_H_

#include "global.h"


class Element
{

private:

  // The maximum value of this element.
  //
  unsigned int max_value;

  // The maximum number of values that can be added into this element.
  //
  unsigned int max_number_of_values;

  // The name of the element.
  //
  string name;

  // The current number of values added into this element.
  //
  unsigned int number_of_values;

  // The array containing the added values.
  //
  int * value;

public:

  // Default constructor, gets the max number of values and the element's name.
  //
  Element (unsigned int, string);

  // Copy constructor
  //
  Element (Element *);

  // Default destructor.
  //
  virtual ~Element ();

  // Add a value into the element.
  // Return true if it was successfully added and false otherwise.
  //
  bool add_element_value (int);

  // Return the name of the element.
  //
  string get_element_name ();

  // Return the value of the element at position "index". If there
  // is no such element, returns -1 and prints a warning in the STDOUT.
  //
  int get_element_value (unsigned int);

  // Return the maximum number of values of this element.
  //
  unsigned int get_max_number_of_values ();

  // Return the maximum value.
  //
  unsigned int get_max_value ();

  // Return the number of values added already.
  //
  unsigned int get_number_of_values ();

  // Print the name of the element and its values in the STDOUT.
  //
  void print_element ();

  // Sets a new name to the element.
  //
  void set_element_name (string);

};

#endif /* ELEMENT_H_ */
