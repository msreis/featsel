#!/usr/bin/perl -w

#
# compare_featsel_against_Weka.pl : program to carry out the comparisons of the
# featsel framework against the Weka workbench, in order to test the performance
# of the former.
#
#    This file is part of the featsel program
#    Copyright (C) 2017  Marcelo S. Reis
#
#    This program is free software: you can redistribute it and/or modify
#    it under the terms of the GNU General Public License as published by
#    the Free Software Foundation, either version 3 of the License, or
#    (at your option) any later version.
#
#    This program is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU General Public License for more details.
#
#    You should have received a copy of the GNU General Public License
#    along with this program.  If not, see <http://www.gnu.org/licenses/>.
#

# This program requires to be within the "/bin" directory in the featsel 
# directory structure. Additionally, these two Java programs should be stored
# in "/test/weka":
#
#           RunBestFirstSearch.java
#           RunExhaustiveSearch.java
#
# And the system should have installed Java (compiler and JVM) and also the
# Weka workbench (the tested versions were >= 3.6.1).
#
# To execute this program, just call "bin/compare_featsel_against_Weka.pl".

use strict;

use Time::HiRes qw (gettimeofday tv_interval);


# Main definitions and variables of featsel and Weka.
#
my $FEATSEL_BIN    = "./bin/featsel";

my $LOG            = "output/result.log";


my $WEKA_BFS_CLASS  = "RunBestFirstSearch";
my $WEKA_BFS_FILE  = "test/weka/$WEKA_BFS_CLASS.java";

my $WEKA_ES_CLASS  = "RunExhaustiveSearch";
my $WEKA_ES_FILE  = "test/weka/$WEKA_ES_CLASS.java";

my $WEKA_DIR =  $ENV{'WEKAINSTALL'};
if (!length $WEKA_DIR) 
{
  print "Please enter the weka.jar directory (e.g. /usr/share/java). " . 
        "To avoid this message," .
        " set up the WEKAINSTALL environment variable!\n";
  $WEKA_DIR = <STDIN>;
  chomp $WEKA_DIR;
}
my $WEKA_CLASSPATH = "test/weka:$WEKA_DIR/weka.jar:" . 
                     "$WEKA_DIR/attributeSelectionSearchMethods.jar";

# The used data sets.
#
my @DATA_SETS = ( "Zoo", "Promoters", "Arrhythmia", "Gisette",
                  "Madelon", "Musk2", "Optdigits", "Waveform");

my %labels    = ("Zoo" => 7,  "Promoters" => 2,  "Arrhythmia" => 16, 
                 "Gisette" => 2, "Madelon" => 2, "Musk2" => 2,
                 "Optdigits" => 10, "Waveform" => 3);

my %features =  ("Zoo" => 15, "Promoters" => 57, "Arrhythmia" => 279, 
                 "Gisette" => 5000, "Madelon" => 500, "Musk2" => 166,
                 "Optdigits" => 64, "Waveform" => 21);

my %dat_file =  ("Zoo" => "input/Zoo/Test_15_3.dat",
                 "Promoters" => "input/Promoters/Test_01_A.dat",
                 "Arrhythmia" => "input/Arrhythmia/Test_01_A.dat",
                 "Gisette" => "input/Gisette/Test_01_A.dat",
                 "Madelon" => "input/Madelon/Test_01_A.dat",
                 "Musk2" => "input/Musk2/Test_01_A.dat",
                 "Optdigits" => "input/Optdigits/Test_01_A.dat",
                 "Waveform" => "input/Waveform/Test_01_A.dat");

my %algorithm = ("Zoo" => "es",
                 "Promoters" => "bfs", 
                 "Arrhythmia" => "bfs", 
                 "Gisette" => "bfs",
                 "Madelon" => "bfs",
                 "Musk2" => "bfs",
                 "Optdigits" => "bfs",
                 "Waveform" => "bfs");



# Number of repetitions per data set.
#
my $k = 20;

# Weka's ARFF file.
#
my $arff_file = "input/tmp/Test_01_A.arff";


# Upper bound for cost function calls (a time stop criterion, specially for
# the execution of the featsel framework on the Gisette data set).
#
my $MAX_NUMBER_OF_COST_FUNCTION_CALLS = 1000000;


