//
// UCurveToolBoxTest.cpp -- implementation of the namespace "UCurveToolBoxTest".
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

#include "UCurveToolBoxTest.h"

namespace UCurveToolBoxTest
{

  bool it_should_update_lower_restriction_set_with_an_outer_subset ()
  {
    Collection ucc;
    ElementSet a_set ("S1",3,100);
    ElementSubset X ("X", &a_set); // X = [000]
    ElementSubset Y ("Y", &a_set);
    ElementSubset Z ("Z", &a_set);
    Y.add_element(1);  // Y = [011]
    Y.add_element(2);
    Z.add_element(1);  // Z = [010]
    UCurveToolBox::update_lower_restriction(&ucc, &X);  // should add X
    UCurveToolBox::update_lower_restriction(&ucc, &Y);  // should add Y and remove X
    UCurveToolBox::update_lower_restriction(&ucc, &Z);  // should do nothing
    if ((!ucc.has_subset(&X)) &&
      ( ucc.has_subset(&Y)) &&
      (!ucc.has_subset(&Z)) )
      return true;
    else
      return false;
  }


  bool it_should_update_upper_restriction_set_with_an_outer_subset ()
  {
    Collection ucc;
    ElementSet a_set ("S1",3,100);
    ElementSubset X = ElementSubset("X", &a_set);
    ElementSubset Y = ElementSubset("Y", &a_set);
    ElementSubset Z = ElementSubset("Z", &a_set);
    X.set_complete_subset();  // X = [111]
    Y.add_element(0);  // Y = [100]
    Z.add_element(0);
    Z.add_element(2);  // Z = [101]
    UCurveToolBox::update_upper_restriction(&ucc, &X);  // should add X
    UCurveToolBox::update_upper_restriction(&ucc, &Y);  // should add Y and remove X
    UCurveToolBox::update_upper_restriction(&ucc, &Z);  // should do nothing
    if ((!ucc.has_subset(&X)) &&
      ( ucc.has_subset(&Y)) &&
      (!ucc.has_subset(&Z)) )
      return true;
    else
      return false;
  }


  bool it_should_not_update_lower_restriction_set_with_a_covered_subset ()
  {
    ElementSet a_set ("S1",3,100);
    Collection ucc;
    ElementSubset X ("X", &a_set); // X = [000]
    ElementSubset Y ("Y", &a_set);
    ElementSubset Z ("Z", &a_set);
    ElementSubset O ("O", &a_set); // O = [000]
    ElementSubset P ("P", &a_set);
    ElementSubset Q ("Q", &a_set);
    Y.set_complete_subset();
    Y.remove_element(0);              // Y = [011]
    Z.add_element(1);                 // Z = [010]
    P.add_element(2);                 // P = [001]
    Q.set_complete_subset();          // Q = [111]
    UCurveToolBox::update_lower_restriction(&ucc, &X);   // should add X
    UCurveToolBox::update_lower_restriction(&ucc, &Z);   // should add Z and remove X
    UCurveToolBox::update_lower_restriction(&ucc, &P);   // should add P
    if( (!(!ucc.has_subset(&O)) &&
      ( ucc.has_subset(&Z)) &&
      ( ucc.has_subset(&P))) )
      return false;
    UCurveToolBox::update_lower_restriction(&ucc, &Q);  // should add Q and remove P and Z
    UCurveToolBox::update_lower_restriction(&ucc, &Y);  // should do nothing
    if ((!ucc.has_subset(&O)) &&
      ( ucc.has_subset(&Q)) &&
      (!ucc.has_subset(&Y)) &&
      (!ucc.has_subset(&Z)) &&
      (!ucc.has_subset(&P)) )
      return true;
    else
      return false;
  }


