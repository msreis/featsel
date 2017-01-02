#featsel
A flexible feature selection program. This program  receives  a  set  S of objects, values associated with each of them, a cost function c with subsets of S as domain and real numbers as image,  and searches for a subset X in S such as c(X) is minimum (i.e., there are no Y in S, such as c(Y) < c(X)).  The  minima  subsets  found are printed into the standard output.

## Requirements
These requirements are mandatory:
- a terminal environment (XTerm, MS-DOS prompt, etc.);
- the "make" command or equivalent;
- a C++ compiler (GNU g++ or equivalent), with the standard libraries.

Additionally, the following dependencies are recommended:
- GNU Bison 2.4.1 or higher, required to update the XML parser.
- Flex 2.5 or higher, also required to update the XML parser.
- Perl 5 interpreter version 17 or higher, which is required to: 
  - recompile 'featsel.cpp' and 'featselTest.cpp' files;
  - add and remove algorithms and cost functions;
   - run the benchmarking program.
- Gnuplot to generate performance graphs using the benchmarking program.
- GNU groff to update a help man page.


## How to Compile
### The Main Program And the Unit Tests
After unzip this package, just open a terminal and type, inside the directory
were it was unzipped:

```
> make
```

To compile the tests, type:

```
> make test
```

### The Extended Markup Language (XML) Parser
Warning: this procedure requires GNU Bison and GNU Flex.

After the modifications in the XML parser files, type:

```
> make bison
```

and then:

```
> make flex
```

### Update 'featsel.cpp' and 'featselTest.cpp' Files
Warning: this procedure requires Perl 5.
Just type:

```
> make featsel.cpp
```

and

```
> make featselTest.cpp
```

to update the featsel program main and unit test main files, respectively.

After a successful addition of new algorithms and/or cost functions, these two commands are executed automatically.


### Help Man Page
Warning: this procedure requires GNU groff.

Just type:

```
> make help
```

to update the help man page (file "docs/featsel.txt").


### Everything at Once
Warning: this procedure requires Perl 5, GNU Bison, Flex and groff.

Just type:

```
> make all
```

in order to compile at once everything listed in items (3.1)--(3.4).


### Clen Objects and Binaries
To remove the objects and binaries produced by the compiler, type:

```
> make clean
```


## Using Featsel
### Main Program
To run the featsel program, after a successful compilation just type 

```
> bin/featsel
```

inside the main directory. Here follows a syntax example, in which the program is run for an subset sum instance of seven elements, using the SFS algorithm and showing the best four results:

```
> bin/featsel -f input/subset_sum/Test_07_A.xml -c subset_sum -a sfs -m 4  
```

The output of this call should be like this one:
```
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
```
 
The first column is a subset X of the set S of elements, in the format [b_1, ..., b_|S|], where the bit b_i is "1" is it belongs to X and "0" otherwise, and the second column is the cost of X, that is, the value of c(X).

For the proper use of the parameters, type ```bin/featsel -h``` or look at featsel paper describing this framework.


### Unit Tests
To run the test program, make test and type:

```
> bin/featselTest
```

It should run all the tests automatically, displaying the results into the
standard output.

### Runnig the Benchmarking Program
The syntax of benchmarking auxiliary program is:

```
> perl bin/run_benchmarking.pl  OUTPUT_FILE_PREFIX  INSTANCE_MODE COST_FUNCTION_CODE  k  n   SEARCH_MODE  [max_number_of_calls]
```

Where:
- ```OUTPUT_FILE_PREFIX```: prefix of the output file name.

- ```COST_FUNCTION_CODE```: code of the employed cost function.

- ```INSTANCE_MODE```: must be 0 or 1; if the value is 0, then for each size of instance in [1,n], it creates k random instances of the chosen cost function. Otherwise, it reads input files already generated, which must have k files per instance size, which in turn must range from 1 to n.

- ```SEARCH_MODE```: must be 0 or 1; if the value is 0, then it performs a complete search, which may be optimal or not depending on the algorithm. Otherwise, it runs a search constrained by 'max_number_of_calls' calls of the cost function.

