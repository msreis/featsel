// CHCGAPopulation.h automatically generated by bin/add_new_algorithm.pl
// in Fri May  4 11:19:48 2018.

//
// CHCGAPopulation.h -- definition of the class "CHCGAPopulation".
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

// Algorithm published in:
//
// L.J. Eshelman, The CHC adaptive search algorithm: how to safe search 
// when engaging in non traditional genetic recombination, in: 
// G.J.E. Rawlins (Ed.), Foundations of Genetic Algorithms, Morgan 
// Kaufmann, San Mateo, 1991, pp. 265–283

#ifndef CHCGAPopulation_H_
#define CHCGAPopulation_H_

#include "ElementSet.h"
#include "ElementSubset.h"
#include "CostFunction.h"
#include <vector>


class CHCGAPopulation
{


private:

  // Defines an individual as an ElementSubset
  //
  typedef ElementSubset Individual;

  // Defines the population structure as a map<double, Individual *>
  //
  typedef multimap<double, Individual *> Population;

  // The current population
  //
  Population population;

  // The set of all possible elements an individual can hold
  //
  ElementSet * set;

  // The cost function used to evaluate each individual
  //
  CostFunction * c;

  // Stores the size of the population
  //
  unsigned int population_max_size;

  // Minimum Hamming Distance between two parents
  //
  unsigned int diff_threshold;

  // Stores the divergence rate
  // 
  // L. J. Eshelman uses 35% on his experiments
  //
  static constexpr double div_rate = .35;

  // Performs a cataclysm on the population.
  //
  void cataclysm ();

  // Removes and destroy a population entry
  //
  void kill_individual (Population::iterator);

  // Functor with operator () as a comparison function.
  //
  struct Functor
  {
    bool operator () (ElementSubset * first, ElementSubset * second)
    {
      if (first->cost < second->cost)
        return true;
      else
        return false;
    }
  };

public:

  // Default constructor.
  //
  CHCGAPopulation (ElementSet *, CostFunction *);

  // Default destructor.
  //
  virtual ~CHCGAPopulation ();

  // Starts a population with size given as a parameter
  //
  void start_population (unsigned int);

  // Mates current individuals and return a list of children
  //
  list<ElementSubset *> recombine ();

  // Combines two individuals
  //
  ElementSubset * combine_individuals (ElementSubset *, ElementSubset *);

  // Receives a list of children and adds it to the population. Then 
  // only the fittest individual survive. Returns true if a cataclysm 
  // happened.
  //
  bool fittest_survival (list<ElementSubset *>);

  // Returns population size
  //
  unsigned int get_size ();

  // Returns the current population
  //
  list<ElementSubset *> get_population ();
};

#endif /* CHCGAPopulation_H_ */
