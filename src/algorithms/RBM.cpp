//
// RBM.cpp -- implementation of the class "RBM".
//
//    This file is part of the featsel program
//    Copyright (C) 2018  Marcelo S. Reis
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

#include "RBM.h"


RBM::RBM ()
{
  lower_restriction = new Collection ();
  upper_restriction = new Collection ();
  list_of_visited_subsets = new Collection ();
  cost_function = NULL;
}


RBM::~RBM ()
{
  if (lower_restriction != NULL)
    delete lower_restriction;
  if (upper_restriction != NULL)
    delete upper_restriction;
  if (list_of_visited_subsets != NULL)
    delete list_of_visited_subsets;
}



bool RBM::down_up_direction (Collection   * L,
                             Collection   * lower_restriction,
                             Collection   * upper_restriction,
                             CostFunction * cost_function,
                             ElementSet   * set)
{

  ElementSubset * A, * B, * M = NULL;

  B = UCurveToolBox::get_minimal_subset (lower_restriction,
                                         upper_restriction, set);
  if (B == NULL)
    return true;
    
  if (upper_restriction->upper_covers (B))
  {
    UCurveToolBox::update_lower_restriction (lower_restriction, B);
    delete B;
    B = UCurveToolBox::get_maximal_subset (lower_restriction,
                                           upper_restriction, set);
    if (B == NULL)   
      return true;
    else
    {
      delete B;
      return false;
    }
  }

  B->cost = cost_function->cost (B);

  do
  {
    A = M;
    M = B;
    L->add_subset (B);
    B = select_upper_adjacent (NULL, lower_restriction, upper_restriction, M);
    
    if (B != NULL)
      B->cost = cost_function->cost (B);
  }
  while ((B != NULL) && (B->cost <= M->cost));

  if (A != NULL)
  {
    UCurveToolBox::update_lower_restriction (lower_restriction, A);
    delete A; 
  }

  if (B != NULL)
  {
    UCurveToolBox::update_upper_restriction (upper_restriction, B);
    delete B;
  }
          
  L->add_subset (M);
  RBM::minima_exhausting (L, lower_restriction, 
                          upper_restriction, M, cost_function);
  delete M;
  return false;
}



bool RBM::up_down_direction (Collection   * L,
                             Collection   * lower_restriction,
                             Collection   * upper_restriction,
                             CostFunction * cost_function,
                             ElementSet   * set)
{

  ElementSubset * A, * B, * M = NULL;

  B = UCurveToolBox::get_maximal_subset (lower_restriction,
                                         upper_restriction, set);
  if (B == NULL)
    return true;
    
  if (lower_restriction->lower_covers (B))
  {
    UCurveToolBox::update_upper_restriction (upper_restriction, B);
    delete B;
    B = UCurveToolBox::get_minimal_subset (lower_restriction,
                                           upper_restriction, set);
    if (B == NULL)   
      return true;
    else
    {
      delete B;
      return false;
    }
  }

  B->cost = cost_function->cost (B);

  do
  {
    A = M;
    M = B;
    L->add_subset (B);
    B = select_lower_adjacent (NULL, lower_restriction, upper_restriction, M);
    if (B != NULL)
      B->cost = cost_function->cost (B);
  }
  while ((B != NULL) && (B->cost <= M->cost));

  if (A != NULL)
  {
    UCurveToolBox::update_upper_restriction (upper_restriction, A);
    delete A; 
  }

  if (B != NULL)
  {
    UCurveToolBox::update_lower_restriction (lower_restriction, B);
    delete B;
  }

  L->add_subset (M);
  RBM::minima_exhausting (L, lower_restriction, 
                          upper_restriction, M, cost_function);
  delete M;
  return false;
}



void RBM::get_minima_list (unsigned int max_size_of_minima_list)
{
  timeval begin_program, end_program;
  gettimeofday (& begin_program, NULL);

  //
  unsigned int direction;
  Collection * L = new Collection ();
  bool search_space_is_empty = false;
  ElementSubset * X, Y ("", set);

  do
  {
    // at each iteration it is called either minimal_element or maximal_element
    direction = rand () % 2;
    if (direction == 0)
    {
      search_space_is_empty = down_up_direction (L, lower_restriction,
                                                 upper_restriction,
                                                 cost_function, set);
    }    
    else
    {
      search_space_is_empty = up_down_direction (L, lower_restriction,
                                                 upper_restriction,
                                                 cost_function, set);    
    }
    
    while (L->size() > 0)
    {
      X = L->remove_last_subset ();
      list_of_minima.push_back (X);
      if (store_visited_subsets)
        list_of_visited_subsets->add_subset (X);
    }

    clean_list_of_minima (max_size_of_minima_list);

  }
  while ((! search_space_is_empty) && 
         (! cost_function->has_reached_threshold ()));

  delete L;
  //

  number_of_visited_subsets =
  cost_function->get_number_of_calls_of_cost_function ();
  clean_list_of_minima (max_size_of_minima_list);

  gettimeofday (& end_program, NULL);
  elapsed_time_of_the_algorithm = diff_us (end_program, begin_program);
}



