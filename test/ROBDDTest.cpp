//
// ROBDDTest.cpp -- implementation of the namespace "ROBDDTest".
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


#include "ROBDDTest.h"

namespace ROBDDTest 
{

  bool a_new_robdd_has_only_a_terminal_node ()
  {
    ElementSet * elm_set = new ElementSet ("", 3, 100);
    ROBDD * robdd = new ROBDD (elm_set);
    Vertex * root = robdd->get_root ();
    bool answ = true;
    answ = root->get_var () == NULL && root->get_child (true) == NULL && root->get_child (false) == NULL;
    delete elm_set;
    delete robdd;
    return answ;
  }


  bool the_union_of_a_new_robdd_with_itself_should_be_itself ()
  {
    ElementSet * elm_set = new ElementSet ("", 3, 100);
    ROBDD * robdd = new ROBDD (elm_set);
    Vertex * root = robdd->get_root ();
    robdd->union_to (root);
    root = robdd->get_root ();
    bool answ = (root->get_child (true) == NULL) && (root->get_child (false) == NULL) \
    && root->get_value () == 0;
    delete elm_set;
    delete robdd;
    return answ;
  }


  bool the_union_of_all_subsets_must_be_the_function_1 ()
  {
    ElementSet * elm_set = new ElementSet ("", 3, 3);
    ElementSubset * subset = new ElementSubset ("", elm_set);
    subset->add_element (0);
    subset->add_element (1);
    subset->add_element (2);
    ROBDD * robdd = new ROBDD (elm_set);
    robdd->add_interval (subset, false);
    Vertex * root = robdd->get_root ();
    bool answ = (root->get_child (true) == NULL) && (root->get_child (false) == NULL) \
    && root->get_value () == 1;
    delete robdd;
    delete elm_set;
    delete subset;
    return answ;
  }


  bool a_new_robdd_contais_no_subset ()
  {
    ElementSet * elm_set = new ElementSet ("", 3, 100);
    ROBDD * robdd = new ROBDD (elm_set);
    ElementSubset * subset1 = new ElementSubset ("", elm_set);
    subset1->add_element (0);
    ElementSubset * subset2 = new ElementSubset ("", elm_set);
    subset2->add_element (1);
    ElementSubset * subset3 = new ElementSubset ("", elm_set);
    subset3->add_element (2);
    bool answ = !robdd->contains (subset1) && !robdd->contains (subset2) && \
    !robdd->contains (subset3);
    delete elm_set;
    delete robdd;
    delete subset1;
    delete subset2;
    delete subset3;
    return answ;
  }


  bool an_added_subset_must_be_covered ()
  {
    ElementSet * elm_set = new ElementSet ("", 3, 3);
    ElementSubset * subset = new ElementSubset ("", elm_set);
    subset->add_element (0);
    subset->add_element (1);
    subset->add_element (2);
    ROBDD * robdd = new ROBDD (elm_set, subset);
    bool answ = robdd->contains (subset);
    delete robdd;
    delete subset;
    delete elm_set;
    return answ;
  }


  bool the_function_one_covers_all_subsets ()
  {
    ElementSet elm_set ("", 2, 2);  
    ElementSubset subset ("", &elm_set);
    subset.add_element (0);
    subset.add_element (1);
    ROBDD robdd (&elm_set);
    robdd.add_interval (&subset, false);
    ElementSubset sub0 ("", &elm_set);
    ElementSubset sub1 ("", &elm_set);
    sub1.add_element (0);
    ElementSubset sub2 ("", &elm_set);
    sub2.add_element (1);
    ElementSubset sub3 ("", &elm_set);
    sub3.add_element (0);
    sub3.add_element (1);
    bool answ = robdd.contains (&sub0) && robdd.contains (&sub1) && \
    robdd.contains (&sub2) && robdd.contains (&sub3);
    return answ;
  }


  bool the_function_one_have_no_subset_evaluated_zero ()
  {
    ElementSet elm_set ("", 2, 2);  
    ElementSubset subset ("", &elm_set);
    subset.add_element (0);
    subset.add_element (1);
    ROBDD robdd (&elm_set);
    robdd.add_interval (&subset, false);
    ElementSubset * elm = robdd.get_random_zero_evaluated_element ();
    return elm == NULL;
  }


  bool its_possible_to_get_a_random_subset_evaluated_zero ()
  {
    ElementSet elm_set ("", 2, 2);  
    ROBDD robdd (&elm_set);
    ElementSubset subset1 ("", &elm_set);
    ElementSubset subset2 ("", &elm_set);
    subset1.add_element (0);
    subset2.add_element (1);
    robdd.add_interval (&subset1, true);
    robdd.add_interval (&subset2, true);
    ElementSubset expected_subset ("", &elm_set);
    ElementSubset * answ_subset = robdd.get_random_zero_evaluated_element ();
    bool answ = answ_subset->is_equal(&expected_subset);
    delete answ_subset;
    return answ;
  }


  bool it_should_be_able_to_reduce_an_obdd () 
  {
    ElementSet * elm_set = new ElementSet ("", 7, 100);
    Vertex * v1 = new Vertex (elm_set->get_element (0), 1);
    Vertex * v2 = new Vertex (elm_set->get_element (1), 2);
    Vertex * v3 = new Vertex (elm_set->get_element (2), 3);
    Vertex * v4a = new Vertex (elm_set->get_element (3), 4);
    Vertex * v4b = new Vertex (elm_set->get_element (3), 4);
    Vertex * v5a = new Vertex (elm_set->get_element (4), 5);
    Vertex * v5b = new Vertex (elm_set->get_element (4), 5);
    Vertex * v5c = new Vertex (elm_set->get_element (4), 5);
    Vertex * v6a = new Vertex (elm_set->get_element (5), 6);
    Vertex * v6b = new Vertex (elm_set->get_element (5), 6);
    Vertex * v7 = new Vertex (elm_set->get_element (6), 7);
    Vertex * one = new Vertex (true, 8);
    Vertex * zero = new Vertex (false, 8);
    v1->set_child (one, true);
    v1->set_child (v2, false);
    v2->set_child (one, true);
    v2->set_child (v3, false);
    v3->set_child (v4b, true);
    v3->set_child (v4a, false);
    v4a->set_child (v6b, true);
    v4a->set_child (v5a, false);
    v4b->set_child (v5c, true);
    v4b->set_child (v5b, false);
    v5a->set_child (v6b, true);
    v5a->set_child (one, false);
    v5b->set_child (v6b, true);
    v5b->set_child (v6a, false);
    v5c->set_child (one, true);
    v5c->set_child (v6b, false);
    v6a->set_child (one, true);
    v6a->set_child (one, false);
    v6b->set_child (one, true);
    v6b->set_child (v7, false);
    v7->set_child (zero, true);
    v7->set_child (one, false);

    ROBDD * R = new ROBDD (elm_set, v1, 13);
    R->reduce ();
    delete R;
    delete elm_set;
    return true;
  }


  bool it_should_be_able_to_add_a_subset () 
  {
    ElementSet * elm_set = new ElementSet ("", 3, 3);
    ElementSubset * subset = new ElementSubset ("", elm_set);
    subset->add_element (0);
    subset->add_element (1);
    subset->add_element (2);
    ROBDD * robdd = new ROBDD (elm_set);
    robdd->add_subset (subset);
    bool answ = robdd->contains (subset);
    delete robdd;
    delete subset;
    delete elm_set;
    return answ;
  }


} // end of namespace

