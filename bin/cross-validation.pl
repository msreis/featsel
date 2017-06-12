#!/usr/bin/perl -w 

#
# build_featsel_main_file.pl : program to generate the featsel main file taking
#                              into account the current set of available 
#                              algorithms and cost functions.
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

use strict;
use List::MoreUtils 'pairwise';
use List::Util qw/shuffle/;
use Data::Dumper;

use lib './lib';


# Set constants
my $OUTPUT_DIR = "output";
my $LOG_FILE = $OUTPUT_DIR . "/result.log";
my $FEATSEL_BIN = "./bin/featsel";


# Arguments parsing
my $data_set_file_name;
my $cost_function;
my $algorithm;
my $pucs_p = "";
my $pucs_l = "";
my $number_of_features;
my $number_of_classes;
if ((@ARGV == 5) || (@ARGV == 7))
{
  $data_set_file_name = $ARGV[0];
  $number_of_features = $ARGV[1];
  $number_of_classes = $ARGV[2];
  $cost_function = $ARGV[3];
  $algorithm = $ARGV[4];
  if ((@ARGV) > 5)
  {
    $pucs_p = $ARGV[5];
    $pucs_l = $ARGV[6];
  }
}
else
{
  die "\nSyntax: $0 " . 
    "DATA_SET_FILE NUMBER_OF_FEATURES NUMBER_OF_CLASSES " .
    "COST_FUNCTION_CODE FEATSEL_ALGORITHM_CODE \n\n" .
    "Where:\n\n" .
    "    DATA_SET_FILE: file name of data set.\n\n" .
    "    NUMBER_OF_FEATURES: number of features on the data set.\n\n" .
    "    NUMBER_OF_CLASSES: number of classes on the data set.\n\n" .
    "    COST_FUNCTION_CODE: the cost function used on the feature " .
    "selection.\n\n" . 
    "    FEATSEL_ALGORITHM_CODE: algorithm code used to select " . 
    "features from the data set.\n\n";
}


# Runs featsel algorithm to get subset of features
my $selected_features = "";
print "Starting algorithm $algorithm to select features on the " . 
  "data set $data_set_file_name... ";
my $featsel_call_string = "$FEATSEL_BIN " .
  "-n $number_of_features -l $number_of_classes " .
  "-a $algorithm $pucs_p $pucs_l " .
  "-c $cost_function -f $data_set_file_name " .
  " > $LOG_FILE\n";
system ($featsel_call_string);
open LOG, $LOG_FILE;
while (<LOG>)
{
  if ($_ =~ /\<(\d+)\>\s+\:\s+(\S+)/)
  {
    $selected_features = $1;
  }
}
close (LOG);
print ("[Done!]\n");
print ("Selected Features: $selected_features\n");
my @selected_features_arr = split ('', $selected_features);


# Parses data file
my @data_set = ();
my $i = 0;
open DATA, $data_set_file_name;
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


# 10-fold Cross Validation
my @folds = fold_data (\@data_set);
my $cv_error = 0;
# For each fold we should calculate in sample error
for (my $i = 0; $i < 10; $i++)
{
  # Samples with all folds except fold i
  my @training_set = ();
  for (my $j = 0; $j < 10; $j++)
  {
    if ($j != $i)
    {
      push @training_set, @{$folds[$j]};
    }
  }
  my %hypothesis = create_hypothesis (\@training_set, 
    \@selected_features_arr);

  # Validates with fold i
  my @validation_set = @{$folds[$i]};
  my $validation_error = validation_error (\@validation_set, 
    \@selected_features_arr, \%hypothesis);
  $cv_error += $validation_error;
}
$cv_error /= 10;
print ("Cross-validation error: $cv_error\n");




# Given a training set, returns a hypothesis
#
# Training set should be a reference to an array of hashes containing 
# two keys:
#  - FEATURES
#  - CLASS
#
#  Return value is a reference to a hash containing as key strings
#  representing subsets of features. The value should be the class
#  infered from that subset of features.
#
sub create_hypothesis
{
  my $training_set_ref = $_[0];
  my @selected_features = @{$_[1]};
  my %sample;

  #Samples training data set 
  for my $data (@$training_set_ref)
  {
    my @features = @{$data->{FEATURES}};
    my @class = @{$data->{CLASS}};
    my @considered_features = mask_on_selected_features (\@features,
      \@selected_features);
    my $features_key = join ("", @considered_features);
    my $class_key = join ("", @class);

    if (not exists $sample{$features_key})
    {
      my %labels;
      $sample{$features_key} = \%labels;
    }

    ${$sample{$features_key}}{$class_key} += 1;
  }

  #print Dumper (\%sample);

  # Creates hypothesis
  my %hypothesis;
  foreach my $feature (keys %sample) 
  {
    my %class_occurrences = %{$sample{$feature}};
    my $max_occur = 0;
    my $infered_class;
    foreach my $class (keys %class_occurrences)
    {
      if ($class_occurrences{$class} > $max_occur)
      {
        $max_occur = $class_occurrences{$class};
        $infered_class = $class; 
      }
    }
    $hypothesis{$feature} = $infered_class;
  }
  return %hypothesis; 
}



# Given a validation set, hypothesis and selected_features, calculates
# validation error
#
# Validation set should be a reference to an array of hashes containing 
# two keys:
#  - FEATURES
#  - CLASS
#
sub validation_error
{
  my $validation_set_ref = $_[0];
  my @selected_features = @{$_[1]};
  my %hypothesis = %{$_[2]};
  my $validation_error = .0;
 

  for my $data (@$validation_set_ref)
  {
    my @features = @{$data->{FEATURES}};
    my @class = @{$data->{CLASS}};
    my @considered_features = mask_on_selected_features (\@features,
      \@selected_features);
    my $features_key = join ("", @considered_features);
    my $class_key = join ("", @class);
    # If the hypothesis has a don't care we will consider it as a
    # classification error
    if (defined $hypothesis{$features_key})
    {
      my $classified_class = $hypothesis{$features_key};
      $validation_error += not $classified_class eq $class_key;    
    }
    else
    {
      $validation_error += 1;
    }
  }

  $validation_error /= scalar @$validation_set_ref;
  return $validation_error;
}


sub fold_data 
{
  my @data = @{$_[0]};
  my @folds;
  my $k = 10;
  my $n = int (scalar @data / $k);
  my $remainder = scalar @data % $k;
  @data = shuffle (@data);
  for (my $i = 0; $i < $k; $i++)
  {
    my $a = $i * $n;
    my $b = ($i + 1) * $n - 1;
    my @fold = @data[$a .. $b];
    $folds[$i] = \@fold;
  }
  for (my $i = 0; $i < $remainder; $i++) 
  {
    my @fold = @{$folds[$i]};
    push @fold, $data[$n * $k + $i];
    $folds[$i] = \@fold;
  }
  my $i = 0;
  #for my $fold_r (@folds)
  #{
    #my @fold = @{$fold_r};
    #print ("\n\nfold $i:\n @fold");
    #$i++;
  #}
  return @folds;
}


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
