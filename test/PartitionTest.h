//
// PartitionTest.h -- definition of the namespace "PartitionTest".
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

#ifndef PARTITION_TEST_H_
#define PARTITION_TEST_H_

#include "../src/Partition.h"
#include "../src/ElementSet.h"
#include "../src/ElementSubset.h"
#include "../src/global.h"

namespace PartitionTest
{

    bool it_should_create_the_fixed_elm_set ();

    bool it_should_create_the_unfixed_elm_set ();

}

#endif /* PARTITION_TEST_H_ */