bool is_subset_in_stack (list <ElementSubset *> * L, ElementSubset * X)
{
  list <ElementSubset *>::iterator it;
  
  if (L == NULL)
    return false;
  
  for (it = L->begin (); it != L->end (); it++)
    if (X->is_equal (*it))
      return true;

  return false;
}


ElementSubset * RBM::select_lower_adjacent (list <ElementSubset *> * L,
                           Collection * RL, Collection * RU, ElementSubset * A)
{
  unsigned int i;
  ElementSubset B ("temp", A->get_set_that_contains_this_subset ()), * C;
  if (RL == NULL)
  {
    cout << "select_lower_adjacent: error: RL should not be NULL." << endl;
    return NULL;
  }
  
  C = new ElementSubset ("lower_adj", A->get_set_that_contains_this_subset ());
  C->copy (A);

  B.copy (A);
  
  while (! B.is_empty ())
  {
    i = B.remove_random_element ();
    C->remove_element (i);
    if ( (! is_subset_in_stack (L, C)) && 
         (! RL->lower_covers (C)) && 
         (! RU->upper_covers (C)))
      return C;
    else
      C->add_element (i);
  }

  delete C;
  return NULL;
}



ElementSubset * RBM::select_upper_adjacent (list <ElementSubset *> * L,
                           Collection * RL, Collection * RU, ElementSubset * A)
{
  unsigned int i;
  ElementSubset B ("temp", A->get_set_that_contains_this_subset ()), * C;

  if (RU == NULL)
  {
    cout << "select_upper_adjacent: error: RU should not be NULL." << endl;
    return NULL;
  }
  
  C = new ElementSubset ("upper_adj", A->get_set_that_contains_this_subset ());
  C->copy (A);

  B.copy (A);
  B.set_complement_subset ();

  while (! B.is_empty ())
  {
    i = B.remove_random_element ();
    C->add_element (i);
    
    if ( (! is_subset_in_stack (L, C)) && 
         (! RL->lower_covers (C)) && 
         (! RU->upper_covers (C)))
      return C;
    else
      C->remove_element (i);
  }

  delete C;
  return NULL;
}


ElementSubset * RBM::select_adjacent (list <ElementSubset *> * L,
                           Collection * RL, Collection * RU, ElementSubset * M)
{
  unsigned int direction;
	ElementSubset * A;

	direction = rand () % 2;

	if (direction == 0)
	{
    A = RBM::select_lower_adjacent (L, RL, RU, M);

		if (A == NULL)
		  return RBM::select_upper_adjacent (L, RL, RU, M);
		else
			return A;
	}
	else
	{
		A = RBM::select_upper_adjacent (L, RL, RU, M);

		if (A == NULL)
			return RBM::select_lower_adjacent (L, RL, RU, M);
		else
			return A;
	}
}


// Minima_exausting procedure from the original RBM paper.
// Warning: it may lose global minima! :-/
//
void RBM::minima_exhausting (Collection * list_of_minima, Collection * RL,
    	                    Collection * RU, ElementSubset * M, CostFunction * c)
{
  list <ElementSubset *> stack;
  ElementSubset * A, * B, * T;
  bool exausted;
  A = new ElementSubset ("minimum_exausted",
                         M->get_set_that_contains_this_subset ());
  A->copy (M);
  stack.push_back (A);
    
  while (! stack.empty ())
  {
    exausted = true;

    T = stack.back ();            // Getting just a copy from the stack's top,
                                  // without popping it!
    do
    {
      A = RBM::select_adjacent (&stack, RL, RU, T);

      if (A != NULL)
      {
        A->cost = c->cost (A);
      
        if (T->cost >= A->cost)
        {
          B = new ElementSubset ("", M->get_set_that_contains_this_subset ());
          B->copy (A);
          stack.push_back (B);
          exausted = false;
        }
        else if (A->contains (M))
          UCurveToolBox::update_upper_restriction (RU, A);
        else
          UCurveToolBox::update_lower_restriction (RL, A);
        delete A;
      }
    }
    while (A != NULL);
        
    if (exausted)
    {
      stack.pop_back ();
      list_of_minima->add_subset (T);
      UCurveToolBox::update_upper_restriction (RU, T);
      UCurveToolBox::update_lower_restriction (RL, T);
      delete T;
    }
  }
}

