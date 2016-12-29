//
// CollectionTest.cpp -- implementation of the namespace "CollectionTest".
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

#include "CollectionTest.h"

namespace CollectionTest
{

  bool a_lower_covered_subset_should_be_found ()
  {
    ElementSet S ("", 3, 100);
    ElementSubset X ("X", &S);
    ElementSubset Y ("Y", &S);
    ElementSubset Z ("Z", &S);
    Collection L;
    X.add_element (0);  // X = [101]
    X.add_element (2);
    Y.add_element (2);  // Y = [001]
    Z.add_element (1);  // Z = [010]
    L.add_subset (&X);
    L.add_subset (&Z);
    if (L.lower_covers (&Y))
      return true;
    else
      return false;
    }


  bool an_upper_covered_subset_should_be_found ()
  {
    ElementSet S ("", 3, 100);
    ElementSubset X ("X", &S);
    ElementSubset Y ("Y", &S);
    ElementSubset Z ("Z", &S);
    Collection L;
    X.add_element (1);  // X = [010]
    Y.add_element (0);  // Y = [110]
    Y.add_element (1);
    Z.add_element (0);  // Z = [101]
    Z.add_element (2);
    L.add_subset (&X);
      L.add_subset (&Z);
    if (L.upper_covers (&Y))
      return true;
    else
      return false;
  }


  bool a_non_lower_covered_subset_should_not_be_found ()
  {
    ElementSet S ("", 3, 100);
    ElementSubset X ("X", &S);
    ElementSubset Y ("Y", &S);
    ElementSubset Z ("Z", &S);
    Collection L;
    X.add_element (0);  // X = [101]
    X.add_element (2);
    Y.add_element (2);  // Y = [001]
    Z.add_element (1);  // Z = [010]
    L.add_subset (&Y);
    L.add_subset (&Z);
    if (! L.lower_covers (&X))
      return true;
    else
      return false;
  }


  bool a_non_upper_covered_subset_should_not_be_found ()
  {
    ElementSet S ("", 3, 100);
    ElementSubset X ("X", &S);
    ElementSubset Y ("Y", &S);
    ElementSubset Z ("Z", &S);
    Collection L;
    X.add_element (1);  // X = [010]
    Y.add_element (0);  // Y = [110]
    Y.add_element (1);
    Z.add_element (0);  // Z = [101]
    Z.add_element (2);
    L.add_subset (&Y);
    L.add_subset (&Z);
    if (!L.upper_covers (&X))
      return true;
    else
      return false;
  }


  bool it_should_remove_lower_covered_subsets_in_a_collection ()
  {
    ElementSet S ("", 3, 100);
    ElementSubset X ("X", &S);
    ElementSubset Y ("Y", &S);
    ElementSubset Z ("Z", &S);
    ElementSubset O ("O", &S);  // O = [000]
    ElementSubset P ("P", &S);
    ElementSubset Q ("Q", &S);
    ElementSubset R ("R", &S);
    Collection L;
    X.add_element (0);           // X = [101]
    X.add_element (2);
    Y.set_complete_subset ();    // Y = [111]
    Z.add_element (1);           // Z = [010]
    P.add_element (0);           // P = [100]
    Q.copy (&X);                 // Q = [101] = X
    R.add_element (2);           // R = [001]
    L.add_subset (&R);
    L.add_subset (&Y);
    L.add_subset (&Z);
    L.add_subset (&O);
    L.add_subset (&P);
    L.add_subset (&Q);
    // should remove O,P,Q,R and keep Y,Z
    if ((L.remove_lower_covered_subsets (&X) == 4)  &&
        ( L.has_subset (&Y)) &&
        (! L.has_subset (&X)) &&
        ( L.has_subset (&Z)) &&
        (! L.has_subset (&O)) &&
        (! L.has_subset (&R)) )
      return true;
    else
      return false;
  }


  bool it_should_remove_upper_covered_subsets_in_a_collection ()
  {
    ElementSet S ("", 3, 100);
    ElementSubset X ("X", &S);
    ElementSubset Y ("Y", &S);  // Y = [000]
    ElementSubset Z ("Z", &S);
    ElementSubset O ("O", &S);
    ElementSubset P ("P", &S);
    ElementSubset Q ("Q", &S);
    ElementSubset R ("R", &S);
    Collection L;
    O.set_complete_subset();  // O = [111]
    X.add_element (1);         // X = [010]
    Z.add_element (0);         // Z = [101]
    Z.add_element (2);
    P.add_element (1);         // P = [011]
    P.add_element (2);
    Q.copy (&X);               // Q = [010] = X
    R.add_element (0);         // R = [110]
    R.add_element (1);
    L.add_subset (&R);
    L.add_subset (&Y);
    L.add_subset (&Z);
    L.add_subset (&O);
    L.add_subset (&P);
    L.add_subset (&Q);
    // should remove O,P,Q,R and keep Y,Z
    if ((L.remove_upper_covered_subsets (&X) == 4)  &&
        (L.has_subset (&Y)) &&
        (! L.has_subset (&X)) &&
        (L.has_subset (&Z)) &&
        (! L.has_subset (&O)) &&
        (! L.has_subset (&R)) )
      return true;
    else
      return false;
  }


  bool the_evaluated_subset_should_not_be_deleted ()
  {
    ElementSet S ("", 3, 100);
    ElementSubset X ("X", &S);
    Collection L;
    L.add_subset (&X);
    L.remove_lower_covered_subsets (&X);
    if ( (!L.lower_covers (&X)) &&
         (X.get_set_that_contains_this_subset () == &S))
      return true;
    else
      return false;
  }


  bool it_should_copy_a_collection ()
  {
    ElementSet S ("", 3, 100);
    ElementSubset X ("X", &S);
    ElementSubset Y ("Y", &S);
    ElementSubset Z ("Z", &S);
    ElementSubset O ("O", &S);   // O = [000]
    ElementSubset P ("P", &S);
    Collection L1;
    Collection L2;
    X.add_element (0);           // X = [101]
    X.add_element (2);
    Y.set_complete_subset ();    // Y = [111]
    Z.add_element (1);           // Z = [010]
    P.add_element (0);           // P = [100]
    L1.add_subset (&X);
    L1.add_subset (&Y);
    L1.add_subset (&Z);
    L2.copy (&L1);
    if ((L2.has_subset (&X)) &&
        (L2.has_subset (&Y)) &&
        (L2.has_subset (&Z)) &&
        (! L2.has_subset (&O)) &&
        (! L2.has_subset (&P)) )
      return true;
    else
      return false;
  }

} // end of namespace

