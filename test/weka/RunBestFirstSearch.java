//
// RunBestFirstSearch.java
//
// A program that run command-line version of Weka to perform feature selection
// using Best-First Search (BFS) as the algorithm and the Correlation-based
// Feature Selection (CFS) as the cost function.
//
// M.S.Reis, February 17, 2017.

// Requires "weka" program, which can be downloaded through apt tool.

// To compile, call javac with the Weka classpath in your system. Example:
//
// javac -Xlint -classpath ".:/usr/share/java/weka.jar" RunBestFirstSearch.java
//
// To execute, call java with your Weka classpath and also the arguments:
//
// java -classpath "./:/usr/share/java/weka.jar" RunBestFirstSearch file.arff n
//
// where "file.arff" is a sample file in Weka's format and n is the
// number of features contained in that file.

// Package weka.attributeSelection.
//
import weka.attributeSelection.*;
import weka.core.*;
import weka.core.converters.ConverterUtils.*;

import java.lang.String;
import java.util.*;


public class RunBestFirstSearch
{
  protected static void runAttributeSelection (Instances data, int n)
            throws Exception
  {
    AttributeSelection attsel   = new AttributeSelection ();
    CfsSubsetEval cost_function = new CfsSubsetEval ();    // CFS cost function.
    BestFirst algorithm         = new BestFirst ();        // BFS algorithm.

    cost_function.buildEvaluator (data);

    algorithm.setLookupCacheSize (n);

    // BFS with forward direction and terminating search after five
    // non-improving nodes. 
    //
    String [] parameters = {"-D 1", "-N 5"};

    algorithm.setOptions (parameters);

    attsel.setEvaluator (cost_function);
    attsel.setSearch (algorithm);

    attsel.SelectAttributes (data);

    int [] indices = attsel.selectedAttributes ();

    System.out.println ("Selected features:\n" + Utils.arrayToString (indices));
  }

  public static void main (String[] arg) throws Exception
  {
    // Load data.
    //
    System.out.println ("\nLoading sample file...");
    DataSource source = new DataSource (arg[0]);
    Instances data = source.getDataSet ();

    if (data.classIndex () == -1)
      data.setClassIndex (data.numAttributes () - 1);
  
    int n = Integer.parseInt (arg[1]);

    System.out.println ("Instance with " + n + " features!");
    System.out.println ("\nRunning BFS algorithm with CFS cost function...");

    // Run feature selection algorithm BFS using CFS cost function.
    //
    runAttributeSelection (data, n);
  }

}

// End of program.

