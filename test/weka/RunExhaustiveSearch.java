//
// RunExhaustiveSearch.java - A program that run command-line version of Weka to
// perform feature selection using Exhaustive Search (ES) as the algorithm and
// the Correlation-based Feature Selection (CFS) as the cost function.
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

// Requires "weka" program, which can be downloaded through apt tool.

// To compile, call javac with the Weka classpath in your system. Example:
//
// javac -Xlint -classpath ".:/usr/share/java/weka.jar" RunExhaustiveSearch.java
//
// To execute, call java with your Weka classpath and also the arguments:
//
// java -classpath "./:/usr/share/java/weka.jar" RunExhaustiveSearch file.arff n
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


public class RunExhaustiveSearch
{
  protected static void runAttributeSelection (Instances data, int n)
            throws Exception
  {
    AttributeSelection attsel   = new AttributeSelection ();
    CfsSubsetEval cost_function = new CfsSubsetEval ();    // CFS cost function.
    ExhaustiveSearch algorithm  = new ExhaustiveSearch (); //  ES algorithm.

    cost_function.buildEvaluator (data);

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
    System.out.println ("\nRunning ES algorithm with CFS cost function...");

    // Run feature selection algorithm ES using CFS cost function.
    //
    runAttributeSelection (data, n);
  }

}

// End of program.

