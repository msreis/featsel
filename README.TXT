//==============================================================================
//
//    featsel -- a flexible feature selection program.
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
//==============================================================================

SUMMARY:

(1) CONTENTS

(2) REQUIREMENTS

(3) HOW TO COMPILE
  (3.1) MAIN PROGRAM
  (3.2) THE EXTENDED MARKUP LANGUAGE (XML) PARSER
  (3.3) UPDATE 'featsel.cpp' AND 'featselTest.cpp' FILES
  (3.4) HELP MAN PAGE
  (3.5) EVERYTHING AT ONCE
  (3.6) CLEAN OBJECTS AND BINARIES

(4) USING FEATSEL
  (4.1) MAIN PROGRAM
  (4.2) UNIT TESTS
  (4.3) RUNNING THE BENCHMARKING PROGRAM

(5) INCLUDING AND REMOVING ALGORITHMS AND COST FUNCTIONS
  (5.1) INCLUDING A NEW ALGORITHM
  (5.2) INCLUDING A NEW COST FUNCTION
  (5.3) REMOVING AN ALGORITHM
  (5.4) REMOVING A COST FUNCTION

(6) BUG REPORT AND CONTACT

--------------------------------------------------------------------------------

(1) CONTENTS

This zip package should contain the following directories and files:

\featsel
|
|---\bin\
|   |---\add_new_algorithm.pl                - add new algorithm into framework.
|   |---\add_new_cost_function.pl        - add new cost function into framework.
|   |---\build_benchmarking_file.pl           - recompile 'run_benchmarking.pl'.
|   |---\build_featsel_main_file.pl         - recompile 'featsel.cpp' main file.
|   |---\build_featselTest_main_file.pl - recompile 'featselTest.ccp' main file.
|   |---\remove_algorithm.pl                  - remove algorithm from framework.
|   |---\remove_cost_function.pl          - remove cost function from framework.
|   |---\run_benchmarking.pl                   - benchmarking program (in Perl).
|
|
|---\docs\
|   |---\featsel.1                                  - help page in troff format.
|   |---\featsel.txt                                - help page in ASCII format.
|   |---\LICENSE.TXT                                      - the GNU/GPL license.
|     
|---\input                                      - directory for the input files.
|   |---\tmp\       - directory to store instances for the benchmarking program.
|
|---\lib\        - random instance generation libs for the benchmarking program.
|    
|---\output\                                   - directory for the output files.
|
|   
|---\src\                                      - featsel classes and namespaces.
|   |---\Collection.cpp
|   |---\Collection.h
|   |---\CostFunction.cpp
|   |---\CostFunction.h
|   |---\Element.cpp
|   |---\Element.h
|   |---\ElementSet.cpp
|   |---\ElementSet.h
|   |---\ElementSubset.cpp
|   |---\ElementSubset.h
|   |---\featsel.tpp
|   |---\global.h
|   |---\Solver.cpp
|   |---\Solver.h
|   |---\algorithms\                      - algorithms classes (inherit Solver).
|   |---\functions\             - cost functions classes (inherit CostFunction).
|   |---\parsers\                      - interpreter files (parser and scanner).
|       |---\DatParserDriver.cpp
|       |---\DatParserDriver.h
|       |---\XmlParserDriver.cpp
|       |---\XmlParserDriver.h
|       |---\XmlParser.yy
|       |---\XmlScanner.ll
|    
|---\test\                           - featsel unit test classes and namespaces.
|   |---\CollectionTest.cpp
|   |---\CollectionTest.h
|   |---\CostFunctionTest.cpp
|   |---\CostFunctionTest.h
|   |---\ElementTest.cpp
|   |---\ElementTest.h
|   |---\ElementSetTest.cpp
|   |---\ElementSetTest.h
|   |---\ElementSubsetTest.cpp
|   |---\ElementSubsetTest.h
|   |---\featselTest.tpp
|   |---\algorithms\                                  - algorithms test classes.
|   |---\functions\                               - cost functions test classes.
|  
|---\Makefile                      - compiles the source code and the test code.
| 
|---\README.TXT                                                     - this file.
  
--------------------------------------------------------------------------------
--------------------------------------------------------------------------------

(2) REQUIREMENTS


These requirements are mandatory:

* a terminal environment (XTerm, MS-DOS prompt, etc.);

* the "make" command or equivalent;

* A C++ compiler (GNU g++ or equivalent), with the standard libraries.


Additionally, the following dependencies are recommended:

* GNU Bison 2.4.1 or higher, required to update the XML parser.

