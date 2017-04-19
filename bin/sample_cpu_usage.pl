#!/usr/bin/perl -w

# sample_cpu_usage: this script runs a command passed as an argument
# and, while running, keeps a log of cpu usage. The cpu usage data is
# transformed into a graph.

use strict;

my $OUTPUT_DIR = "./output";
my $LOG_FILE = $OUTPUT_DIR . "/cpu_usage.log";
my $OUTPUT_PREFIX = join ("", @ARGV);
$OUTPUT_PREFIX =~ s/\///g;
print $OUTPUT_PREFIX;

# Runs program and creates log
#
# Forks logger process
my $logger_pid = fork ();
if (defined $logger_pid and !$logger_pid)
{
  my $cmd = "exec mpstat -P ALL 1 > $LOG_FILE";
  exec ($cmd);
}
else
{
  # Run user command
  print "Running command: " . "@ARGV\n";
  system ("@ARGV");
  # Ends logger process
  kill 'TERM', $logger_pid;
}


# Parses log
#
my @core_load;
my $sec_tick = -1; # first header will sum 1
open (LOG, $LOG_FILE);
while (<LOG>)
{
  # Sample line from log file
  #04:09:51 PM  CPU    %usr   %nice    %sys %iowait    %irq   %soft  %steal  %guest  %gnice   %idle
  #04:09:52 PM  all    2.50    0.00    1.25    0.75    0.00    0.00    0.00    0.00    0.00   95.50
  #04:09:52 PM    0    2.00    0.00    2.00    2.00    0.00    0.00    0.00    0.00    0.00   94.00
  if ($_ =~ /\d+:\d+:\d+\s+[PM]*\s+(\d+)\s+(\d+[\.|\,]\d+\s+)+\s+(\d+[\.|\,]\d+\s+)\s*/)
  {
    my $core = $1 + 1;
    my $idle = $3;
    $idle =~ s/,/\./;
    $idle = $idle / 100;
    #print "Core: " . $core . " idle for " . $idle . "\n";
    $core_load[$sec_tick][$core] = 1.0 - $idle;
  }
  elsif ($_ =~ /[\d+:]+\d+\s[PM]*\s+CPU.+/)
  {
    #print "new sample\n";
    $sec_tick++;
  }
  elsif (/\d+:\d+:\d+\s+[PM]*\s+(all)\s+(\d+[\.|\,]\d+\s+)+\s+(\d+[\.|\,]\d+\s+)\s*/)
  {
    my $idle = $3;
    $idle =~ s/,/\./;
    $idle = $idle / 100;
    $core_load[$sec_tick][0] = 1.0 - $idle;
  } 
}
close (LOG);
#system ("rm $LOG_FILE");

if ($sec_tick <= 0)
{
  print "Sample was too short... try again with a \
    command that takes more time to be executed.\n";
  exit 0;
}

# Prints graph
#
my $DATA_FILE = $OUTPUT_DIR . "/dat.temp";
my $GNUPLOT_FILE = $OUTPUT_DIR . '/gnuplot.temp';
my @first_row =  @{$core_load[0]};
my $cpu_number = $#first_row;
open (DATA, ">$DATA_FILE");
foreach my $row (@core_load)
{
  foreach my $core (@{$row})
  {
    printf DATA "%.4f ", $core;
  }
  printf DATA "\n";
}
close (DATA);


my $xrange = max ($sec_tick * 20, 1000);
my $yrange = max ($xrange / 2, 500);

print "Printing graph...";

open (PLOT, ">$GNUPLOT_FILE");
printf PLOT "set terminal svg enhanced size $xrange, $yrange\n";
printf PLOT "set output '$OUTPUT_DIR/$OUTPUT_PREFIX.svg'\n";
printf PLOT "set border linewidth .5\n";
printf PLOT "# color definitions\n";
for my $i (1..$cpu_number + 1)
{
  my $c = color_combination ($i / ($cpu_number + 1.0));
  printf PLOT "set style line $i lc rgb '$c' lt 1 lw 2 pt 7 pi -1 ps 1\n";
}
printf PLOT "set pointintervalbox 1\n";
printf PLOT "set yrange [0:1]\n";
printf PLOT "set title \"Core usage by time\"\n";
printf PLOT "set xlabel \"Time (seconds)\" rotate parallel\n";
printf PLOT "set ylabel \"CPU use\" rotate parallel\n";
printf PLOT "plot \"$DATA_FILE\" using 0:1 title 'AVG' with linespoints ls 1, \\\n";
for my $i (2..$cpu_number + 1)
{
  my $cpu_number = $i - 2;
  printf PLOT " \"$DATA_FILE\" using 0:$i title \"CPU $cpu_number\" with linespoints ls $i, \\\n";
}
close (PLOT);
system ("gnuplot $GNUPLOT_FILE");
#system ("rm $GNUPLOT_FILE");
#system ("rm $DATA_FILE");
print "[done].\n";

# Returns a hex code (string) of a color to be used in the plot. 
# Argument should be between 0 and 1.
sub color_combination
{
  my $alpha = $_[0] * 255;
  my $R_mod = 256;
  my $G_mod = 128;
  my $B_mod = 64;
  my $c_r = int ($alpha % $R_mod);
  my $c_g = int ($alpha % $G_mod);
  my $c_b = int ($alpha % $B_mod);
  my $hex_r = sprintf ("%X", $c_r);
  my $hex_g = sprintf ("%X", $c_g);
  my $hex_b = sprintf ("%X", $c_b);
  $hex_r = fix_hex ($hex_r);
  $hex_g = fix_hex ($hex_g);
  $hex_b = fix_hex ($hex_b);
  my $hex = "#" . "$hex_r" . "$hex_g" . "$hex_b";
  $hex;
} 


sub max
{
  my ($a, $b) = @_;
  my $answ = 0;
  if ($a >= $b)
  {
    $answ = $a;
  }
  else
  {
    $answ = $b;
  }
}

sub fix_hex
{
  $a = $_[0];
  if (length ($a) == 1)
  { 
    $a = "0" . $a;
  }
  $a;
} 
