//
// BFSTest.cpp -- implementation of the namespace "BFSTest".
//
//    This file is part of the featsel program
//    Copyright (C) 2017 Marcelo S. Reis
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

#include "BFSTest.h"

namespace BFSTest
{

  bool it_should_store_all_the_visited_subsets ()
  {
    // This instance is from "Arrhythmia" data set from UCI Machine Learning
    // repository (279 features, 16 labels).
    //
    ElementSet arrhythmia (16, "input/Arrhythmia/Test_01_A.dat", 279);

    BFS t;
    CFS c (& arrhythmia);

    t.set_parameters (&c, &arrhythmia, true);
    t.get_minima_list (1);

    // Same number of visited subsets as Weka's BFS/CFS with:
    // * CFS's locally predictive == false;
    // * argument -N 5;
    // * argument -D 1 (direction == forward); 
    // * lookupCacheSize == 80.
    //
    if (t.get_list_of_visited_subsets ()->size () == 14971)
      return true;
    else
      return false;
  }


  bool it_should_give_the_number_of_the_visited_subsets ()
  {
    // This instance is from "Arrhythmia" data set from UCI Machine Learning
    // repository (279 features, 16 labels).
    //
    ElementSet arrhythmia (16, "input/Arrhythmia/Test_01_A.dat", 279);

    BFS t;
    CFS c (& arrhythmia);

    t.set_parameters (&c, &arrhythmia, false);
    t.get_minima_list (1);

    // Same number of visited subsets as Weka's BFS/CFS with:
    // * CFS's locally predictive == false;
    // * argument -N 5;
    // * argument -D 1 (direction == forward);
    // * lookupCacheSize == 80.
    //
    if (t.print_number_of_visited_subsets () == 14971)
      return true;
    else
      return false;
  }


} // end of namespace
