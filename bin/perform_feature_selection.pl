#!/usr/bin/perl -w 

#
# perform_feature_selection.pl : this program performs feature selection
#                                with the given dataset
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
my $FEATSEL_BIN = "./bin/featsel";
my $LOG_FILE = $OUTPUT_DIR . "/featsel.log";

# Arguments parsing
my $data_set_file_name;
my $number_of_features;
my $number_of_classes;
my $algorithm;
my $k = -1;
my $cost_function;
if ((@ARGV == 5) || (@ARGV == 6))
{
  $data_set_file_name = $ARGV[0];
  $number_of_features = $ARGV[1];
  $number_of_classes = $ARGV[2];
  $cost_function = $ARGV[3];
  $algorithm = $ARGV[4];
  if (@ARGV > 5)
  {
    $k = $ARGV[5];
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
  "-c $cost_function -f $data_set_file_name " .
  "-a $algorithm ";
if ($k > 0)
{
  $featsel_call_string = $featsel_call_string . "-k $k ";
}
$featsel_call_string = $featsel_call_string . "> $LOG_FILE\n";

print "calling: $featsel_call_string\n\n";

my $t0 = [gettimeofday];
system ($featsel_call_string);
my $t1 = [gettimeofday];
my $execution_time = tv_interval ($t0, $t1);

open LOG, $LOG_FILE;
while (<LOG>)
{
  print $_;
}
close (LOG);
