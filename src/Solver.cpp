//
// Solver.cpp -- implementation of the class "Solver".
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

#include "Solver.h"


Solver::Solver ()
{
  cost_function = NULL;
  list_of_visited_subsets = NULL;
  set = NULL;
  store_visited_subsets = false;
  number_of_visited_subsets = 0;
  elapsed_time_of_the_algorithm = 0;
}


Solver::~Solver ()
{
  ElementSubset * X;

  while (list_of_minima.size () > 0)
  {
    X = list_of_minima.back ();
    list_of_minima.pop_back ();
    delete X;
  }
}


void Solver::set_parameters (CostFunction * a_cost_function,
     ElementSet * a_set, bool keep_subsets)
{
  set = a_set;
  cost_function = a_cost_function;
  store_visited_subsets = keep_subsets;
}


float Solver::subset_cost (ElementSubset * X)
{
  return cost_function->cost (X);
}


string Solver::print_list_of_minima ()
{
  list<ElementSubset *>::iterator it;
  string list_string ("");
  for (it = list_of_minima.begin (); it != list_of_minima.end (); it++)
    list_string.append ((*it)->print_subset ());
  return list_string;
}


string Solver::print_list_of_visited_subsets ()
{
  return list_of_visited_subsets->print_collection ();
}


string Solver::print_search_report ()
{
  list<ElementSubset *>::iterator it;
  string report_string ("");
  std::ostringstream value;
  for (it = list_of_minima.begin (); it != list_of_minima.end (); it++)
  {
    report_string.append ((*it)->print_subset ());
    report_string.append (": ");
    value.precision (COST_FUNCTION_PRECISION);
    value <<  (*it)->cost;
    report_string.append (value.str ());
    report_string.append ("\n");
    value.str ("");
  }
  return report_string;
}


unsigned int Solver::print_number_of_visited_subsets ()
{
  return number_of_visited_subsets;
}


// Clean the minima list up to 'max_number_of_minima'
//
void Solver::clean_list_of_minima (unsigned int max_size_of_minima_list)
{
  ElementSubset * X;
  Functor f; // struct of class Solver that stores the comparison function

  int number_of_elements_to_remove = list_of_minima.size () - 
      (int) max_size_of_minima_list;

  f.cf = cost_function;

  list_of_minima.sort (f);

  while (number_of_elements_to_remove > 0)
  {
    X = list_of_minima.back ();
    list_of_minima.pop_back ();
    delete X;
    number_of_elements_to_remove--;
  }
}


unsigned int Solver::number_of_minima ()
{
  return list_of_minima.size ();
}


int Solver::get_elapsed_time_of_the_algorithm ()
{
  return elapsed_time_of_the_algorithm;
}


unsigned int Solver::get_number_of_iterations ()
{
  return number_of_iterations;
}


// "diff" in microseconds
//
int Solver::diff_us (timeval t1, timeval t2)
{
  return (((t1.tv_sec - t2.tv_sec) * 1000000) + (t1.tv_usec - t2.tv_usec));
}


void Solver::store_visited_subset (ElementSubset * X)
{
  if (store_visited_subsets)
    list_of_visited_subsets->add_subset (X);
}


void Solver::store_minimum_subset (ElementSubset * X)
{
  ElementSubset * Y;
  Y = new ElementSubset ("", set);
  Y->copy (X);
  list_of_minima.push_back (Y);
}


list<ElementSubset *> Solver::get_list_of_minima ()
{
  list<ElementSubset *> lst;
  list<ElementSubset *>::iterator it = list_of_minima.begin ();
  while (it != list_of_minima.end ()) 
  { 
    lst.push_back (new ElementSubset (*it));
    it++;
  }
  return lst;
}


Collection * Solver::get_list_of_visited_subsets () 
{
  return list_of_visited_subsets;
}
