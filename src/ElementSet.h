//
// ElementSet.h -- definition of the class "ElementSet".
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

#ifndef ELEMENTSET_H_
#define ELEMENTSET_H_

#include "Element.h"

class ElementSet
{

private:

  // list_of_elements[0..number_of_elements-1]
  //
  Element * * list_of_elements;

  // true if it has extra elements (used, for instance, in W-operator
  // feature selection) and false otherwise.
  //
  bool has_extra_element;

  // Number of labels of an .DAT file (default == 2, for binary classification).
  //
  unsigned int number_of_labels;

  // Name of this set.
  //
  string name;

  // A real value associated to this set.
  //
  float value;

  // Number of elements contained by this set.
  //
  unsigned int number_of_elements;

  // container map; it is implemented as a self-balancing binary search tree
  // (namely a Red-black tree).
  //
  // This map is used to store a cost function explicitly defined, that is,
  // for each X \subseteq S, it is stored the value of c(X).
  //
  map <string, float> explicit_cost;

  // Auxiliary method to load .DAT files.
  //
  void load_dat_file (string file_name, unsigned int n);

public:

  // Default constructor: an empty set with a given name.
  //
  ElementSet (string);

  // Copy constructor
  //
  ElementSet (ElementSet *);

  // Constructor: a set with elements imported from a XML file.
  //
  ElementSet (string, string);

  // Constructor: a set with elements imported from a DAT file with n elements.
  // (number of labels specified by user).
  //
  ElementSet (unsigned int, string, unsigned int);

  // Constructor: a set with elements imported from a DAT file with n elements.
  // (binary labels).
  //
  ElementSet (string, unsigned int);

  // Constructor: a set with n random-valued elements with integers between
  // [0, range].
  //
  ElementSet (string, unsigned int, unsigned int);

  // Copy constructor
  // The new element set has only the elements determined by 'map'
  //
  ElementSet (ElementSet *, unsigned int *, unsigned int);

  // Default destructor.
  //
  virtual ~ElementSet ();

  // Return a pointer to the element at the "index" position. If there is no
  // element at such position, it returns NULL.
  //
  Element * get_element (unsigned int);

  // Return the cardinality of this set (i.e., the number of elements).
  //
  unsigned int get_set_cardinality ();

  // Receive a string corresponding to a subset and returns its cost,
  // explicitly declared through the input file.
  //
  float get_explicit_cost (string);

  // Return the name of this set.
  //
  string get_set_name ();

  // Return the real value associated to this set.
  //
  float get_set_value ();

  // Permute the elements of the set. Useful to run suboptimal and
  // stochastic experiments.
  //
  void permute_set ();

  // Print in the STDOUT all the elements of this set and their values.
  //
  void print_list_of_elements ();

  // Return the number of labels of a given set.
  //
  unsigned int get_number_of_labels ();
};

#endif /* ELEMENTSET_H_ */