In the following sections, we show two examples with different usage of the syntax.


#### Example 1: 
Running optimal search with subset sum instances createdon-the-fly by the benchmarking program.

Try to type:

```
> perl bin/run_benchmarking.pl test 0 subset_sum 10 7 0
```

With this command, it should generate an HTML table which contains, for each subset sum instance of size i in [1,7] and for each algorithm j, the average results of 10 runs for:
 - Required total time (in seconds);
 
 - Required time to compute all cost function calls (in seconds);
	
 - Number of calls of the cost function (i.e., number of computed nodes);
	
 - Number of times j had the best solution among all algorithms.

It also generates, for each algorithm, an eps graph which depicts the average required computational time (both total and cost function ones) as a function of the instance size. For instance, if the algorithms are "ES", "SFS", "SFFS", and "UBB", then for the example above the /output directory should contain the following files:

```
  test_ES.eps
  test_SFFS.eps
  test_SFS.eps
  test_table.html
  test_UBB.eps
```

#### Example 2: 
Running an optimal search with Mean Conditional Entropy instances read directly from the /input/tmp directory.

Try to type:

```
> perl bin/run_benchmarking.pl test 1 mce 3 4 0
```

In this example, we assume that there are, for each instance size between 1 and 4, at least 3 Mean Conditional Entropy (mce) instances in .dat format insde the input/tmp directory.


#### Example 3: 
Running a suboptimal search with Explicit instances created on-the-fly by the benchmarking program, with a limit of 100 calls of the cost function; these instances range from size 1 to 5, with 20 instances per size.

Try to type:

```
> perl bin/run_benchmarking.pl test 1 explicit 20 5 1 100
```

In this example, for each instance (regardless of its size), each algorithm will compute the cost function up to 100 times and then end its execution.

  
## Creating New Algorithms and Cost Functions
### Including a New Algorithm

To include a new algorithm, type:

```
> perl bin/add_new_algorithm.pl code ClassName
```

Where:
- ```code```: a code for the algorithm, which will be employed as an argument in the featsel main program in order to use this algorithm.

- ```ClassName```: name of the algorithm class.

If the user try to add an algorithm whose name is already used by either another algorithm, a cost function, or other main program class and/or file, the "add_new_algorithm.pl" program will return an error message.

If the user successfuly include a new algorithm, say X, then the following files will be created:

```
|---\src\
|   |---\algorithms\
|       |---\X.cpp
|       |---\X.h
|    
|---\test\
|   |---\algorithms\
|       |---\XTest.cpp
|       |---\XTest.h
```

Besides, the "Makefile", "featsel.cpp", and "featselTest.cpp" will be properly updated. The user must just edit the files in the directory tree above and run "make" and "make test" in order to have her/his new algorithm available.


### Including a New Cost Function
To include a new cost function, type:

```
> perl bin/add_new_cost_function.pl code ClassName file_type
```

Where:

- ```code```: a code for the cost function, which will be employed as an argument in the featsel main program in order to use this cost function.

- ```ClassName```: name of the cost function class.

- ```file_type```: may be either "dat" (flat file) or "xml".


If the user try to add a cost function whose name is already used by either another cost function, an algorithm or other main program class and/or file, the "add_new_cost_function.pl" program will return an error message.

If the user successfuly include a new cost function, say Y, then the following files will be created:

```
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
```

Besides, the "Makefile", "featsel.cpp", and "featselTest.cpp" will be properly updated. The user must just edit the files in the directory tree above and run "make" and "make test" in order to have her/his new cost function available.

It is noteworthy to remark that once a new cost function is included, then it can be used straightforwardly in the benchmarking program if the user put manually them into the "/input/tmp" directory. However, to generate random instances of the newly included cost function, the user must edit a subroutine to this end within the file "/lib/Y.m"; the subroutine itself will be named as "sub random_y_instance", where "y" is the code of the cost function Y.


## Bug Report and Contact:
Marcelo S. Reis
msreis at butantan dot gov dot br
