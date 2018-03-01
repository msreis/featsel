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
use Parallel::Loops;

use lib './lib';


# Set constants
my $OUTPUT_DIR = "output";
my $LOG_FILE = $OUTPUT_DIR . "/ncm_result.log";
my $FEATSEL_BIN = "./bin/featsel";


# Arguments parsing
my $trn_data_set_file_name;
my $tst_data_set_file_name;
my $number_of_features;
my $number_of_classes;
my $selected_features;
if (@ARGV == 5)
{
  $trn_data_set_file_name = $ARGV[0];
  $tst_data_set_file_name = $ARGV[1];
  $number_of_features = $ARGV[2];
  $number_of_classes = $ARGV[3];
  $selected_features = $ARGV[4];
}
else
{
  die "\nSyntax: $0 " . 
    "TRAINING_DATA_SET_FILE TESTING_DATA_SET_FILE NUMBER_OF_FEATURES ".
    "NUMBER_OF_CLASSES SELECTED_FEATURES\n\n" .
    "Where:\n\n" .
    "    TRAINING_DATA_SET_FILE: file name of data set used for ".  
    "training.\n\n" .
    "    TESTING_DATA_SET_FILE: file name of data set used for testing". 
    " \n\n" .
    "    NUMBER_OF_FEATURES: number of features on the data set.\n\n" .
    "    NUMBER_OF_CLASSES: number of classes on the data set.\n\n" .
    "    SELECTED_FEATURES: a binary description of the selected ".
    "features";
}


my @selected_features_arr = split ('', $selected_features);

# Reads training data file and creates model
print "Reading training data and creating model...";
my @model;
my @class_n;
my $i = 0;
open DATA, $trn_data_set_file_name;
while (<DATA>)
{
  # Reads training data sample
  my @line_arr = split (' ', $_);
  my @features = @line_arr[0 .. $number_of_features - 1];
  @features = mask_on_selected_features (\@features,
   \@selected_features_arr);
  my @class = @line_arr[$number_of_features .. $#line_arr];
  
  # Updates model
  for my $l (0 .. (scalar @class - 1))
  {
    if ($class[$l] != 0)
    {
      if (!defined $model[$l])
      {
        weigh_array (\@features, $class[$l]);
        $model[$l] = \@features;
        $class_n[$l] = $class[$l];
      }
      else
      {
        weigh_array (\@features, $class[$l]);
        array_sum ($model[$l], \@features);
        $class_n[$l] += $class[$l];
      }
    }
  }
}
close (DATA);

# Divides class array to get an average characteristic vector
foreach my $l (0 .. (scalar @model - 1))
{
  if (defined $model[$l])
  {
    my $mean_arr_ref = $model[$l];
    for (my $i = 0; $i < scalar @$mean_arr_ref; $i++)
    {
      $mean_arr_ref->[$i] /= $class_n[$l] * 1.0;
    }
  }
}
print "[DONE].\n";
undef (@class_n);



# Parses testing data file and calculates validation error
print "Reading testing data and validating model...";
my ($sample_err, $sample_card);
my $k;
my $v_error = .0;
my $test_card = 0;
$i = 0;

open DATA, $tst_data_set_file_name;
my @data_buffer = ([], []);
while (<DATA>)
{
  my @line_arr = split (' ', $_);
  my @features = @line_arr[0 .. $number_of_features - 1];
  @features = mask_on_selected_features (\@features,
    \@selected_features_arr);
  my @class = @line_arr[$number_of_features .. $#line_arr];

  push @{$data_buffer[0]}, \@features;
  push @{$data_buffer[1]}, \@class;
  $i += 1;
  
  
  if ($i % 500 == 0)
  {
    print "\n$i testing samples read; now testing it...";
    
    my $pl = Parallel::Loops->new (16);
    my @processing_blocks = (0 .. 4);
    my %return_values;
    $pl->share (\%return_values);
    $pl->foreach (\@processing_blocks, sub 
    {
      my $block = $_;
      my $block_v_error = 0;
      my $block_test_card = 0;
      for my $j (0 .. 99)
      {
        my $idx = $block * 100 + $j;
        @features = @{$data_buffer[0]->[$idx]};
        @class = @{$data_buffer[1]->[$idx]};
        ($sample_err, $sample_card) = ncm_validate_sample (\@model, 
          \@features, \@class);
        $block_v_error += $sample_err;
        $block_test_card += $sample_card;
      }
      $return_values{$block} = [$block_v_error, $block_test_card];
    });

    undef $data_buffer[0];
    undef $data_buffer[1];
    for my $block (@processing_blocks)
    {
      $v_error += $return_values{$block}->[0];
      $test_card += $return_values{$block}->[1];
    }
    @data_buffer = ([], []);
  }
}
close (DATA);
print "\n$i testing samples read; now testing it...";

for my $j (1 .. scalar @{$data_buffer[0]})
{
  my @features = @{pop @{$data_buffer[0]}};
  my @class = @{pop @{$data_buffer[1]}};
  ($sample_err, $sample_card) = ncm_validate_sample (\@model, 
    \@features, \@class);
  $v_error += $sample_err;
  $test_card += $sample_card; 
}

$v_error /= $test_card;
print "[DONE].\n";

# Validation
print ("validation error: $v_error\n");


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


sub ncm_validate_sample
{
  my $model_ref = $_[0];
  my $test_feature_ref = $_[1];
  my $test_class_ref = $_[2];
  my $min_d;
  my $classification_l;
  my $test_card = array_elm_sum ($test_class_ref);

  my $l = 0;
  $l += 1 while (not defined $model_ref->[$l]);
  $min_d = array_dist2 ($model_ref->[$l], $test_feature_ref);
  $classification_l = $l;
    
  for $l ($classification_l .. (scalar @{$model_ref} - 1))
  {
    if (defined $model_ref->[$l])
    {
      my $d2 = bounded_array_dist2 ($model_ref->[$l], $test_feature_ref,
        $min_d);
      if ($d2 < $min_d)
      {
        $min_d = $d2;
        $classification_l = $l;
      }
    }
  }

  return ($test_card - $test_class_ref->[$classification_l], 
      $test_card);
}


sub array_dist2
{
  my $arr1 = $_[0];
  my $arr2 = $_[1];
  my $d = 0.0;
  for (my $i = 0; $i < scalar @{$arr1}; $i++)
  {
    $d += ($arr1->[$i] - $arr2->[$i]) ** 2;
  }
  return $d;
}


sub bounded_array_dist2
{
  my $arr1 = $_[0];
  my $arr2 = $_[1];
  my $bound = $_[2];
  my $d = 0.0;
  for (my $i = 0; $i < scalar @{$arr1}; $i++)
  {
    $d += ($arr1->[$i] - $arr2->[$i]) ** 2;
    if ($d > $bound)
    {
      last;
    }
  }
  return $d;
}


sub array_sum
{
  my $arr1 = $_[0];
  my $arr2 = $_[1];
  for (my $i = 0; $i < scalar @$arr1; $i++)
  {
    $arr1->[$i] = $arr1->[$i] + $arr2->[$i];
  }
}


sub array_elm_sum
{
  my $arr = $_[0];
  my $acc = 0;
  for (my $i = 0; $i < scalar @$arr; $i++)
  {
    $acc += $arr->[$i];
  }
  return $acc;
}


sub weigh_array
{
  my $arr = $_[0];
  my $const = $_[1];
  for (my $i = 0; $i < scalar @$arr; $i++)
  {
    $arr->[$i] *= $const;
  }
}
