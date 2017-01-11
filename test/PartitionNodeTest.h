//
// PartitionNodeTest.h -- definition of the namespace "PartitionNodeTest".
//
//    This file is part of the featsel program
//    Copyright (C) 2016 Marcelo S. Reis, Gustavo Estrela de Matos
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

#ifndef PARTITION_NODE_TEST_H_
#define PARTITION_NODE_TEST_H_

#include "../src/Partition.h"
#include "../src/PartitionNode.h"
#include "../src/ElementSubset.h"
#include "../src/ElementSet.h"
#include "../src/global.h"

namespace PartitionNodeTest
{

  bool it_map_subsets_correctly ();

  bool it_should_check_upper_adjacent_parts ();

  bool it_should_return_the_least_subset ();

  bool it_should_return_the_greatest_subset ();

}
  
#endif /* PARTITION_NODE_TEST_H_ */
