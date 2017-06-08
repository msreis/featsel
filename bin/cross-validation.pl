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
if ((@ARGV == 7) || (@ARGV == 9))
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
  "data set $data_set_file_name\n";
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
    #print "$1";
  }
}
close (LOG);

my @selected_features_arr = split ('', $selected_features);
# Samples data file
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
  print ("@{$data_set[$i]->{FEATURES}}  ");
  print ("@{$data_set[$i]->{CLASS}}\n");
  $i++;
}
close (DATA);
