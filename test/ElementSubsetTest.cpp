//
// ElementSubsetTest.cpp -- implementation of the namespace "ElementSubsetTest"
//
//    This file is part of the featsel program
//    Copyright (C) 2010  Marcelo S. Reis
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

#include "ElementSubsetTest.h"

namespace ElementSubsetTest
{

  bool a_new_subset_should_be_an_empty_set ()
  {
    unsigned int i;
    ElementSet set ("S1",3,2);
    ElementSubset subset ("X1", &set);
    for (i = 0; i < subset.get_set_cardinality (); i++)
      if (subset.has_element (i))
        return false;
    return true;
  }


  bool an_element_not_in_subset_should_be_added ()
  {
    ElementSet set ("S1",3,2);
    ElementSubset subset ("X1", &set);
    if ((subset.add_element (2)) &&
        (subset.has_element (2)))
      return true;
    else
      return false;
  }


  bool an_element_in_subset_should_be_removed ()
  {
    ElementSet set ("S1",3,2);
    ElementSubset subset ("X1", &set);
    if ((subset.add_element (1)) &&
        (subset.has_element (1)) &&
        (subset.remove_element (1)) &&
	(!subset.has_element (1)))
      return true;
    else
      return false;
  }


  bool it_should_give_the_set_that_belongs_the_subset ()
  {
    ElementSet set ("S1",3,2);
    ElementSubset subset ("X1", &set);
    if (subset.get_set_that_contains_this_subset () == &set)
      return true;
    else
      return false;
  }


  bool a_set_should_contains_its_subset ()
  {
    ElementSet set ("S1",3,2);
    ElementSubset X ("X", &set);
    ElementSubset Y ("Y", &set);
    X.set_complete_subset ();
    Y.add_element (1);
    return X.contains( &Y);
  }


  bool a_subset_should_be_contained_by_its_set ()
  {
    ElementSet set ("S1",3,2);
    ElementSubset X ("X", &set);
    ElementSubset Y ("Y", &set);
    X.set_complete_subset ();
    Y.add_element (1);
    return Y.is_contained_by (&X);
  }


  bool a_subset_should_be_successfully_cloned ()
  {
    ElementSet set ("S1",3,2);
    ElementSubset X ("X", &set);
    ElementSubset Y ("Y", &set);
    X.add_element (0);
    X.add_element (2);
    Y.copy (&X);
    if ((X.contains (&Y)) &&
        (Y.contains(&X)))
      return true;
    else
      return false;
  }


  bool a_random_element_should_be_removed ()
  {
    unsigned int i;
    ElementSet set ("S1",3,2);
    ElementSubset X ("X", &set);
    X.set_complete_subset ();

    i = X.remove_random_element ();
    if (!(((!X.has_element (0)) &&
           (X.has_element (1)) &&
           (X.has_element (2)) &&
           (i == 0)) ||
          ((X.has_element (0)) &&
           (!X.has_element (1)) &&
           (X.has_element (2)) &&
           (i == 1) ) ||
          ((X.has_element (0)) &&
           (X.has_element (1)) &&
           (!X.has_element (2)) &&
           (i == 2))))
      return false;

    X.remove_random_element ();
    if (X.is_empty ())
      return false;

    X.remove_random_element ();
    if (!X.is_empty ())
      return false;

    if ((X.is_empty ()) &&
        (X.remove_random_element () == set.get_set_cardinality ()))
      return true;
    else
      return false;
  }


  bool it_should_give_the_complement_of_the_set ()
  {
    ElementSet set ("S1",3,2);
    ElementSubset subset ("X1", &set);
    if (!subset.add_element (1))
      return false;

    subset.set_complement_subset ();
    if ((subset.has_element (0)) &&
        (!subset.has_element (1)) &&
        (subset.has_element (2)))
      return true;
    else
      return false;
  }

} // end of namespace

