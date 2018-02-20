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
my $trn_data_set_file_name;
my $tst_data_set_file_name;
my $number_of_features;
my $number_of_classes;
my $selected_features;
my $k;
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

print ("Calculating error...\n");
my @selected_features_arr = split ('', $selected_features);

# Parses training data file
my @trn_data_set = ();
my $i = 0;
open DATA, $trn_data_set_file_name;
while (<DATA>)
{
  my @line_arr = split (' ', $_);
  my @features = @line_arr[0 .. $number_of_features - 1];
  @features = mask_on_selected_features (\@features,
   \@selected_features_arr);
  my @class = @line_arr[$number_of_features .. $#line_arr];
  $trn_data_set[$i] = {};
  $trn_data_set[$i]->{FEATURES} = \@features;
  $trn_data_set[$i]->{CLASS} = \@class;
  $i++;
}
close (DATA);

# The learning model is a NCM classifier. It's implemented as a hash 
# that stores the mean of featues of objects of the same class.
my $model_ref;
$model_ref = create_model (\@trn_data_set);

# Now we can free the @trn_data_set
undef (@trn_data_set);

# Parses testing data file
my @tst_data_set = ();
$i = 0;
open DATA, $tst_data_set_file_name;
while (<DATA>)
{
  my @line_arr = split (' ', $_);
  my @features = @line_arr[0 .. $number_of_features - 1];
  @features = mask_on_selected_features (\@features,
   \@selected_features_arr);
  my @class = @line_arr[$number_of_features .. $#line_arr];
  $tst_data_set[$i] = {};
  $tst_data_set[$i]->{FEATURES} = \@features;
  $tst_data_set[$i]->{CLASS} = \@class;
  $i++;
}
close (DATA);

# Validation
my $v_error = .0;
$v_error = ncm_validation (\@tst_data_set, $model_ref);
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


sub create_model
{
  my @trn_set = @{$_[0]};
  my %model;
  my %class_n;

  for my $sample (@trn_set)
  {
    my @features = @{$sample->{FEATURES}};
    my @class = @{$sample->{CLASS}};
    my $class_str = join ("", @class);
    if (!exists $model{$class_str})
    {
      my @feature_arr = @features;
      $model{$class_str} = \@feature_arr;
      $class_n{$class_str} = 1;
    }
    else
    {
      array_sum ($model{$class_str}, \@features);
      $class_n{$class_str} += 1;
    }
  }

  foreach my $class (keys %model)
  {
    my $mean_arr_ref = $model{$class};
    for (my $i = 0; $i < scalar @$mean_arr_ref; $i++)
    {
      $mean_arr_ref->[$i] /= $class_n{$class} * 1.0;
    }
  }

  return \%model;
}


sub ncm_validation
{
  my @tst_set = @{$_[0]};
  my %model = %{$_[1]};
  my $validation_err = 0.0;

  # Validate data
  for my $test (@tst_set)
  {
    my $min_d = -1;
    my $classification_str;
    my $test_class_str = join ("", @{$test->{CLASS}});
    for my $class_str (keys %model)
    {
      my $d2 = array_dist2 ($model{$class_str}, $test->{FEATURES});
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

  return $validation_err / scalar @tst_set;
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