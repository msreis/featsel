//
// CostFunction.cpp -- implementation of the class "CostFunction".
//
//    This file is part of the featsel program
//    Copyright (C) 2017  Marcelo S. Reis
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

#include "CostFunction.h"


CostFunction::CostFunction ()
{
  set = NULL;
  number_of_calls_of_cost_function = 0;
  elapsed_time_of_cost_function_calls = 0;
  has_threshold_set = false;
  reached_threshold = false;
  has_max_number_of_calls_set = false;
}


CostFunction::~CostFunction ()
{
  // Empty destructor.
}


// "diff" in microseconds
//
int CostFunction::diff_us (timeval t1, timeval t2)
{
  return (((t1.tv_sec - t2.tv_sec) * 1000000) + (t1.tv_usec - t2.tv_usec));
}


void CostFunction::add_elapsed_time (int microseconds)
{
  timeval begin, end;
  useconds_t micro_seconds = microseconds;

  gettimeofday (& begin, NULL);

  usleep (micro_seconds); // pause

  gettimeofday (& end, NULL);

  elapsed_time_of_cost_function_calls += diff_us (end, begin);
}


unsigned int CostFunction::get_number_of_calls_of_cost_function ()
{
  return number_of_calls_of_cost_function;
}


int CostFunction::get_the_elapsed_time_of_the_calls_of_this_cost_function ()
{
  return elapsed_time_of_cost_function_calls;
}


unsigned int CostFunction::get_max_number_of_calls_of_cost_function ()
{
  return max_number_of_calls_of_cost_function;
}


void CostFunction::set_max_number_of_calls_of_cost_function
(unsigned int max_number_of_calls)
{
  has_max_number_of_calls_set = true;
  max_number_of_calls_of_cost_function = max_number_of_calls;
}


double CostFunction::get_threshold ()
{
  return threshold;
}


void CostFunction::set_threshold (double value)
{
  has_threshold_set = true;
  threshold = value;
}


bool CostFunction::has_reached_threshold ()
{
  return reached_threshold;
}


bool CostFunction::has_threshold ()
{
  return (has_threshold_set || has_max_number_of_calls_set);
}

