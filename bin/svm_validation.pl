#!/usr/bin/perl -w 

#
# ncm_cross_validation.pl : this program performs a k-fold cross 
#                           validation with the given algorithm and
#                           dataset.
#
#    This file is part of the featsel program
#    Copyright (C) 2017  Gustavo Estrela and Marcelo S. Reis
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

use warnings;
use strict;
use Time::HiRes qw (gettimeofday tv_interval);
use List::MoreUtils 'pairwise';
use List::Util qw/shuffle/;
use Data::Dumper;


# Set constants
my $OUTPUT_DIR = "output";
my $LIBSVM_DIR = "/home/gestrela/projects/libsvm-3.22/";
my $FEATSEL_BIN = "./bin/featsel";
my $PROCESS_PID = $$;
my $TRN_TMP_FILE = $OUTPUT_DIR . "/trn_tmp_" . $PROCESS_PID . ".dat";
my $TST_TMP_FILE = $OUTPUT_DIR . "/tst_tmp_" . $PROCESS_PID . ".dat";
my $MODEL_TMP_FILE = $OUTPUT_DIR . "/model_tmp_" . $PROCESS_PID . ".dat";
my $TRN_LOG_FILE = $OUTPUT_DIR . "/trn_log_" . $PROCESS_PID . ".txt";
my $TRN_OUTPUT_FILE = $OUTPUT_DIR . "/trn_result_" . $PROCESS_PID . ".txt";

# Arguments parsing
my $trn_data_set_file_name;
my $tst_data_set_file_name;
my $number_of_features;
my $selected_features;
if (@ARGV == 4)
{
  $trn_data_set_file_name = $ARGV[0];
  $tst_data_set_file_name = $ARGV[1];
  $number_of_features = $ARGV[2];
  $selected_features = $ARGV[3];
}
else
{
  die "\nSyntax: $0 " . 
    "TRAINING_DATA_SET_FILE TESTING_DATA_SET_FILE NUMBER_OF_FEATURES ".
    "SELECTED_FEATURES\n\n" .
    "Where:\n\n" .
    "    TRAINING_DATA_SET_FILE: file name of data set used for ".  
    "training.\n\n" .
    "    TESTING_DATA_SET_FILE: file name of data set used for testing". 
    " \n\n" .
    "    NUMBER_OF_FEATURES: number of features on the data set.\n\n" .
    "    SELECTED_FEATURES: a binary description of the selected ".
    "features";
}
my @selected_features_arr = split ('', $selected_features);

# Parses training data file
my @data_set = ();
my $i = 0;
open DATA, $trn_data_set_file_name;
while (<DATA>)
{
  my @line_arr = split (' ', $_);
  my @features = @line_arr[0 .. $number_of_features - 1];
  my @class = @line_arr[$number_of_features .. $#line_arr];
  $data_set[$i] = {};
  $data_set[$i]->{FEATURES} = \@features;
  $data_set[$i]->{CLASS} = \@class;
  $i++;
}
close (DATA);
create_libsvm_file ($TRN_TMP_FILE, \@data_set, \@selected_features_arr);

# Creates svm model
my $libsvmcallstr = $LIBSVM_DIR . "svm-train -q -s 0 -t 0 -c 100 " .
  $TRN_TMP_FILE . " " . $MODEL_TMP_FILE;
system ($libsvmcallstr);

# Parses testing data file
@data_set = (); 
$i = 0;
open DATA, $tst_data_set_file_name;
while (<DATA>)
{
  my @line_arr = split (' ', $_);
  my @features = @line_arr[0 .. $number_of_features - 1];
  my @class = @line_arr[$number_of_features .. $#line_arr];
  $data_set[$i] = {};
  $data_set[$i]->{FEATURES} = \@features;
  $data_set[$i]->{CLASS} = \@class;
  $i++;
}
close (DATA);
create_libsvm_file ($TST_TMP_FILE, \@data_set, \@selected_features_arr);

# Validates
my $v_error = -1;
$libsvmcallstr = $LIBSVM_DIR . "svm-predict " . $TST_TMP_FILE . 
  " " . $MODEL_TMP_FILE . " " . $TRN_LOG_FILE . " > " . $TRN_OUTPUT_FILE;
system ($libsvmcallstr);
open DATA, $TRN_OUTPUT_FILE;
while (<DATA>)
{
  if ($_ =~ /\S+\s+=\s+(\d+\.?\d*)%.*/)
  {
    $v_error = 1 - $1 / 100;
  }
}
close (DATA);
print ("validation error: $v_error\n");

# Remove temporary files
system ("rm $TRN_TMP_FILE");
system ("rm $TST_TMP_FILE");
system ("rm $MODEL_TMP_FILE");
system ("rm $TRN_LOG_FILE");
system ("rm $TRN_OUTPUT_FILE");


## Functions 
sub mask_on_selected_features
{
  my @features = @{$_[0]};
  my @selected_features = @{$_[1]};
  my @masked_features;
  @masked_features = pairwise {$a * $b} @features, @selected_features;
  return @masked_features;
}


sub class_arr_to_int
{
  my @class_arr = @_;
  my $class_number = 0;
  my $multiplier = 1;
  for my $digit (reverse (@class_arr))
  {
    $class_number += $digit * $multiplier;
    $multiplier *= 2;    
  }
  return $class_number;
}


sub create_libsvm_file
{
  my $file_name = "./" . $_[0];
  my @data = @{$_[1]};
  my @sel_feat = @{$_[2]};

  open (DATA, ">$file_name");
  for my $sample (@data)
  {
    my @attr_arr = @{$sample->{FEATURES}};
    my @label_arr = @{$sample->{CLASS}};
    @attr_arr = mask_on_selected_features (\@attr_arr, \@sel_feat);
    my $label = class_arr_to_int (@label_arr);

    printf DATA "$label ";
    foreach my $i (0..$#attr_arr)
    {
      printf DATA  ($i + 1) . ":$attr_arr[$i] ";
    }
    printf DATA "\n";
  }
  close (DATA);
}
