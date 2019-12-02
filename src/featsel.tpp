//============================================================================
//
//    featsel -- a flexible feature selection program.
//    Copyright (C) 2017  Marcelo S. Reis
//
//
//   If you use featsel in your publication, we kindly ask you to acknowledge us
//   by citing the paper that describes this framework:
//
//   M.S. Reis, G. Estrela, C.E. Ferreira and J. Barrera
//   "featsel: A Framework for Benchmarking of
//   Feature Selection Algorithms and Cost Functions"
//   SoftwareX 6 (2017) pp. 193-197.
//   DOI: 10.1016/j.softx.2017.07.005.
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
//============================================================================


# include "global.h"
# include "string.h"
# include "ElementSet.h"
# include "CostFunction.h"

# define noop   // no operation

// <COST FUNCTION TEMPLATE 1>

// <ALGORITHM TEMPLATE 1>


// Function to parse and verify the correctness of the parameters.
// It returns '0' if all the parameters were correct, '1' if there was an error,
// and '2' if the option '-h' (help) was called.
//
int parse_parameters
  (int, char **, string *, unsigned int *, string *,
   unsigned int *, unsigned int *, bool *, string *, unsigned int *, double * p,
   unsigned int *, unsigned int *, unsigned int *);


// The main function.
//
int main(int argc, char * argv[])
{
  unsigned int max_number_of_minima = 1;
  unsigned int n = 3;
  unsigned int range = 1000;
  unsigned int number_of_labels = 2;
  unsigned int l = 2;                                         // PUCS parameter.
  unsigned int max_number_of_calls_of_cost_function = 0;      // 0 for no limit.
  double p = .5;                                              // PUCS parameter.
  unsigned int k = 10;        // Spec-CMI parameter.
  int i;
  CostFunction * cost_function;
  Solver * solver;
  ElementSet * S;
  string list;
  string a_cost_function ("subset_sum");
  string algorithm ("ubb");
  string file_name;
  bool store_visited_subsets = false;
  ofstream log_file;


  // enables omp nested parallelization
  omp_set_nested (true);

  srand ((unsigned) time (NULL));

  // Parse the parameters
  //
  i = parse_parameters(argc, argv, &file_name, &max_number_of_minima,
      &a_cost_function, &n, &range, &store_visited_subsets, &algorithm,
      &max_number_of_calls_of_cost_function, &p, &l, &number_of_labels, &k);

  if (i != EXIT_SUCCESS)    // Help or error in parameters
    return EXIT_FAILURE;

  // Select the type of instance (from XML file, from DAT file, or random)
  //
  if ((file_name.size () > 0) && ((file_name.find (".xml") != string::npos) ||
      (file_name.find (".XML") != string::npos)))
    S = new ElementSet ("S", file_name.data ());
  else if ((file_name.size () > 0) &&
           ((file_name.find (".dat") != string::npos) ||
            (file_name.find (".DAT") != string::npos)))
    S = new ElementSet (number_of_labels, file_name.data (), n);
  else
    S = new ElementSet ("S", n, range);

  // Instantiate an object of the selected solver (algorithm)
  //
  if (0)
    noop;
  // <ALGORITHM TEMPLATE 2>
  else
    return EXIT_FAILURE;

  // Instantiate an object of the selected cost function
  //
  if (0)
    noop;
  // <COST FUNCTION TEMPLATE 2>
  else
    return EXIT_FAILURE;

  // If the solver has a heuristic strategy, here we
  // set the maximum number of calls of the cost function
  //
  if (max_number_of_calls_of_cost_function > 0)
    cost_function->set_max_number_of_calls_of_cost_function
    (max_number_of_calls_of_cost_function);

  // Set the solver parameters
  //
  solver->set_parameters (cost_function, S, store_visited_subsets);

  // Run the solver
  //
  solver->get_minima_list (max_number_of_minima);

  // Print the results
  //
  cout << endl << "== List of best subsets found ==" << endl;
  cout << " X : c(X)" << endl;
  cout << solver->print_search_report ();
  if (store_visited_subsets)
  {
    cout << endl << "Visited subsets stored into 'visited.log' file" << endl;
    log_file.open ("output/visited.log");
    log_file << solver->print_list_of_visited_subsets () << endl;
    log_file.close ();
  }
  else
  {
    int elapsed_time =
    cost_function->get_the_elapsed_time_of_the_calls_of_this_cost_function ();

    // Number of visited subsets is the number of calls
    // of the cost function!
    //
    int number_of_subsets = solver->print_number_of_visited_subsets ();
    if (number_of_subsets > 0)
    {
      cout << "\nNumber of visited subsets: " <<  number_of_subsets << endl;
      cout << "Required time to compute the visited subsets: "
           << elapsed_time << " microseconds " << endl;
      cout << "(average " << elapsed_time / number_of_subsets
           << " microseconds per node)" << endl;
    }
    cout << endl << "Elapsed time of execution of the algorithm "
         << "(in microseconds): "
         << solver->get_elapsed_time_of_the_algorithm () << endl;
  }

  cout << endl << "== End of processing ==" << endl << endl;

  // Free allocated memory
  //
  delete S;
  delete solver;
  delete cost_function;

  // Exit to the OS
  //
  return EXIT_SUCCESS;

} // end of main



