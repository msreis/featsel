//
// CHCGATest.h -- definition of the namespace "CHCGATest".
//
//    This file is part of the featsel program
//    Copyright (C) 2018 Marcelo S. Reis, Gustavo Estrela
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
//    along with this program.  If not, see <http://www.gnu.org/liceanses/>.
//

#ifndef CHCGA_POPULATION_TEST_H_
#define CHCGA_POPULATION_TEST_H_

#include "../src/CHCGAPopulation.h"
#include "../src/functions/SubsetSum.h"

namespace CHCGAPopulationTest
{

  bool it_should_create_a_starting_population ();

  bool it_should_combine_individuals ();

  bool only_the_fittest_should_survive ();

}

#endif /* CHCGA_POPULATION_TEST_H_ */
