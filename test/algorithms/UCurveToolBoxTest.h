//
// UCurveToolBoxTest.h -- definition of the namespace "UCurveToolBoxTest".
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

#ifndef UCURVECOLLECTIONTEST_H_
#define UCURVECOLLECTIONTEST_H_

#include "../../src/algorithms/UCurveToolBox.h"

namespace UCurveToolBoxTest
{
  bool it_should_update_lower_restriction_set_with_an_outer_subset ();

  bool it_should_not_update_lower_restriction_set_with_a_covered_subset ();

  bool it_should_update_upper_restriction_set_with_an_outer_subset ();

  bool it_should_not_update_upper_restriction_set_with_a_covered_subset ();

  bool it_should_select_an_unsivited_adjacent ();

  bool it_should_get_a_maximal_subset ();
}

#endif /* UCURVECOLLECTIONTEST_H_ */
