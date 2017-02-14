#!/usr/bin/perl -w


#
# run_core_benchmark : program that creates a benchmark especifically
#     for the PUCS algorithm. This benchmark creates a table that shows
#     average time used to solve random instances of the  U-Curve 
#     problem, with different number of cores being used. The random 
#     instances are created using the SubsetSum cost function.
#
#    This file is part of the featsel program
#    Copyright (C) 2016  Marcelo S. Reis
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

use POSIX qw(ceil floor);
use strict;
use Time::HiRes qw (gettimeofday tv_interval);
use lib './lib';
use SubsetSum;


my $INPUT_DIR = "input/tmp"; # Directory where the input instances are stored.
my $OUTPUT_DIR = "output"; # Directory where output files are stores
my $LOG_FILE = $OUTPUT_DIR . "/result.log"; # Name of the log file
my $FEATSEL_BIN = "./bin/featsel"; # featsel binary file.
my $number_of_instances_per_size = 1;
my $maximum_instance_size = 1;
my $max_number_of_cores = 1;
my $output_file_prefix = "";

if ((@ARGV == 4))
{
  $output_file_prefix = $ARGV[0];
  $number_of_instances_per_size = $ARGV[1];
  $maximum_instance_size = $ARGV[2];
  $max_number_of_cores = $ARGV[3];
}
else
{
  die "\nSyntax: $0 " . 
      " OUTPUT_FILE_PREFIX k  n  MAX_CORES\n\n" .
      "Where:\n\n" .
      "    OUTPUT_FILE_PREFIX: prefix of the output file name.\n" .
      "    MAX_CORES: the maximum number of core used in the benchmark\n";
}

print "Starting $0 program.\nExecution on instances from size 1 to ". 
      "$maximum_instance_size.\nFor each size, it calculates the average of " . 
      "$number_of_instances_per_size executions.\n";



#------------------------------------------------------------------------------#
#
# Printing HTML output file header
#
open (OUT, ">output/" . $output_file_prefix . "_table.html") 
  or die "Error: could not open HTML output file!\n";
print OUT "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//EN\">\n";

print OUT "<HTML><HEAD>" .
"<meta http-equiv=\"content-type\" content=\"text/html; charset=iso-8859-1\">" .
  "<TITLE>Feature selection algorithms and cost functions " . 
  "benchmarking</TITLE></HEAD>\n";

# Information
print OUT "<BODY><H1>PUCS Core Speedup Benchmark</H1><HR>\n";
print OUT "<H3>Table description:</H3>\n";
print OUT "<EM>2^|S|:</EM> size of the search space of an instance of S " .
  "elements.<BR>\n";
print OUT "<EM>Total Time (sec):</EM> required time to run each algorithm " .
  "(average of up to $number_of_instances_per_size runnings).<BR>\n";

# Table 
## Header
print OUT "<TABLE border='5'>\n";
print OUT "<TR bgcolor='yellow'>\n";
print OUT "<TD colspan=2><CENTER>Size of instance</CENTER></TD>\n";
print OUT "<TD><CENTER>&nbsp;</CENTER></TD>\n" .
  "<TD colspan=$max_number_of_cores><CENTER>Maximum Number of Cores</CENTER>" .
  "</TD></TR>\n";
## Header 2nd line
print OUT "<TD><CENTER>&nbsp;|S|&nbsp;</CENTER></TD><TD><CENTER>&nbsp;2^|S|" . 
  "&nbsp;</CENTER></TD>";
print OUT  "<TD><CENTER>&nbsp;</CENTER></TD>";
for (my $i = 0; $i < $max_number_of_cores; $i++)
{
  printf OUT "<TD><CENTER>&nbsp; %s &nbsp;</CENTER></TD>", $i + 1;
}


#------------------------------------------------------------------------------#
# Create random instances and load the input files.
#
my @instance_file;             # Store the list of instance file names.
my $MAX_ELEM_VALUE = 100000;   # Maximum value of an element of S.

foreach my $i (1..$maximum_instance_size)    
{
  # Create files containing random instances.
  foreach (1..$number_of_instances_per_size)
  {
    SubsetSum::random_subset_sum_instance ($i, $MAX_ELEM_VALUE, $INPUT_DIR, $_);
  }
}


# Load file names.
my %experiment;
opendir (my $dh, $INPUT_DIR) or die "Cannot open input directory: $!\n";
@instance_file = grep { /.*\.xml$/ && -f "$INPUT_DIR/$_" }
  readdir ($dh);
closedir $dh;
(scalar(@instance_file) >=
  $number_of_instances_per_size * $maximum_instance_size)
  or die "Insufficient number of instance files stored in '$INPUT_DIR'!\n\n";

foreach my $file (sort @instance_file)
{
  if ($file =~ /Test_0*(\d+)_\w+/)
  {
    push @{$experiment{$1}}, $file;
  }
  else
  {
    die "\nError: '$file' file name must follow this format: Test_\\d+_\\w+." .
        "xml" . ".\n\n";
  }
}


#------------------------------------------------------------------------------#
# Run experiments
#
my %total_time;
my $MAX_TIME_VALUE = 0; # useful for generating graphs later
print "\nRunning benchmarking experiments with up to $max_number_of_cores " .
        "cores and instances of size up to $maximum_instance_size.\n\n";
foreach my $i (1..$maximum_instance_size)
{
  print "Starting iteration $i...";
  # Current instance size (|S| = $i, implying on a 2^$i search space)
  printf OUT "<TR><TD><CENTER>&nbsp;%2d&nbsp;</CENTER></TD>", $i;
  printf OUT "<TD><CENTER>&nbsp;%7d&nbsp;</CENTER></TD>", 2 ** $i;

  my @average_time_of_core;
  for (my $j = 0; $j < $max_number_of_cores; $j++)
  {
    $average_time_of_core[$j] = 0;
  }

  foreach my $k (1..$number_of_instances_per_size)
  {
    for (my $j = 0; $j < $max_number_of_cores; $j++)
    {
      $ENV{'OMP_NUM_THREADS'} = $j + 1;
      # print $ENV{'OMP_NUM_THREADS'} . "\n";
      my ($t0, $t1);
      $t0 = [gettimeofday];
      system ("$FEATSEL_BIN -n $i -a pucs " . 
              "-c subset_sum -f $INPUT_DIR/" . $experiment{$i}->[$k-1]  . 
              " > $LOG_FILE");
      $t1 = [gettimeofday];
      $average_time_of_core[$j] += tv_interval ($t0, $t1);      
    } 
  } 

  for (my $j = 0; $j < $max_number_of_cores; $j++)
  {
    $average_time_of_core[$j] /= $number_of_instances_per_size;
  }

  # Outputs average time
  printf OUT "<TD><CENTER>&nbsp;</CENTER></TD>\n";
  for (my $j = 0; $j < $max_number_of_cores; $j++)
  {
    printf OUT "<TD><CENTER>&nbsp;%4.2f&nbsp;</CENTER></TD>",
      $average_time_of_core[$j];
  }
  print OUT "</TR>\n";
  print "[done]\n";
}

# Create the table and HTML footers.
print OUT "</TABLE><BR><HR>\n";
print OUT "
<p>
    <a href=\"http://validator.w3.org/check?uri=referer\"><img
        src=\"http://www.w3.org/Icons/valid-html401-blue\"
        alt=\"Valid HTML 4.01 Strict\" height=\"31\" width=\"88\"></a>
</p>\n";
print OUT "</BODY></HTML>\n";
close (OUT);


system ("rm $LOG_FILE");
print "\nEnd of execution.\n\n";
exit 0;
