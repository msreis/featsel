use strict;
use POSIX;
use lib './lib';
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
  $max_p = $ARGV[5] * 1;
  $max_l = $ARGV[6];
}
else
{
  die "\nSyntax: $0 " . 
  " OUTPUT_FILE_PREFIX  k  n P_STEP L_STEP  \n" . 
  " MAX_P MAX_L\n\n";
}

my $p_values = ceil ($max_p / $p_step);
my $l_values = $max_l / $l_step;
print "Starting $0 program.\nExecution of $number_of_instances " .
      "instances of size $instance_size \n".
      "    Parameter p varies from $p_step to $max_p\n".
      "    Parameter l varies from $l_step to $max_l\n\n";

# Generate instances
my @instance_file;             # Store the list of instance file names.
my $MAX_ELEM_VALUE = 100000;   # Maximum value of an element of S.
foreach my $i (1..$number_of_instances)
{
  random_subset_sum_instance ($instance_size, $MAX_ELEM_VALUE, $INPUT_DIR, $_);
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

# Creates graph data
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

# Creates time graph
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
printf PLOT "set dgrid3d $l_values, $p_values\n";
printf PLOT "set hidden3d\n";  
printf PLOT "splot \"$GNUPLOT_DATA_FILE\" using 1:2:4 with lines\n";
# Execute Gnuplot.
system ("gnuplot $GNUPLOT_PLOT_FILE");
system ("rm $GNUPLOT_PLOT_FILE");

my $Xaxis = $max_p * 3;
my $grid_points = $max_l * 3;
open (PLOT, ">$GNUPLOT_PLOT_FILE");
my $x = $max_l * 3;
printf PLOT "set terminal svg enhanced size 700, 500\n";
printf PLOT "set output '$OUTPUT_DIR/$output_file_prefix\_error.svg'\n";
printf PLOT "unset key\n";
printf PLOT "set xlabel \"p\" rotate parallel\n";
printf PLOT "set ytics 1\n";
printf PLOT "set ylabel \"l\" rotate parallel\n";
printf PLOT "set zlabel \"Average Absolute Error from Optimal Solution\" rotate parallel\n";
printf PLOT "unset colorbox\n";
printf PLOT "set dgrid3d $l_values, $p_values\n";
printf PLOT "set hidden3d\n";  
printf PLOT "splot \"$GNUPLOT_DATA_FILE\" using 1:2:3 with lines\n";
# Execute Gnuplot.
system ("gnuplot $GNUPLOT_PLOT_FILE");
# Remove temporary files.
# system ("rm $GNUPLOT_PLOT_FILE");
# system ("rm $GNUPLOT_DATA_FILE");
print "[done]\n";


sub random_subset_sum_instance
{
  my ($size, $MAX_ELEMENT_VALUE, $INPUT_DIR, $id) = @_;
  my @S; 

  $S[$_] = int (rand ($MAX_ELEMENT_VALUE)) foreach 1..$size;
  my $sum = 0;
  $sum += $S[$_] foreach 1..$size;
  my $t = 0;
  foreach my $i (1..$size)
  {
    if (rand() > .5)
    {
      $t += $S[$i];
    }
  }

  my $file_name = sprintf "$INPUT_DIR/Test_%03d_%04d.xml", $size, $id;
  open (XML, ">$file_name")
    or die "Cannot create '$file_name' file!\n";
  printf XML "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
  printf XML "<ElementSet>\n";
  printf XML "  <SetName> Test_%03d_%04d </SetName>\n", $size, $id;
  printf XML "  <SetValue> $t </SetValue>\n";
  printf XML "  <MaxNumberOfElementValues> 1 </MaxNumberOfElementValues>\n";
  printf XML "  <NumberOfElements> $size </NumberOfElements>\n";

  foreach (1..$size)
  {
    printf XML "  <Element>\n    <name>e$_</name>\n";
    printf XML "    <value> %d  </value>\n  </Element>", $S[$_];
  }

  printf XML "\n</ElementSet>\n";

  close (XML);
}