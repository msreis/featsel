use strict;
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

# Number of instances to be processed for each combination of 
# parameters p and l
my $number_of_instances = 25;
# The size of the instance
my $instance_size = 100;
my $p_step;
my $max_p;
my $l_step;
my $max_l;

if (@ARGV == 7)
{
  $output_file_prefix = $ARGV[0];
  $number_of_instances = $ARGV[1];
  $instance_size = $ARGV[2];
  $p_step = $ARGV[3];
  $l_step = $ARGV[4];
  $max_p = $ARGV[5];
  $max_l = $ARGV[6];
}
else
{
  die "\nSyntax: $0 " . 
  " OUTPUT_FILE_PREFIX  k  n P_STEP L_STEP  \n" . 
  " MAX_P MAX_L\n\n";
}


print "Starting $0 program.\nExecution of $number_of_instances " .
      "instances of size $instance_size \n".
      "Parameters p and l varies from 0 to $max_p and $max_l ".
      "respectively. The increment of parameters p and l for every " .
      "observation are, respectively, $p_step and $l_step\n\n";



# Generate instances
my @instance_file;             # Store the list of instance file names.
my $MAX_ELEM_VALUE = 100000;   # Maximum value of an element of S.
foreach my $i (1..$number_of_instances)
{
  SubsetSum::random_subset_sum_instance
    ($i, $MAX_ELEM_VALUE, $INPUT_DIR, $_);
}
# Load files
opendir (my $dh, $INPUT_DIR) or die "Cannot open input directory: $!\n";
@instance_file = grep { /.*\.xml$/ && -f "$INPUT_DIR/$_" }
  readdir ($dh);
closedir $dh;
my @experiment;
foreach my $file (sort @instance_file)
{
  if ($file =~ /Test_0*(\d+)_\w+/)
  {
    push @experiment, $file;
  }
  else
  {
    die "\nError: '$file' file name must follow this format: Test_\\d+_\\w+." .
        "xml" . ".\n\n";
  }
}

my $p_values = $max_p / $p_step;
my $l_values = $max_l / $l_step;
my @avg_time;
my @avg_err;

foreach my $i (1..$p_values)
{
  foreach my $j (1..$l_values)
  {
    my $p = $i * $p_step;
    my $l = $j * $l_step;
    print "Runnig with p = $p and l = $l ...";
    $avg_time[$i][$j] = 0;
    $avg_err[$i][$j] = 0;
    foreach my $k (1..$number_of_instances)
    {
      my ($t0, $t1);
      $t0 = [gettimeofday];
      print "instance size = $instance_size\n";
      system ("$FEATSEL_BIN -n $instance_size -a pucs $p $l " . 
              "-c subset_sum -f $INPUT_DIR/" . $experiment[$k] . 
              " > $LOG_FILE");
      $t1 = [gettimeofday];

      $avg_time[$i][$j] += tv_interval ($t0, $t1);

      open (LOG, $LOG_FILE);
      while (<LOG>)
      {
        if ($_ =~ /(\<\d+\>)\s+\:\s+(\S+)/)
        {
          my $minimum_found = $2;
          $avg_err[$i][$j] += $minimum_found;
        }
      }
      close(LOG);
      
      $avg_err[$i][$j] /= $number_of_instances;
      $avg_time[$i][$j] /= $number_of_instances;
    }
    print "[done].\n";
  }
}

# Creates graph
my $GNUPLOT_DATA_FILE = $OUTPUT_DIR . "/dat.temp";
my $GNUPLOT_PLOT_FILE = $OUTPUT_DIR . "/gnuplot.temp";
print "\nPrinting output graphs... ";

open (DATA, ">$GNUPLOT_DATA_FILE");
for (my $i = 1; $i <= $p_values; $i++)
{
  for (my $j = 1; $j <= $l_values; $j++)
  { 
    my $p = $i * $p_step;
    my $l = $j * $l_step;
    printf DATA "$p $l %.4f %.6f\n", $avg_err[$i][$j],
                                     $avg_time[$i][$j];
  }
}
close (DATA);

my $Xaxis = $max_p * 3;
my $grid_points = $max_l * 3;

open (PLOT, ">$GNUPLOT_PLOT_FILE");

my $x = $max_l * 3;
printf PLOT "set terminal svg enhanced size 700, 500\n";
printf PLOT "set output '$OUTPUT_DIR/$output_file_prefix\_time.svg'\n";
printf PLOT "unset key\n";
printf PLOT "set xlabel \"p\" rotate parallel\n";
printf PLOT "set ytics 1\n";
printf PLOT "set ylabel \"l\" rotate parallel\n";
printf PLOT "set zlabel \"Average Total time (seconds)\" rotate parallel\n";
printf PLOT "unset colorbox\n";
printf PLOT "set dgrid3d $grid_points, $max_l\n";
printf PLOT "set hidden3d\n";  
printf PLOT "splot \"$GNUPLOT_DATA_FILE\" using 1:2:4 with lines\n";

# Execute Gnuplot.
system ("gnuplot $GNUPLOT_PLOT_FILE");

# Remove temporary files.
# system ("rm $GNUPLOT_DATA_FILE");
# system ("rm $GNUPLOT_PLOT_FILE");
print "[done]\n";