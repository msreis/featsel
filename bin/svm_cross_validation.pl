#!/usr/bin/perl -w 

#
# svm_cross_validation.pl : this program performs a k-fold cross 
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
my $LOG_FILE = $OUTPUT_DIR . "/svm_result.log";
my $FEATSEL_BIN = "./bin/featsel";
my $LIBSVM_DIR = "/home/gustavo/cs/libsvm/libsvm-3.22/";


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
    "NUMBER_OF_FOLDS SELECTED_FEATURES \n\n" .
    "Where:\n\n" .
    "    DATA_SET_FILE: file name of data set.\n\n" .
    "    NUMBER_OF_FEATURES: number of features on the data set.\n\n" .
    "    NUMBER_OF_CLASSES: number of classes on the data set.\n\n" .
    "    NUMBER_OF_FOLDS: the number k used for a k-fold cross " .
    "validation.\n\n" . 
    "    SELECTED_FEATURES: a binary description of the selected ".
    "features"
}

print ("Calculating error with " . $k . "-fold Cross-validation\n");
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

# k-fold Cross Validation
my $cv_error = .0;
my $tf = "data_set.txt";
create_libsvm_file ($tf, \@data_set, \@selected_features_arr);
my $libsvmcallstr = $LIBSVM_DIR . "svm-train -s 0 -t 0 -c 100 -v $k " .
$tf . " > $LOG_FILE\n";

system ($libsvmcallstr);    
open LOG, $LOG_FILE;
while (<LOG>)
{
  if ($_ =~ /.*=\s(\d+(\.)*\d*)%.*/)
  {
    my $accuracy = $1;
    $cv_error = (100.0 - $accuracy) / 100.0;
    $cv_error = sprintf("%.3f", $cv_error);
  }
}
close (LOG);
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
