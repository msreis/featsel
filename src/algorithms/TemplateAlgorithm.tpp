//
// %template%.cpp -- implementation of the class "%template%".
//
//    This file is part of the featsel program
//    Copyright (C) 2016  Marcelo S. Reis
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

#include "%template%.h"


%template%::%template% ()
{
  list_of_visited_subsets = new Collection ();
  cost_function = NULL;
}


%template%::~%template% ()
{
  if (list_of_visited_subsets != NULL)
  delete list_of_visited_subsets;
}


void %template%::get_minima_list (unsigned int max_size_of_minima_list)
{
  timeval begin_program, end_program;
  gettimeofday (& begin_program, NULL);

  //

  // TODO: [ADD YOUR ALGORITHM CODE HERE!]

  ElementSubset * X;
  X = new ElementSubset ("X", set);    
  X->cost = cost_function->cost (X);   // A template algorithm returns the
  X->set_complete_subset ();           // complete subset as best one, since it
	list_of_minima.push_back (X);  // does not perform any search yet!
  //

  number_of_visited_subsets =
  cost_function->get_number_of_calls_of_cost_function ();
  clean_list_of_minima (max_size_of_minima_list);

  gettimeofday (& end_program, NULL);
  elapsed_time_of_the_algorithm = diff_us (end_program, begin_program);

}


