//
// ROBDDTest.h -- definition of the namespace "ROBDDTest".
//
//    This file is part of the featsel program
//    Copyright (C) 2016  Marcelo S. Reis, Gustavo Estrela
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

#ifndef ROBDDTEST_H_
#define ROBDDTEST_H_

#include "../src/ROBDD.h"
#include "../src/Vertex.h"

namespace ROBDDTest 
{

  bool a_new_robdd_has_only_a_terminal_node ();

  bool the_union_of_a_new_robdd_with_itself_should_be_itself ();

  bool the_union_of_all_subsets_must_be_the_function_1 ();

  bool a_new_robdd_contais_no_subset ();

  bool an_added_subset_must_be_covered ();

  bool the_function_one_covers_all_subsets ();

  bool the_function_one_have_no_subset_evaluated_zero ();

  bool its_possible_to_get_a_random_subset_evaluated_zero ();
  
  bool it_should_be_able_to_reduce_an_obdd ();

  bool it_should_be_able_to_add_a_subset ();

}

#endif /* ROBDDTEST_H_ */
