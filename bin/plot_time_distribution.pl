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
my $p;
my $l;

if (@ARGV == 5)
{
  $output_file_prefix = $ARGV[0];
  $number_of_instances = $ARGV[1];
  $instance_size = $ARGV[2];
  $p = $ARGV[3];
  $l = $ARGV[4];
}
else
{
  die "\nSyntax: $0 " . 
  " OUTPUT_FILE_PREFIX  k  n p l  \n\n";
}
print "Starting $0 program.\nExecution of $number_of_instances " .
      "instances of size $instance_size \n".
      "    Parameter p is $p\n".
      "    Parameter l is $l\n\n";

# Generate instances
my @instance_file;              # Store the list of instance file names.
my $MAX_ELEM_VALUE = 10000000;  # Maximum value of an element of S.
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


# The dictionary times stores samples of time spent in every part solved
# in the pucs algorithm. The samples are composed of two time values:
# 1) The time spent walking;
# 2) The time spent waiting for child tasks
# 3) The ttime spent cleaning the minima list
# 4) The level of the solved part.
my @avg_time_walking;
my @avg_time_waiting;
my @avg_time_cleaning;
my @parts_of_level;
foreach my $i (0..$number_of_instances - 1)
{
  # print "running: " . "$FEATSEL_BIN -n $instance_size -a pucs $p $l " . 
  #         "-c subset_sum -f $INPUT_DIR/" . $experiment[$i] . 
  #         " > $LOG_FILE\n";
  system ("$FEATSEL_BIN -n $instance_size -a pucs $p $l " . 
          "-c subset_sum -f $INPUT_DIR/" . $experiment[$i] .
          " > $LOG_FILE");
  open (LOG, $LOG_FILE);
  while (<LOG>)
  {
    if ($_ =~ /\[(0[x|X][\d|a-f]+)\]\sti\s=\s(\d+);\std\s=\s(\d+);\stf\s=\s(\d+);\sl\s=\s(\d+)/)
    {
      my $level = $5 - 1;
      my $part = $1;
      my $time_walking = $2;
      my $time_waiting = $3;
      my $time_cleaning = $4;
      $avg_time_walking[$level] += $2;
      $avg_time_waiting[$level] += $3;
      $avg_time_cleaning[$level] += $4;
      $parts_of_level[$level]++;
      print "Captured [$part] -> $time_walking, $time_waiting, $time_cleaning, $level\n";
    }
  }
  close(LOG);
}
my $levels = $#parts_of_level + 1;
for my $l (0..$levels - 1)
{
  #print "Level $l ---------------\n";
  #print "parts = $parts_of_level[$l]\n";
  $avg_time_walking[$l] /= $parts_of_level[$l];
  $avg_time_walking[$l] /= 1e6;
  $avg_time_waiting[$l] /= $parts_of_level[$l];
  $avg_time_waiting[$l] /= 1e6;
  $avg_time_cleaning[$l] /= $parts_of_level[$l];
  $avg_time_cleaning[$l] /= 1e6;
  #print "cleaning $avg_time_cleaning[$l]\n";
  #print "waiting $avg_time_waiting[$l]\n";
  #print "walking $avg_time_walking[$l]\n";
}


# Creates graph data
my $GNUPLOT_DATA_FILE = $OUTPUT_DIR . "/dat.temp";
my $GNUPLOT_PLOT_FILE = $OUTPUT_DIR . "/gnuplot.temp";
open (DATA, ">$GNUPLOT_DATA_FILE");
for my $l (0..$levels - 1)
{
  printf DATA "%d %.4f %.4f %.4f\n", $parts_of_level[$l], $avg_time_walking[$l], 
    $avg_time_waiting[$l], $avg_time_cleaning[$l];
}
close (DATA);

#Creates time graph
open (PLOT, ">$GNUPLOT_PLOT_FILE");
printf PLOT "set terminal svg enhanced size 700, 500\n";
printf PLOT "set output '$OUTPUT_DIR/$output_file_prefix\_time_distribution.svg'\n";
printf PLOT "set border linewidth .5\n";
printf PLOT "set style line 1 lc rgb '#6699cc' lt 1 lw 2 pt 7 pi -1 " . 
 "ps 1\n";
printf PLOT "set style line 2 lc rgb '#0e8466' lt 1 lw 2 pt 7 pi -1 " .
 "ps 1\n";
printf PLOT "set style line 3 lc rgb '#c8b03e' lt 1 lw 2 pt 7 pi -1 " .
 "ps 1\n";
printf PLOT "set style line 4 lc rgb '#550033' lt 1 lw 2 pt 7 pi -1 " .
 "ps 1\n";
print PLOT "set pointintervalbox 1\n";
#printf PLOT "unset key\n";
printf PLOT "set title 'Time distribution on PUCS algorithm.' \n";
printf PLOT "set xtics 1\n";
printf PLOT "set ytics nomirror\n";
printf PLOT "set xlabel 'Recursion level'\n";
printf PLOT "set ylabel 'Time (s)'\n";
printf PLOT "set y2label 'Number of parts'\n";
print  PLOT "set y2tics 1\n";
printf PLOT "plot " . 
"'$GNUPLOT_DATA_FILE' using (\$0 + 1):2 title 'Walking' with linespoints ls 1" .
",\\\n ".
"'$GNUPLOT_DATA_FILE' using (\$0 + 1):3 title 'Waiting' with linespoints ls 2" .
",\\\n ".
"'$GNUPLOT_DATA_FILE' using (\$0 + 1):4 title 'Cleaning' with" . 
" linespoints ls 3,\\\n".
"'$GNUPLOT_DATA_FILE' using (\$0 + 1):1 title 'Number of parts' with " .
"linespoints ls 4 axes x1y2\n";

close (PLOT);

#Execute Gnuplot.
system ("gnuplot $GNUPLOT_PLOT_FILE");

#system ("rm $GNUPLOT_PLOT_FILE");

system ("rm $INPUT_DIR/*.xml");
print "[done]\n";


sub divide_arr
{
  my $arr_r = $_[0];
  my $const = $_[1];
  my @arr = @{$arr_r};
  foreach my $x (@arr) 
  {
    $x = $x / ($const * 1.0); 
  }
}
