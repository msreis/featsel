#!/usr/bin/perl -w

# Perl program to carry out the comparisons of the featsel framework against
# the Weka workbench, in order to test the performance of the former.
#
# M.S.Reis, April 12, 2017.
#

# This program requires to be within the "/bin" directory in the featsel 
# directory structure. Additionally, these two Java programs should be stored
# in "/test/weka":
#
#           RunBestFirstSearch.java
#           RunExhaustiveSearch.java
#
# And the system should have installed Java (compiler and JVM) and also the
# Weka workbench (the tested version was 3.6.13).
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

my $WEKA_CLASSPATH = "test/weka:/usr/share/java/weka.jar";


# The used data sets.
#
my @DATA_SETS = ("Zoo", "Promoters", "Arrhythmia", "Gisette");

my %labels    = ("Zoo" => 7,  "Promoters" => 2,  "Arrhythmia" => 16, 
                 "Gisette" => 2);

my %features =  ("Zoo" => 15, "Promoters" => 57, "Arrhythmia" => 279, 
                 "Gisette" => 5000);

my %dat_file =  ("Zoo" => "input/Zoo/Test_15_3.dat",
                 "Promoters" => "input/Promoters/Test_01_A.dat",
                 "Arrhythmia" => "input/Arrhythmia/Test_01_A.dat",
                 "Gisette" => "input/Gisette/Test_01_A.dat");

my %algorithm = ("Zoo" => "es",
                 "Promoters" => "bfs", 
                 "Arrhythmia" => "bfs", 
                 "Gisette" => "bfs");

# Number of repetitions per data set.
#
my $k = 10;

# Weka's ARFF file.
#
my $arff_file = "input/tmp/Test_01_A.arff";


# Compiling Weka's wrappers.
#
system ("javac -Xlint -classpath \"$WEKA_CLASSPATH\" $WEKA_BFS_FILE 2> $LOG");
system ("javac -Xlint -classpath \"$WEKA_CLASSPATH\" $WEKA_ES_FILE 2> $LOG");

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
            " -c cfs " .
            " -f " . $dat_file{$data_set} . 
            " > $LOG");
    my $t1 = [gettimeofday];

    printf "%2d featsel run on $data_set, required time = %f\n",
      $i, tv_interval ($t0, $t1);

    if ($algorithm{$data_set} eq "es")
    {
      $t0 = [gettimeofday];
      system ("java -classpath \"$WEKA_CLASSPATH\" $WEKA_ES_CLASS " .
              $arff_file . " " . $features{$data_set} . " 1> $LOG 2>> $LOG");
      $t1 = [gettimeofday];
    }
    else
    {
      $t0 = [gettimeofday];
      system ("java -classpath \"$WEKA_CLASSPATH\" $WEKA_BFS_CLASS " .
              $arff_file . " " . $features{$data_set} . " 1> $LOG 2>> $LOG");
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
    elsif (($dataset eq "Promoters") || ($dataset eq "Gisette"))
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