  bool it_should_not_update_upper_restriction_set_with_a_covered_subset ()
  {
    ElementSet a_set ("S1",3,100);
    Collection ucc;
    ElementSubset X ("X", &a_set);
    ElementSubset Y ("Y", &a_set);
    ElementSubset Z ("Z", &a_set);
    ElementSubset O ("O", &a_set);
    ElementSubset P ("P", &a_set);
    ElementSubset Q ("Q", &a_set); // Q = [000]
    O.set_complete_subset();
    X.set_complete_subset();          // X = O = [111]
    Y.add_element(0);                 // Y = [100]
    Z.add_element(0);          // Z = [101]
    Z.add_element(2);
    P.add_element(0);                 // P = [110]
    P.add_element(1);
    UCurveToolBox::update_upper_restriction(&ucc, &X);   // should add X
    UCurveToolBox::update_upper_restriction(&ucc, &Z);   // should add Z and remove X
    UCurveToolBox::update_upper_restriction(&ucc, &P);   // should add P
    if( (!(!ucc.has_subset(&O)) &&
        ( ucc.has_subset(&Z)) &&
        ( ucc.has_subset(&P)) ) )
      return false;
    UCurveToolBox::update_upper_restriction(&ucc, &Q);  // should add Q and remove P and Z
    UCurveToolBox::update_upper_restriction(&ucc, &Y);  // should do nothing
    if ((!ucc.has_subset(&O)) &&
      ( ucc.has_subset(&Q)) &&
      (!ucc.has_subset(&Y)) &&
      (!ucc.has_subset(&Z)) &&
      (!ucc.has_subset(&P)) )
      return true;
    else
      return false;
  }

  bool it_should_get_a_maximal_subset ()
  {
    ElementSet a_set ("S1",3,100);
    Collection RU1;
    Collection RU2;
    Collection RL;  // empty lower restriction
    unsigned int i;
    ElementSubset * A;
    ElementSubset X ("X", &a_set);
    ElementSubset Y ("Y", &a_set); // Y = [000]
    X.add_element (1);
    X.add_element (2);              // X = [011]

    UCurveToolBox::update_upper_restriction (&RU1, &Y);
    // there is no maximal subset
    A = UCurveToolBox::get_maximal_subset (&RL, &RU1, &a_set);
    if (A != NULL)
    {
      delete A;
      return false;
    }

    UCurveToolBox::update_upper_restriction (&RU2, &X);
    for (i = 0; i < 100; i++)  // repeating the test n times, once the procedure is stochastic
    {
      // therefore the maximal subsets here are [101] and [110]
      A = UCurveToolBox::get_maximal_subset (&RL, &RU2, &a_set);
      if (A == NULL)
        return false;
      else if (( A->has_element (0) ) &&
           (!A->has_element (1) ) &&
           ( A->has_element (2) ) )
        delete A;
      else if (( A->has_element (0) ) &&
           ( A->has_element (1) ) &&
           (!A->has_element (2) ) )
        delete A;
      else
      {
        delete A;
        return false;
      }
    }
    return true;
  }


