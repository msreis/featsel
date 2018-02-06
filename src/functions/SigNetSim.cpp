// SigNetSim.cpp -- implementation of the class "SigNetSim".
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

#include "SigNetSim.h"
#include "Python.h"


SigNetSim::SigNetSim (ElementSet * a_set)
{
  set = a_set;
}


SigNetSim::~SigNetSim ()
{
  // Empty destructor.
}


double SigNetSim::cost (ElementSubset * X)
{
  timeval begin, end;
  gettimeofday (& begin, NULL);

  double cost = -1; // This value will make it fail in the empty set
                   // unit test, which should return zero.

  number_of_calls_of_cost_function++;

  if (set->get_set_cardinality () == 0)
    return 0;

  Py_Initialize ();

  int argc = 1;
  char * argv [1], * str;
  str = argv[0] = new char [set->get_set_cardinality () + 3];
  strcpy (argv[0], X->print_subset ().c_str ());

  PySys_SetArgv (argc, argv);

  FILE * sns_input_f = fopen (SIGNETSIM_INPUT_PATH, "r");

  if (sns_input_f == NULL)
    cout << "Error while opening SigNetSim input file!" << endl;

  PyRun_SimpleFile (sns_input_f, SIGNETSIM_PROGRAM);

  fclose (sns_input_f);

  Py_Finalize ();

  FILE * sns_output_f = fopen (SIGNETSIM_OUTPUT_PATH, "r");

  if (sns_output_f == NULL)
    cout << "Error while opening SigNetSim output file!" << endl;

  while (fscanf (sns_output_f, "%s", str) > 0)
    cout << str << " ";
  cout << endl;

  fclose (sns_output_f);


  delete argv[0];

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