// parse_parameters function implementation
//
int parse_parameters (int argc, char ** argv, string * file_name,
	              unsigned int * m, string * c, unsigned int * n,
	              unsigned int * range, bool * keep_subsets, string * a,
	              unsigned int * max_number_of_calls_of_cost_function,
                  double * p, unsigned int * l, unsigned int * number_of_labels,
                      unsigned int * number_of_features)
{
  int i;
  bool error = false;
  string a_line;
  ifstream a_file;
  string disclaimer ("featsel -- a flexible feature selection program.\n\
Copyright (C) 2017  Marcelo S. Reis.\n\n \
This program comes with ABSOLUTELY NO WARRANTY.\n \
This is free software, and you are welcome to redistribute it\n \
under certain conditions; see 'LICENSE.TXT' for details. \n\n \
If you use featsel in your publication, we kindly ask you to acknowledge us \n \
by citing the paper that describes this framework: \n\n \
M.S. Reis, G. Estrela, C.E. Ferreira and J. Barrera \n \
'featsel: A Framework for Benchmarking of \n \
Feature Selection Algorithms and Cost Functions' \n \
SoftwareX 6 (2017) pp. 193-197.\n \
DOI: 10.1016/j.softx.2017.07.005.\n\n \
");

  // Parse the arguments
  //
  for (i = 1; i < argc; i++)
  {

    // -f
    //
    if ( (strcmp (argv[i],"-f") == 0) && ((i+1) < argc) )
    {
      a_file.open (argv[++i]);
      if (a_file.is_open ())
      {
        a_file.close ();
        file_name->append (argv[i]);
      }
      else
      {
        cout << "\nError: unable to open input file '" << argv[i] << "'!\n";
        error = true;
      }
    }
    else if ( (strcmp (argv[i],"-f") == 0) && ((i+1) >= argc) )
    {
      cout << "\nError: parameter '-f' must have a file name." << endl;
      error = true;
    }

    // -r
    //
    else if ( (strcmp( argv[i],"-r") == 0) && ((i+1) < argc) )
    {
      *range = atoi (argv[++i]);
      if (*range >= RAND_MAX)
      {
        cout << "\nError: range should be in [0, " << RAND_MAX << ") !\n";
        error = true;
      }
    }
    else if ( (strcmp (argv[i],"-r") == 0) && ((i+1) >= argc) )
    {
      cout << "\nError: parameter '-r' must have a value." << endl;
      error = true;
    }

    // -t   a limit to the number of calls of the cost function
    //
    else if ( (strcmp( argv[i],"-t") == 0) && ((i+1) < argc) )
    {
      *max_number_of_calls_of_cost_function = atoi (argv[++i]);
    }
    else if ( (strcmp (argv[i],"-t") == 0) && ((i+1) >= argc) )
    {
      cout << "\nError: parameter '-t' must have a value." << endl;
      error = true;
    }

    // -n
    //
    else if ( (strcmp (argv[i],"-n") == 0) && ((i+1) < argc) )
    {
      if (atoi (argv[++i]) > 0)
        *n = atoi (argv[i]);
      else
      {
        cout << "\nError: number of elements should be a positive integer!\n";
        error = true;
      }
    }
    else if ( (strcmp (argv[i],"-n") == 0) && ((i+1) >= argc) )
    {
      cout << "\nError: parameter '-n' must have a value." << endl;
      error = true;
    }

    // -c
    //
    else if ( (strcmp (argv[i],"-c") == 0) && ((i+1) < argc) )
    {
      i++;
      if ( (0) 
          // <COST FUNCTION TEMPLATE 3>           
         )
      {
        c->clear();
        c->append(argv[i]);
      }
      else
      {
        cout << "\nError: unknown cost function '" << argv[i] << "'!" << endl;
        error = true;
      }
    }
    else if ( (strcmp (argv[i], "-a") == 0) && ((i+1) >= argc) )
    {
      cout << "\nError: parameter '-c' must have a cost function name." << endl;
      error = true;
    }

    // -a
    //
    else if ( (strcmp(argv[i],"-a") == 0) && ((i+1) < argc) )
    {
      i++;
      if ( (0)  
          // <ALGORITHM TEMPLATE 3> 
         )
      {
        a->clear ();
        a->append (argv[i]);
        if (strcmp (argv[i], "pucs") == 0) 
        {
          if (i + 2 < argc) {
            if (argv[i + 1][0] != '-')
              *p = atof (argv[++i]);
            else 
              *p = 0;
            if (argv[i + 1][0] != '-')
              *l = atoi (argv[++i]);
            else
              *l = 1;
          }
          else {
            *l = 1;
            *p = 0;
          }
        }
      }
      else
      {
        cout << "\nError: unknown algorithm '" << argv[i] << "'!" << endl;
        error = true;
      }
    }
    else if ( (strcmp (argv[i], "-a") == 0) && ((i+1) >= argc) )
    {
      cout << "\nError: parameter '-a' must have an algorithm name." << endl;
      error = true;
    }

    // -m
    //
    else if ( (strcmp (argv[i], "-m") == 0) && ((i+1) < argc) )
    {
      if (atoi (argv[++i]) > 0)
        *m = atoi (argv[i]);
      else
      {
        cout << "\nError: maximum number of minima should be "
             << "a positive integer!\n";
        error = true;
      }
    }
    else if ( (strcmp (argv[i],"-m") == 0) && ((i+1) >= argc) )
    {
      cout << "\nError: parameter '-m' must have a value." << endl;
      error = true;
    }

    // -l
    //
    else if ( (strcmp (argv[i], "-l") == 0) && ((i+1) < argc) )
    {
      if (atoi (argv[++i]) > 0)
        *number_of_labels = atoi (argv[i]);
      else
      {
        cout << "\nError: number of labels should be "
             << "a positive integer!\n";
        error = true;
      }
    }
    else if ( (strcmp (argv[i],"-l") == 0) && ((i+1) >= argc) )
    {
      cout << "\nError: parameter '-l' must have a value." << endl;
      error = true;
    }

    // -k
    //
    else if ( (strcmp (argv[i], "-k") == 0) && ((i+1) < argc) )
    {
      if (atoi (argv[++i]) > 0)
        *number_of_features = atoi (argv[i]);
      else
      {
        cout << "\nError: number of features to be selected should be "
             << "a positive integer!\n";
        error = true;
      }
    }
    else if ( (strcmp (argv[i],"-k") == 0) && ((i+1) >= argc) )
    {
      cout << "\nError: parameter '-k' must have a value." << endl;
      error = true;
    }

    // -s
    //
    else if (strcmp (argv[i],"-s") == 0)
    {
      *keep_subsets = true;
    }

    // -h
    //
    else if (strcmp (argv[i],"-h") == 0)
    {
      cout << endl << disclaimer << endl << endl;
      a_file.open ("docs/featsel.txt");
      if (a_file.is_open ())
        while(a_file.good ())
        {
          getline (a_file, a_line);
          cout << a_line << endl;
        }
        a_file.close ();
        return EXIT_FAILURE;
    }

    // Unknown option
    //
    else
    {
      cout <<  endl << "Unknown option '" << argv[i] << "'." << endl;
      error = true;
    }
  }

  // Show help dialog in case of error
  //
  if ( (error) || (argc == 1) )
  {
    cout << endl << disclaimer << endl;
    cout << endl << "Type 'featsel -h' for help." << endl << endl;
    return EXIT_FAILURE;
  }

  // Arguments parsed successfully
  //
  else
    return EXIT_SUCCESS;

} // End of function

