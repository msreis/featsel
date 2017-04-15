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


# The dictionary times stores samples of time spent in every part solved
# in the pucs algorithm. The samples are composed of two time values:
# 1) The time spent walking and composing a solution, which is not
# parallelizable;
# 2) The total time spent;
# 3) The level of the solved part.
my %times;
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
    if ($_ =~ /\[(0[x|X][0-9|a-f|A-F]+)\]\sTime\sspent\swalking\son\slevel\s<(\d+)>:\s(\d+)/)
    {
      my $part = $1;
      my $time_walking = $3;
      my $level = $2;
      my @entry = ($time_walking, undef, $level);
      $times{$part} = \@entry;
      # print "Captured [$part] -> $time_walking, , $level\n";
    }
    elsif ($_ =~/\[(0[x|X][0-9|a-f|A-F]+)\]\sTotal\sprogram\stime\sof\slevel\s<(\d+)>:\s(\d+)/)
    {
      my $part = $1;
      my $total_time = $3;
      my $entry = $times{$part};
      $entry->[1] = $total_time;
    }
  }
  close(LOG);
}
my $total_parts = keys %times;

# For each level l, stores the percentage of time that any part p 
# of level l spent walking.
my @parts_of_level;
my @lv_walking_p;
foreach my $key (keys %times)
{
  my @a = @{$times{$key}};
  # print "times[$key] = ($a[0], $a[1], $a[2])\n";
  push @{$lv_walking_p[$a[2] - 1]}, ($a[0] / $a[1] * 1.0);
  $parts_of_level[$a[2] - 1] = $parts_of_level[$a[2] - 1] + 1;
}
# Creates graph data
my $GNUPLOT_DATA_FILE = $OUTPUT_DIR . "/dat.temp";
my $GNUPLOT_PLOT_FILE = $OUTPUT_DIR . "/gnuplot.temp";
my $line = 0;
my $levels = @lv_walking_p;
open (DATA, ">$GNUPLOT_DATA_FILE");
while ($line < $total_parts) 
{
  for my $l (0..$levels - 1)
  {
    my $p = $lv_walking_p[$l]->[$line];
    if ($p)
    {
      printf DATA "%.4f ", $lv_walking_p[$l]->[$line];
    }
  }
  printf DATA "\n";
  $line++;
}
close (DATA);

# Creates time graph
open (PLOT, ">$GNUPLOT_PLOT_FILE");
printf PLOT "set terminal svg enhanced size 700, 500\n";
printf PLOT "set output '$OUTPUT_DIR/$output_file_prefix\_time_distribution.svg'\n";
printf PLOT "set style fill solid 0.25 border -1\n";
printf PLOT "set style boxplot outliers pointtype 7\n";
printf PLOT "set style data boxplot\n";
printf PLOT "set boxwidth  0.5\n";
printf PLOT "set pointsize 0.5\n";
printf PLOT "unset key\n";
printf PLOT "set xtics (";
for my $i (0..$levels - 1)
{
  my $level = $i + 1;
  printf PLOT "'Level $level; N = $parts_of_level[$i]' $level, "
}
printf PLOT ") scale 0.0\n";
printf PLOT "set xtics nomirror\n";
printf PLOT "set ytics nomirror\n";
printf PLOT "set yrange [0:1]\n";
printf PLOT "set title 'Percentage of time spent walking on the poset.' \n";
printf PLOT "plot ";
for my $i (0..$levels - 1)
{
  my $level = $i + 1;
  printf PLOT "'$GNUPLOT_DATA_FILE' using ($level):$level, ";
}
print PLOT "\n";
close (PLOT);
# Execute Gnuplot.
system ("gnuplot $GNUPLOT_PLOT_FILE");

# system ("rm $GNUPLOT_PLOT_FILE");

system ("rm $INPUT_DIR/*.xml");
print "[done]\n";