  bool it_should_select_an_unsivited_adjacent ()
  {
    ElementSet three_set ("", 3, 100);
    map<string, UCurveToolBox::Node *>  Graph;
    Collection RL, RU;
    ElementSubset X ("", & three_set);
    UCurveToolBox::Node * A, * B, * C, * Y;
    unsigned int i;

    X.set_complete_subset (); // X = 111
    A = UCurveToolBox::create_node (& X);
    Graph.insert (pair<string, UCurveToolBox::Node *> (A->vertex->print_subset (), A));

    Y = UCurveToolBox::select_an_unvisited_adjacent (&Graph, &RL, &RU, A, &i);

    if (Y == NULL)
    {
      UCurveToolBox::delete_node (A);
      return false;
    }

    // verifies if it is returned a valid unvisited adjacent
    if ((Y->vertex->print_subset ().find ("110") == string::npos) &&
      (Y->vertex->print_subset ().find ("101") == string::npos) &&
      (Y->vertex->print_subset ().find ("011") == string::npos))
    {
      UCurveToolBox::delete_node (A);
      UCurveToolBox::delete_node (Y);
      return false;
    }
    UCurveToolBox::delete_node (Y);
    A->unverified->set_complete_subset ();  // "cleans" the node A

    //==========

    // vertices: A, B; edges: AB
    //
    X.remove_element (2); // X = 110
    B = UCurveToolBox::create_node (&X);
        Graph.insert (pair<string, UCurveToolBox::Node *> (B->vertex->print_subset (), B));

    Y = UCurveToolBox::select_an_unvisited_adjacent (&Graph, &RL, &RU, A, &i);

    if (Y == NULL)
    {
      UCurveToolBox::delete_node (A);
      UCurveToolBox::delete_node (B);
      return false;
    }

    // verifies if it is returned a valid unvisited adjacent
    if ((Y->vertex->print_subset ().find ("101") == string::npos) &&
      (Y->vertex->print_subset ().find ("011") == string::npos))
    {
      UCurveToolBox::delete_node (Y);
      UCurveToolBox::delete_node (A);
      UCurveToolBox::delete_node (B);
      return false;
    }
    UCurveToolBox::delete_node (Y);
    A->unverified->set_complete_subset ();  // "cleans" the node A
    B->unverified->set_complete_subset ();  // "cleans" the node B

    //==========

    // now we remove [\emptyset, 101] from the search space,
    // thus A should have just 011 as an unvisited adjacent element
    //
    X.set_complete_subset ();  // X = 101
    X.remove_element (1);
    UCurveToolBox::update_lower_restriction (&RL, &X);

    Y = UCurveToolBox::select_an_unvisited_adjacent (&Graph, &RL, &RU, A, &i);

    if (Y == NULL)
    {
      UCurveToolBox::delete_node (A);
      UCurveToolBox::delete_node (B);
      return false;
    }
    // verifies if it is returned a valid unvisited adjacent
    if (Y->vertex->print_subset ().find ("011") == string::npos)
    {
      UCurveToolBox::delete_node (Y);
      UCurveToolBox::delete_node (A);
      UCurveToolBox::delete_node (B);
      return false;
    }
    UCurveToolBox::delete_node (Y);
    A->unverified->set_complete_subset ();  // "cleans" the node A
    B->unverified->set_complete_subset ();  // "cleans" the node B

    //==========

    // vertices: A, B, C; edges: AB, AC
    // RL = 101
    // A should have no unvisited adjacent element
    //
    X.set_complete_subset (); // X = 011
    X.remove_element (0);
    C = UCurveToolBox::create_node (&X);
        Graph.insert (pair<string, UCurveToolBox::Node *> (C->vertex->print_subset (), C));

    Y = UCurveToolBox::select_an_unvisited_adjacent (&Graph, &RL, &RU, A, &i);

    if (Y != NULL)
    {
      UCurveToolBox::delete_node (Y);
      UCurveToolBox::delete_node (A);
      UCurveToolBox::delete_node (B);
      UCurveToolBox::delete_node (C);
      return false;
    }
    A->unverified->set_complete_subset ();  // "cleans" the node A
    B->unverified->set_complete_subset ();  // "cleans" the node B
    C->unverified->set_complete_subset ();  // "cleans" the node C

    //==========

    // C should have just 010 as an unvisited adjacent element
    //
    Y = UCurveToolBox::select_an_unvisited_adjacent (&Graph, &RL, &RU, C, &i);

    if (Y == NULL)
    {
      UCurveToolBox::delete_node (A);
      UCurveToolBox::delete_node (B);
      UCurveToolBox::delete_node (C);
      return false;
    }
    // verifies if it is returned a valid unvisited adjacent
    if (Y->vertex->print_subset ().find ("010") == string::npos)
    {
      UCurveToolBox::delete_node (Y);
      UCurveToolBox::delete_node (A);
      UCurveToolBox::delete_node (B);
      UCurveToolBox::delete_node (C);
      return false;
    }
    UCurveToolBox::delete_node (Y);
    A->unverified->set_complete_subset ();  // "cleans" the node A
    B->unverified->set_complete_subset ();  // "cleans" the node B
    C->unverified->set_complete_subset ();  // "cleans" the node C

    //==========

    // vertices: A, B, C; edges: AB, AC
    // RL = 101,  RU = 010
    // A should have no unvisited adjacent element
    //
    X.set_empty_subset (); // X = 010;
    X.add_element (1);
    UCurveToolBox::update_upper_restriction (&RU, &X);

    Y = UCurveToolBox::select_an_unvisited_adjacent (&Graph, &RL, &RU, A, &i);

    if (Y != NULL)
    {
      UCurveToolBox::delete_node (Y);
      UCurveToolBox::delete_node (A);
      UCurveToolBox::delete_node (B);
      UCurveToolBox::delete_node (C);
      return false;
    }

    UCurveToolBox::delete_node (A);
    UCurveToolBox::delete_node (B);
    UCurveToolBox::delete_node (C);
    return true;
  }

} // end of namespace