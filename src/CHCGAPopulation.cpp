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
  vector<Individual *> fathers (pop_size + 1, NULL);
  vector<Individual *> mothers (pop_size + 1, NULL);
  list<Individual *> children;
  Population::iterator pop_it;
  vector<Individual *>::iterator f_it;
  vector<Individual *>::iterator m_it;

  pop_it = population.begin ();
  for (unsigned int i = 0; i < pop_size / 2; i++)
    fathers.push_back (pop_it->second);
  while (pop_it != population.end ())
    mothers.push_back (pop_it->second);

  random_shuffle (fathers.begin (), fathers.end ());
  random_shuffle (mothers.begin (), mothers.end ());

  while (f_it != fathers.end () && m_it != mothers.end ())
  {
    Individual * child = combine_individuals (*f_it, *m_it);
    children.push_back (child);
    f_it++;
    m_it++;
  }
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


unsigned int CHCGAPopulation::get_size ()
{
  return population.size ();
}