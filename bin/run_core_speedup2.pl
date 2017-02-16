use strict;
use POSIX;
use lib './lib';
use SubsetSum;
use Time::HiRes qw (gettimeofday tv_interval);

# Output file prefix
my $output_file_prefix = "foo";
# Directory where the input instances are stored.
my $INPUT_DIR = "input/tmp";
# Directory where the output files are stored.
my $OUTPUT_DIR = "output";
# featsel binary file.
my $FEATSEL_BIN = "./bin/featsel";
# Name of the log file used when the featsel binary is executed.
my $LOG_FILE = $OUTPUT_DIR . "/result.log";


my $number_of_instances;
my $instance_size;
my $max_cores;
my $p;
my $l;

if (@ARGV == 6)
{
  $output_file_prefix = $ARGV[0];
  $number_of_instances = $ARGV[1];
  $instance_size = $ARGV[2];
  $p = $ARGV[3];
  $l = $ARGV[4];
  $max_cores = $ARGV[5];
}
else
{
  die "\nSyntax: $0 " . 
  " OUTPUT_FILE_PREFIX  k  n p l MAX_CORES  \n\n";
}
print "Starting $0 program.\nExecution of $number_of_instances " .
      "instances of size $instance_size \n".
      "    Parameter p is $p\n".
      "    Parameter l is $l\n\n";
      "    Number of cores varies from 1 to $max_cores";

# Generate instances
my @instance_file;               # Store the list of instance file names.
my $MAX_ELEM_VALUE = 10000000;   # Maximum value of an element of S.
foreach my $i (1..$number_of_instances)
{
  SubsetSum::random_subset_sum_instance 
  ($instance_size, $MAX_ELEM_VALUE, $INPUT_DIR, $i);
}
# Load files
opendir (my $dh, $INPUT_DIR) or die "Cannot open input directory: $!\n";
@instance_file = grep { /.*\.xml$/ && -f "$INPUT_DIR/$_" }
  readdir ($dh);
closedir $dh;
my @experiment;
foreach my $file (sort @instance_file)
{
  if ($file =~ /(Test_0*${instance_size}_\d+.\w+)/)
  {
    push @experiment, $file;
  }
  else
  {
    die "\nError: '$file' file name must follow this format: Test_${instance_size}_(\\d+)" .
        ".xml" . "\n\n";
  }
}


my @avg_time;
foreach my $i (1..$max_cores)
{
  $avg_time[$i] = 0.0;
}
foreach my $i (0..$number_of_instances - 1)
{
  foreach my $k (1..$max_cores)
  {
    my ($t0, $t1);
    $t0 = [gettimeofday];
    $ENV{'OMP_NUM_THREADS'} = $k;
    system ("$FEATSEL_BIN -n $instance_size -a pucs $p $l " . 
            "-c subset_sum -f $INPUT_DIR/" . $experiment[$i] . 
            " > $LOG_FILE");
    $t1 = [gettimeofday];
    $avg_time[$k] += tv_interval ($t0, $t1);
  }
}
foreach my $i (1..$max_cores)
{
  $avg_time[$i] /= $number_of_instances;
}

# Creates graph data
my $GNUPLOT_DATA_FILE = $OUTPUT_DIR . "/dat.temp";
my $GNUPLOT_PLOT_FILE = $OUTPUT_DIR . "/gnuplot.temp";
print "\nPrinting output graphs... ";

open (DATA, ">$GNUPLOT_DATA_FILE");
for (my $i = 1; $i <= $max_cores; $i++)
{
    printf DATA "$i %.4f\n", $avg_time[$i];
}
close (DATA);

# Creates time graph
open (PLOT, ">$GNUPLOT_PLOT_FILE");
printf PLOT "set terminal svg enhanced size 700, 500\n";
printf PLOT "set output '$OUTPUT_DIR/$output_file_prefix\_core_speedup2.svg'\n";
printf PLOT "unset key\n";
printf PLOT "set xlabel \"Number of cores\" rotate parallel\n";
printf PLOT "set ylabel \"Average Time Spent\" rotate parallel\n";
printf PLOT "plot \"$GNUPLOT_DATA_FILE\" using 1:2 with lines\n";
# Execute Gnuplot.
system ("gnuplot $GNUPLOT_PLOT_FILE");
system ("rm $GNUPLOT_PLOT_FILE");

# system ("rm $INPUT_DIR/*.xml");
print "[done]\n";