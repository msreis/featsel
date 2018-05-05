//
// CHCGAPopulation.cpp -- implementation of the class "CHCGAPopulation".
//
//    This file is part of the featsel program
//    Copyright (C) 2018  Marcelo S. Reis, Gustavo Estrela
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

#include "CHCGAPopulation.h"
#include <algorithm>

CHCGAPopulation::CHCGAPopulation (ElementSet * set, CostFunction * c)
{
  this->set = set;
  this->c = c;
}


CHCGAPopulation::~CHCGAPopulation ()
{
  return;
}


void CHCGAPopulation::start_population (unsigned int pop_size)
{
  unsigned int set_card = set->get_set_cardinality ();
  population_max_size = pop_size;

  for (unsigned int i = 0; i < pop_size; i++)
  {
    Individual * X = new Individual ("", set);
    for (unsigned int e = 0; e < set_card; e++)
      if ((unsigned int) rand () % 2)
        X->add_element (e);
    X->cost = c->cost (X);
    population.insert (make_pair (X->cost, X));
  }
}


list<ElementSubset *> CHCGAPopulation::recombine ()
{
  unsigned int pop_size = population.size ();
  vector<Individual *> fathers;
  vector<Individual *> mothers;
  vector<Individual *>::iterator f_it;
  vector<Individual *>::iterator m_it;
  list<Individual *> children;
  Population::iterator pop_it;

  pop_it = population.begin ();
  for (unsigned int i = 0; i < pop_size / 2; i++)
  {
    fathers.push_back (pop_it->second);
    pop_it++;
  }
  while (pop_it != population.end ())
  {
    mothers.push_back (pop_it->second);
    pop_it++;
  }

  random_shuffle (fathers.begin (), fathers.end ());
  random_shuffle (mothers.begin (), mothers.end ());

  f_it = fathers.begin ();
  m_it = mothers.begin ();
  while (f_it != fathers.end () && m_it != mothers.end ())
  {
    // cout << "father = " << (*f_it)->print_subset () << endl;
    // cout << "mother = " << (*m_it)->print_subset () << endl;
    Individual * child = combine_individuals (*f_it, *m_it);
    // cout << "child = " << child->print_subset () << endl;
    if (child != NULL)
      children.push_back (child);
    f_it++;
    m_it++;
  }

  return children;
}


ElementSubset * CHCGAPopulation::combine_individuals (ElementSubset * f,
 ElementSubset * m)
{
  unsigned int set_card = set->get_set_cardinality ();
  Individual * child = new Individual ("", set);
  ElementSubset differing_genes ("", set);

  for (unsigned int e = 0; e < set_card; e++) 
  {
    if (f->has_element (e) && m->has_element (e))
      child->add_element (e);
    else
      differing_genes.add_element (e);
  }

  while (!differing_genes.is_empty ())
  {
    unsigned int e = differing_genes.remove_random_element ();
    if (f->has_element (e))
      child->add_element (e);
    
    if (!differing_genes.is_empty ())
    {
      e = differing_genes.remove_random_element ();
      if (m->has_element (e))
        child->add_element (e);
    }
  }

  return child;
}


unsigned int CHCGAPopulation::fittest_survival (list<ElementSubset *> 
  children)
{
  list<Individual *>::iterator children_it = children.begin ();
  unsigned int children_alive = children.size ();

  // cout << "Population to survive: " << endl;
  // Population::iterator pop_it;
  // for (pop_it = population.begin (); pop_it != population.end (); pop_it++)
  // {
  //   cout << pop_it->second->print_subset () << endl;
  // }
  // cout << "Children to enter population: " << endl;
  // for (children_it = children.begin (); children_it != children.end (); children_it++)
  // {
  //   cout << (*children_it)->print_subset () << endl;
  // }
  // children_it = children.begin ();

  Population::iterator least_fit = --population.end ();
  while (children_it != children.end ())
  {
    Individual * child = *children_it;
    child->cost = c->cost (child);

    if (least_fit != population.end () && 
      child->cost >= least_fit->first)
    {
      children_alive--;
      children_it = children.erase (children_it);
    }
    else
    {
      population.insert (make_pair (child->cost, child));
      population.erase (least_fit--);
    }
    children_it++;
  }

  return children_alive;
}

  
unsigned int CHCGAPopulation::get_size ()
{
  return population.size ();
}


list<ElementSubset *> CHCGAPopulation::get_population ()
{
  list<Individual *> population_list;
  Population::iterator it, p_end;

  for (it = population.begin (), p_end = population.end (); 
    it != p_end; it++)
  {
    population_list.push_back (it->second);
  }

  return population_list;
}