* Flex 2.5 or higher, also required to update the XML parser.

* Perl 5 interpreter version 17 or higher, which is required to: 
     - recompile 'featsel.cpp' and 'featselTest.cpp' files;
     - add and remove algorithms and cost functions;
     - run the benchmarking program.

* Gnuplot to generate performance graphs using the benchmarking program.

* GNU groff to update a help man page.


--------------------------------------------------------------------------------
--------------------------------------------------------------------------------

(3) HOW TO COMPILE

--------------------------------------------------------------------------------

(3.1) THE MAIN PROGRAM AND THE UNIT TESTS

After unzip this package, just open a terminal and type, inside the directory
were it was unzipped:

$> make

To compile the tests, type:

$> make test

--------------------------------------------------------------------------------

(3.2) THE EXTENDED MARKUP LANGUAGE (XML) PARSER

Warning: this procedure requires GNU Bison and GNU Flex.

After the modifications in the XML parser files, type:

$> make bison

And then:

$> make flex

--------------------------------------------------------------------------------

(3.3) UPDATE 'featsel.cpp' AND 'featselTest.cpp' FILES

Warning: this procedure requires Perl 5.

Just type:

$> make featsel.cpp

and

$> make featselTest.cpp

to update the featsel program main and unit test main files, respectively.

After a successful addition of new algorithms and/or cost functions, these two
commands are executed automatically.

--------------------------------------------------------------------------------

(3.4) HELP MAN PAGE

Warning: this procedure requires GNU groff.

Just type:

$> make help

to update the help man page (file "docs/featsel.txt").

--------------------------------------------------------------------------------

(3.5) EVERYTHING AT ONCE

Warning: this procedure requires Perl 5, GNU Bison, Flex and groff.

Just type:

$> make all

in order to compile at once everything listed in items (3.1)--(3.4).

--------------------------------------------------------------------------------

(3.6) CLEAN OBJECTS AND BINARIES

To remove the objects and binaries produced by the compiler, type:

$> make clean


--------------------------------------------------------------------------------
--------------------------------------------------------------------------------

(4) USING FEATSEL

--------------------------------------------------------------------------------

(4.1) MAIN PROGRAM

To run the featsel program, after a successful compilation just type 

$> bin/featsel

inside the main directory. Here follows a syntax example, in which the 
program is run for an subset sum instance of seven elements, using the SFS
algorithm and showing the best four results:

$> bin/featsel -f input/subset_sum/Test_07_A.xml -c subset_sum -a sfs -m 4  

The output of this call should be like this one:


== List of best subsets found ==
 X : c(X)
 <0101111> : 0
 <0001111> : 2
 <0000111> : 6
 <0000011> : 11

Number of visited subsets: 28
Required time to compute the visited subsets: 1770 microseconds 
(average 63 microseconds per node)

Elapsed time of execution of the algorithm (in microseconds): 2074

== End of processing ==

 
The first column is a subset X of the set S of elements, in the format 
[b_1, ..., b_|S|], where the bit b_i is "1" is it belongs to X and "0"
otherwise, and the second column is the cost of X, that is, the value of c(X).

For the proper use of the parameters, type "bin/featsel -h" or look at featsel
paper describing this framework.

--------------------------------------------------------------------------------

(4.2) UNIT TESTS

To run the test program, make test and type:

$> bin/featselTest

It should run all the tests automatically, displaying the results into the
standard output.

--------------------------------------------------------------------------------

(4.3) RUNNING THE BENCHMARKING PROGRAM

The syntax of benchmarking auxiliary program is:

$> perl bin/run_benchmarking.pl  OUTPUT_FILE_PREFIX   COST_FUNCTION_CODE
                INSTANCE_MODE  k  n   SEARCH_MODE  [max_number_of_calls]

Where:

    OUTPUT_FILE_PREFIX: prefix of the output file name.

    COST_FUNCTION_CODE: code of the employed cost function.

    INSTANCE_MODE: must be 0 or 1; if the value is 0, then for each
    size of instance in [1,n], it creates k random instances of the
    chosen cost function. Otherwise, it reads input files already
    generated, which must have k files per instance size, which in
    turn must range from 1 to n.

    SEARCH_MODE: must be 0 or 1; if the value is 0, then it performs
    a complete search, which may be optimal or not depending on the
    algorithm. Otherwise, it runs a search constrained by
    'max_number_of_calls' calls of the cost function.


In the following, we show two examples with different usage of the syntax.


(4.3.1) EXAMPLE 1: running optimal search with subset sum instances created
on-the-fly by the benchmarking program.

