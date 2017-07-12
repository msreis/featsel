//
// PartCost.cpp -- implementation of the class "PartCost".
//
//    This file is part of the featsel program
//    Copyright (C) 2017 Gustavo Estrela
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

#include "PartCost.h"


PartCost::PartCost (CostFunction * c_f, PartitionNode * p)
{
  orig_cost_f = c_f;
  partition = p;
}


PartCost::~PartCost ()
{
  // Empty destructor.
}


double PartCost::cost (ElementSubset * X)
{
  timeval begin, end;
  gettimeofday (& begin, NULL);

  number_of_calls_of_cost_function++;

  ElementSubset * original_subset;
  original_subset = partition->get_original_subset (X);
  double cost = orig_cost_f->cost (original_subset);
  delete original_subset;

  gettimeofday (& end, NULL);
  elapsed_time_of_cost_function_calls += diff_us (end, begin);

  // Threshold is a maximum number of calls of the cost function
  //
  if ((has_max_number_of_calls_set) &&
      (number_of_calls_of_cost_function >=
       max_number_of_calls_of_cost_function))
    reached_threshold = true;
  return cost;
}
