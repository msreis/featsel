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

use strict;
use Time::HiRes qw (gettimeofday tv_interval);
use List::MoreUtils 'pairwise';
use List::Util qw/shuffle/;
use Data::Dumper;

use lib './lib';


# Set constants
my $OUTPUT_DIR = "output";
my $LOG_FILE = $OUTPUT_DIR . "/ncm_result.log";
my $FEATSEL_BIN = "./bin/featsel";


# Arguments parsing
my $data_set_file_name;
my $number_of_features;
my $number_of_classes;
my $selected_features;
my $k;
if (@ARGV == 5)
{
  $data_set_file_name = $ARGV[0];
  $number_of_features = $ARGV[1];
  $number_of_classes = $ARGV[2];
  $k = $ARGV[3];
  $selected_features = $ARGV[4];
}
else
{
  die "\nSyntax: $0 " . 
    "DATA_SET_FILE NUMBER_OF_FEATURES NUMBER_OF_CLASSES " .
    "COST_FUNCTION_CODE NUMBER_OF_FOLDS FEATSEL_ALGORITHM_CODE \n\n" .
    "Where:\n\n" .
    "    DATA_SET_FILE: file name of data set.\n\n" .
    "    NUMBER_OF_FEATURES: number of features on the data set.\n\n" .
    "    NUMBER_OF_CLASSES: number of classes on the data set.\n\n" .
    "    NUMBER_OF_FOLDS: the number k used for a k-fold cross " .
    "validation.\n\n" . 
    "    SELECTED_FEATURES: a binary description of the selected ".
    "features";
}

print ("Calculating error with " . $k . "-Cross-validation\n");
my @selected_features_arr = split ('', $selected_features);

# Parses data file
my @data_set = ();
my $i = 0;
open DATA, $data_set_file_name;
while (<DATA>)
{
  my @line_arr = split (' ', $_);
  my @features = @line_arr[0 .. $number_of_features - 1];
  @features = mask_on_selected_features (\@features,
   \@selected_features_arr);
  my @class = @line_arr[$number_of_features .. $#line_arr];
  $data_set[$i] = {};
  $data_set[$i]->{FEATURES} = \@features;
  $data_set[$i]->{CLASS} = \@class;
  $i++;
}
close (DATA);

# k-fold Cross Validation
my $cv_error = .0;
$cv_error = ncm_cross_validation (\@data_set, $k);
print ("Cross-validation error: $cv_error\n");

sub fold_data 
{
  my @data = @{$_[0]};
  my $k = $_[1];
  my @folds;
  
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


sub ncm_cross_validation
{
  my @data = @{$_[0]};
  my $k = $_[1];
  my @folds = fold_data (\@data, $k);
  my $cv_err = 0.0;
  for (my $fold_idx = 0; $fold_idx < $k; $fold_idx++)
  {
    my ($training_set_r, $test_set_r) = split_on_fold (\@folds, 
      $fold_idx);
    my $v_err = ncm_validation ($training_set_r, $test_set_r);
    $cv_err += $v_err;
  }
  return $cv_err / $k;
}


sub ncm_validation
{
  my @training_set = @{$_[0]};
  my @test_set = @{$_[1]};
  my %class_mean;
  my %class_n;
  my $validation_err = 0.0;

  # Creates model
  for my $sample (@training_set)
  {
    my @features = @{$sample->{FEATURES}};
    my @class = @{$sample->{CLASS}};
    my $class_str = join ("", @class);
    if (!exists $class_mean{$class_str})
    {
      my @feature_arr = @features;
      $class_mean{$class_str} = \@feature_arr;
      $class_n{$class_str} = 1;
    }
    else
    {
      array_sum ($class_mean{$class_str}, \@features);
      $class_n{$class_str} += 1;
    }
  }
  foreach my $class (keys %class_mean)
  {
    my $mean_arr_ref = $class_mean{$class};
    for (my $i = 0; $i < scalar @$mean_arr_ref; $i++)
    {
      $mean_arr_ref->[$i] /= $class_n{$class} * 1.0;
    }
  }

  # Validate data
  for my $test (@test_set)
  {
    my $min_d = -1;
    my $classification_str;
    my $test_class_str = join ("", @{$test->{CLASS}});
    for my $class_str (keys %class_mean)
    {
      my $d2 = array_dist2 ($class_mean{$class_str}, $test->{FEATURES});
      if ($d2 < $min_d || $min_d  == -1)
      {
        $min_d = $d2;
        $classification_str = $class_str;
      }
    }

    if ($test_class_str ne $classification_str)
    {
      $validation_err += 1.0;
    }
  }

  return $validation_err / scalar @test_set;
}

sub array_dist2
{
  my @arr1 = @{$_[0]};
  my @arr2 = @{$_[1]};
  my $d = 0.0;
  for (my $i = 0; $i < scalar @arr1; $i++)
  {
    $d += ($arr1[$i] - $arr2[$i]) ** 2;
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


sub split_on_fold
{
  my @training_set;
  my @test_set;
  my @folds = @{$_[0]};
  my $fold_idx = $_[1];
  my $k = scalar @folds;

  for (my $i = 0; $i < $k; $i++)
  {
    if ($fold_idx == $i)
    {
      push @test_set, @{$folds[$i]};
    }
    else
    {
      push @training_set, @{$folds[$i]};
    }
  }
  return (\@training_set, \@test_set);
}
