use strict;
use POSIX;
use lib './lib';
use Time::HiRes qw (gettimeofday tv_interval);

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
my $l_step;
my $max_time;

if (@ARGV == 3)
{
  $number_of_instances = $ARGV[0];
  $instance_size = $ARGV[1];
  $max_time = $ARGV[2];
}
else
{
  die "\nSyntax: $0 k  n  MAX_TIME \n\n";
}
print "Starting $0 program.\nExecution of $number_of_instances " .
      "instances of size $instance_size \n\n";

# Generate instances
my @instance_file;             # Store the list of instance file names.
my $MAX_ELEM_VALUE = 10000000;   # Maximum value of an element of S.
foreach my $i (1..$number_of_instances)
{
  random_subset_sum_instance ($instance_size, $MAX_ELEM_VALUE, $INPUT_DIR, $i);
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

$p_step = 1.0 / $instance_size;
$l_step = 1;
my @enqueued;
my @parameter_queue;
my @avg_err;
my $best_i = 1;
my $best_j = 1;
push @parameter_queue, [1, 1];
while (@parameter_queue)
{
  my ($i, $j) = @{shift @parameter_queue};
  my $p = $i * $p_step;
  my $l = $j * $l_step;
  my $found_min;
  printf "Testing p = %.4f and l = $l...", $p;
  $avg_err[$i][$j] = 0;
  foreach my $k (0..$number_of_instances - 1)
  {
    $found_min = 0;
    system ("timeout $max_time" . "s $FEATSEL_BIN -n $instance_size -a pucs $p $l " . 
            "-c subset_sum -f $INPUT_DIR/" . $experiment[$k] . 
            " > $LOG_FILE");
    open (LOG, $LOG_FILE);
    while (<LOG>)
    {
      if ($_ =~ /(\<\d+\>)\s+\:\s+(\S+)/)
      {
        my $minimum_found = $2;
        $avg_err[$i][$j] += $minimum_found;
        $found_min = 1;
      }
    }
    if (!$found_min)
    {
      print " [timeout].\n";
      last;
    }
    
    close(LOG);
  }  
  if ($found_min)
  {
    print " [done].\n";
    enqueue_next_parameters ($i, $j, $p_step, $l_step,
      \@enqueued, \@parameter_queue);
    $avg_err[$i][$j] /= $number_of_instances * 1.0;
    # printf "    avg error = %.2f\n", $avg_err[$i][$j];
    if ($avg_err[$best_i][$best_j] > $avg_err[$i][$j])
    {
      $best_i = $i;
      $best_j = $j;
    }
  }
}

my $best_p = $best_i * $p_step;
my $best_l = $best_j * $l_step;
printf "\nBest parameters found are p = %.4f and l = %d\n.", $best_p, $best_l;
system ("rm $INPUT_DIR/*.xml");
system ("rm $LOG_FILE");

sub enqueue_next_parameters 
{
  my ($i, $j, $p_step, $l_step, $enqueued_r, $parameter_queue_r) = @_;
  my $p = $i * $p_step;
  my $l = $j * $l_step;
  if ($p + $p_step < 0.3 && ${$enqueued_r}[$i + 1][$j] == 0)
  {
    push @{$parameter_queue_r}, [$i + 1, $j];
    ${$enqueued_r}[$i + 1][$j] = 1;
  }
  if ($l + $l_step < 5 && 
    $p + $p_step < 0.3 && ${$enqueued_r}[$i + 1][$j + 1] == 0)
  {
    push @{$parameter_queue_r}, [$i + 1, $j + 1];
    ${$enqueued_r}[$i + 1][$j + 1] = 1;
  }
  if ($l + $l_step < 5 && ${$enqueued_r}[$i][$j + 1] == 0)
  {
    push @{$parameter_queue_r}, [$i, $j + 1];
    ${$enqueued_r}[$i][$j + 1] = 1;
  }
}


sub random_subset_sum_instance
{
  my ($size, $MAX_ELEMENT_VALUE, $INPUT_DIR, $id) = @_;
  my @S; 

  $S[$_] = int (rand ($MAX_ELEMENT_VALUE)) foreach 1..$size;
  my $sum = 0;
  $sum += $S[$_] foreach 1..$size;
  my $t = 0;
  my $opt = "";
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