Try to type:

$> perl bin/run_benchmarking.pl test 0 subset_sum 10 7 0

With this command, it should generate an HTML table which contains, for each
subset sum instance of size i in [1,7] and for each algorithm j, the average
results of 10 runs for:

 - Required total time (in seconds);
 
 - Required time to compute all cost function calls (in seconds);
	
 - Number of calls of the cost function (i.e., number of computed nodes);
	
 - Number of times j had the best solution among all algorithms.

It also generates, for each algorithm, an eps graph which depicts the average
required computational time (both total and cost function ones) as a function of 
the instance size. For instance, if the algorithms are "ES", "SFS", "SFFS", and
"UBB", then for the example above the /output directory should contain the
following files:

  test_ES.eps
  test_SFFS.eps
  test_SFS.eps
  test_table.html
  test_UBB.eps


(4.3.2) EXAMPLE 2: running an optimal search with Mean Conditional Entropy
instances read directly from the /input/tmp directory.

Try to type:

$> perl bin/run_benchmarking.pl test 1 mce 3 4 0

In this example, we assume that there are, for each instance size between 1 and
4, at least 3 Mean Conditional Entropy (mce) instances in .dat format insde the
input/tmp directory.


(4.3.3) EXAMPLE 3: running a suboptimal search with Explicit instances created
on-the-fly by the benchmarking program, with a limit of 100 calls of the cost
function; these instances range from size 1 to 5, with 20 instances per size.

Try to type:

$> perl bin/run_benchmarking.pl test 1 explicit 20 5 1 100

In this example, for each instance (regardless of its size), each algorithm will
compute the cost function up to 100 times and then end its execution.

--------------------------------------------------------------------------------
--------------------------------------------------------------------------------
  
(5) CREATING NEW ALGORITHMS AND COST FUNCTIONS

--------------------------------------------------------------------------------

(5.1) INCLUDING A NEW ALGORITHM

To include a new algorithm, type:

$> perl bin/add_new_algorithm.pl code ClassName

Where:

    code: a code for the algorithm, which will be employed as an argument
          in the featsel main program in order to use this algorithm.

    ClassName: name of the algorithm class.

If the user try to add an algorithm whose name is already used by either
another algorithm, a cost function, or other main program class and/or file, 
the "add_new_algorithm.pl" program will return an error message.

If the user successfuly include a new algorithm, say X, then the following 
files will be created:

|---\src\
|   |---\algorithms\
|       |---\X.cpp
|       |---\X.h
|    
|---\test\
|   |---\algorithms\
|       |---\XTest.cpp
|       |---\XTest.h

Besides, the "Makefile", "featsel.cpp", and "featselTest.cpp" will be properly
updated. The user must just edit the files in the directory tree above and 
run "make" and "make test" in order to have her/his new algorithm available.

--------------------------------------------------------------------------------

(5.2) INCLUDING A NEW COST FUNCTION

To include a new cost function, type:

$> perl bin/add_new_cost_function.pl code ClassName file_type

Where:

    code: a code for the cost function, which will be employed as an argument
          in the featsel main program in order to use this cost function.

    ClassName: name of the cost function class.

    file_type: may be either "dat" (flat file) or "xml".


If the user try to add a cost function whose name is already used by either
another cost function, an algorithm or other main program class and/or file, 
the "add_new_cost_function.pl" program will return an error message.

If the user successfuly include a new cost function, say Y, then the following
files will be created:

|---\lib\
|   |---\Y.m
|
|---\src\
|   |---\functions\
|       |---\Y.cpp
|       |---\Y.h
|    
|---\test\
|   |---\functions\
|       |---\YTest.cpp
|       |---\YTest.h

Besides, the "Makefile", "featsel.cpp", and "featselTest.cpp" will be properly
updated. The user must just edit the files in the directory tree above and 
run "make" and "make test" in order to have her/his new cost function available.

It is noteworthy to remark that once a new cost function is included, then it
can be used straightforwardly in the benchmarking program if the user put 
manually them into the "/input/tmp" directory. However, to generate random
instances of the newly included cost function, the user must edit a subroutine
to this end within the file "/lib/Y.m"; the subroutine itself will be named as
"sub random_y_instance", where "y" is the code of the cost function Y.

--------------------------------------------------------------------------------
--------------------------------------------------------------------------------

(6) BUG REPORT AND CONTACT:

Marcelo S. Reis
msreis at butantan dot gov dot br


--------------------------------------------------------------------------------
END OF README.TXT