# Compiling Weka's wrappers.
#
system ("javac -Xlint -classpath \"$WEKA_CLASSPATH\" $WEKA_BFS_FILE");
system ("javac -Xlint -classpath \"$WEKA_CLASSPATH\" $WEKA_ES_FILE");


# Compiling featsel (if it was not compiled yet).
#
system ("make");

# Comparison main loop.
#
foreach my $data_set (@DATA_SETS)
{
  dat2arff ($data_set, $dat_file{$data_set}, $arff_file,
            $features{$data_set}, $labels{$data_set});

  foreach my $i (1..$k)
  {
    my $t0 = [gettimeofday];
    system ("$FEATSEL_BIN " . 
            " -n " . $features{$data_set} . 
            " -a " . $algorithm{$data_set} . 
            " -l " . $labels{$data_set} .
            " -c " . "cfs" .
            " -t " . $MAX_NUMBER_OF_COST_FUNCTION_CALLS .
            " -f " . $dat_file{$data_set} . 
            " > $LOG");
    my $t1 = [gettimeofday];

    printf "%2d featsel run on $data_set, required time = %f\n",
      $i, tv_interval ($t0, $t1);

    if ($algorithm{$data_set} eq "es")
    {
      $t0 = [gettimeofday];
      system ("java -classpath \"$WEKA_CLASSPATH\" $WEKA_ES_CLASS " .
              $arff_file . " " . $features{$data_set} . " 1> $LOG");
      $t1 = [gettimeofday];
    }
    else
    {
      $t0 = [gettimeofday];
      system ("java -classpath \"$WEKA_CLASSPATH\" $WEKA_BFS_CLASS " .
              $arff_file . " " . $features{$data_set} . " 1> $LOG");
      $t1 = [gettimeofday];
      
    }
    printf "%2d Weka run on $data_set, required time = %f\n",
      $i, tv_interval ($t0, $t1);
  }
}


# Clean up java compilation, log file and the remaining ARFF file.
#
system ("rm test/weka/*.class");
system ("rm $LOG");
system ("rm $arff_file");


# End of program.
#
exit 0;

#------------------------------------------------------------------------------#

# Subroutine that converts a featsel DAT file into ARFF file.
# Requirement: the DAT file must have one observation per line.
#
sub dat2arff
{
  my ($dataset, $input_file, $output_file, $n, $l) = @_;

  open (OUTPUT, ">" . $output_file) 
    or die "Could not open $output_file!\n";

  print OUTPUT "\@RELATION $dataset-$n-$l\n\n";

  for (my $i = 0; $i < $n; $i++)
  {
    if ($dataset eq "Zoo")
    {
      print OUTPUT "\@ATTRIBUTE $i {0, 1}\n";
    }
    elsif (($dataset eq "Promoters") || ($dataset eq "Gisette") ||
           ($dataset eq "Musk2")     || ($dataset eq "Waveform") ||
           ($dataset eq "Madelon")   || ($dataset eq "Optdigits"))
    {
      print OUTPUT "\@ATTRIBUTE $i {0, 1, 2, 3}\n";
    }
    elsif ($dataset eq "Arrhythmia")
    {
      if (($i == 1) || (($i >= 21) && ($i <= 26)))
      {
        print OUTPUT "\@ATTRIBUTE $i {0, 1}\n";
      }
      else
      {
        print OUTPUT "\@ATTRIBUTE $i {0, 1, 2, 3}\n";
      }
    }
  }

  print OUTPUT "\n\@ATTRIBUTE class {0";

  for (my $i = 1; $i < $l; $i++)
  {
    print OUTPUT ", $i";
  }
  print OUTPUT "}\n";

  print OUTPUT "\n\@DATA\n";

  open (INPUT, $input_file) or die "Could not open $input_file!\n";

  while (<INPUT>)
  {
    chomp $_;
    my @line = split " ", $_;
    for (my $i = 0; $i < $n; $i++)
    {
      print OUTPUT $line[$i] . ",";
    }
    my $label;
    for (my $i = 0; $i < $l; $i++)
    {
      $line[$n + $i] == 1 and $label = $i;
    }
    print OUTPUT $label . "\n";
  }

  close (INPUT);
  close (OUTPUT);
}

#------------------------------------------------------------------------------#
