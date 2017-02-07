//
// ElementSubset.cpp -- implementation of the class "ElementSubset".
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

#include "ElementSubset.h"


ElementSubset::ElementSubset (string subset_name, ElementSet * a_set)
{
  unsigned int number_of_elements = a_set->get_set_cardinality ();
  set = a_set;
  subset_cardinality = 0;
  Y = NULL;
  cost = 0;

  if (subset_name.length () > 0)
    name = subset_name;
  else
    name = "X";

  if (number_of_elements == 0)
    list_of_elements = NULL;

  else
  {
    list_of_elements = new (nothrow) bool [number_of_elements];
    if (list_of_elements == NULL)
      cout << "Error in ElementSubset: no memory available!\n";
    // initializes with an empty set
    set_empty_subset ();
  }
}


ElementSubset::~ElementSubset ()
{
  if (list_of_elements != NULL)
    delete [] list_of_elements;
  if (Y != NULL)
    delete [] Y;
  L.clear ();
}


//
// Methods that read AND WRITE data.
//

bool ElementSubset::add_element (unsigned int index)
{
  if (index < set->get_set_cardinality ())
  {
    if (!list_of_elements[index])
    {
      subset_cardinality++;
      list_of_elements[index] = true;
      L.push_back (index);
    }
    return true;
  }
  else
    return false;
}


bool ElementSubset::remove_element (unsigned int index)
{
  if (index < set->get_set_cardinality ())
  {
    if (list_of_elements[index])
    {
      subset_cardinality--;
      list_of_elements[index] = false;
      L.remove (index);
    }
    return true;
  }
  else
    return false;
}


unsigned int ElementSubset::remove_random_element ()
{
  unsigned int i = set->get_set_cardinality ();
  if (L.size () > 0)
  {
    i = L.front ();
    L.pop_front ();
    list_of_elements[i] = false;
    subset_cardinality--;
  }
  return i;
}


void ElementSubset::set_empty_subset ()
{
  unsigned int i;
  for (i = 0; i < set->get_set_cardinality (); i++)
    list_of_elements[i] = false;
  subset_cardinality = 0;
  L.clear ();
}


void ElementSubset::set_complete_subset ()
{
  unsigned int i, j, k, n, * uniform_permutation;
  list <unsigned int>::iterator it;
  n = subset_cardinality = set->get_set_cardinality ();

  uniform_permutation = new (nothrow) unsigned int [n];
  if (uniform_permutation == NULL)
    cout << "Error in ElementSubset::set_complete_subset :" <<
    " no memory available!\n";

  L.clear ();  // if there are some elements they will be reinserted latter

  for (i = 0; i < n; i++)
    uniform_permutation[i] = i;

  // PERMUTE-IN-PLACE algorithm
  for (i = 0; i < n - 1; i++)
  {
    j = ((unsigned int) rand () % (n-i)) + i;  // random number in [i, n]
    k = uniform_permutation[i];
    uniform_permutation[i] = uniform_permutation[j];
    uniform_permutation[j] = k;
  }

  for (i = 0; i < n; i++)
  {
    L.push_back(uniform_permutation[i]);
    list_of_elements[i] = true;
  }

  if (uniform_permutation != NULL)
    delete [] uniform_permutation;
}


void ElementSubset::copy (ElementSubset * Y)
{
  unsigned int i;
  set_complete_subset (); // to guarantee uniform permutation of elements
  for (i = 0; i < set->get_set_cardinality (); i++)
    if (! Y->has_element (i))
      remove_element (i);
  cost = Y->cost;
}


void ElementSubset::set_complement_subset ()
{
  unsigned int i;
  ElementSubset A ("", set);
  A.copy (this);
  set_complete_subset (); // to guarantee the uniform permutation of elements
  for (i = 0; i < set->get_set_cardinality (); i++)
    if (A.has_element (i))
  remove_element (i);
}


void ElementSubset::subset_intersection (ElementSubset * X)
{
  unsigned int i;
  for (i = 0; i < set->get_set_cardinality (); i++)
    if (! X->has_element (i))
  remove_element (i);
}


void ElementSubset::subset_union (ElementSubset * X)
{
  unsigned int i;
  for (i = 0; i < set->get_set_cardinality (); i++)
    if (X->has_element (i))
  add_element (i);
}


//
// Methods that just read data.
//

bool ElementSubset::is_equal(ElementSubset * Y)
{
  unsigned int i;
  for (i = 0; i < set->get_set_cardinality (); i++)
    if (has_element (i) != Y->has_element (i))
      return false;
  return true;
}


bool ElementSubset::has_element (unsigned int index)
{
  if (index >= set->get_set_cardinality ())
  {
    cout << "Error in ElementSubset::has_element:" <<
    " index out of range!" << endl;
    return false;
  }
  else return list_of_elements[index];
}


ElementSet * ElementSubset::get_set_that_contains_this_subset ()
{
  return set;
}


unsigned int ElementSubset::get_set_cardinality ()
{
  return set->get_set_cardinality ();
}


unsigned int ElementSubset::get_subset_cardinality ()
{
  return subset_cardinality;
}


bool ElementSubset::contains (ElementSubset * Y)
{
  unsigned int i;
  for (i = 0; i < set->get_set_cardinality (); i++)
    if ((! has_element (i)) && (Y->has_element (i)))
      return false;
  return true;
}


bool ElementSubset::is_contained_by(ElementSubset * Y)
{
  unsigned int i;
  for (i = 0; i < set->get_set_cardinality (); i++)
    if ((has_element (i)) && (!Y->has_element (i)))
      return false;
  return true;
}


bool ElementSubset::is_empty ()
{
  if (subset_cardinality == 0)
    return true;
  else
    return false;
}


string ElementSubset::print_subset ()
{
  unsigned int i;
  string subset_string (" <");
  for (i = 0; i < set->get_set_cardinality (); i++)
    if (has_element (i))
      subset_string.append ("1");
    else
      subset_string.append ("0");
  subset_string.append ("> ");
  return subset_string;
}


string ElementSubset::print_subset_in_hex ()
{
  unsigned int i;
  unsigned long l = 0;
  std::ostringstream stm;
  string subset_string;
  for (i = 0; i < set->get_set_cardinality (); i++)
    if (has_element (i))
      l += pow ((double) 2, (int) i);
  stm << hex << l;
  subset_string = stm.str ();
  return subset_string;
}



