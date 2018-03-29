#!/usr/bin/perl -w

#
# compare_with_linear_svm.pl : this program compares solutions given by 
# feature selection algorithms by performing a cross-validation method
# with SVMs modeled with those features.
#
#    This file is part of the featsel program
#    Copyright (C) 2017  Marcelo S. Reis, Gustavo Estrela
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

# Main definitions
my $FEATSEL_BIN    = "./bin/featsel";
my $LOG_FILE       = "output/result.log";
my $INPUT_DIR      = "input/";
# Number of repetitions on runs over a dataset
my $m = 3;
my $MAX_NUMBER_OF_COST_FUNCTION_CALLS = 1000000;

my @ALGORITHMS = (
  "pucs",
  "ubb",
  #"sffs",
  "spec_cmi");
my %cost_function = ("pucs" => "mi",
  "ubb" => "mce",
  "sffs" => "mi",
  "spec_cmi" => "cmi");
my @DATA_SETS = (
    #"Iris", 
    #"Car", 
    #"Wine", 
    #"Zoo", 
    #"Waveform", 
    "Parity1", 
    "Parity2", 
    "Parity3");
my %labels    = ("Iris" => 3, "Car" => 4, "Zoo" => 7,  "Wine" => 3,
  "Waveform" => 3, "Parity1" => 2, "Parity2" => 2, "Parity3" => 2);
my %features =  ("Iris" => 4, "Car" => 6, "Zoo" => 15, "Wine" => 13,
  "Waveform" => 21, "Parity1" => 12, "Parity2" => 12, "Parity3" => 12);
my %data_size = ("Iris" => 150, "Car" => 1728, "Wine" => 178, 
  "Zoo" => 101, "Waveform" => 5000, "Parity1" => 1000, "Parity2" => 1000, 
  "Parity3" => 1000);
my %dat_file =  ("Iris" => "input/Iris/Test_01_A.dat",
  "Car" => "input/Car/Test_01_A.dat",
  "Wine" => "input/Wine/Test_01_A.dat",
  "Zoo" => "input/Zoo/Test_15_3.dat",
  "Waveform" => "input/Waveform/Test_01_A.dat",
  "Parity2" => "input/Parity/Test_parity_12_4_1000.dat", 
  "Parity1" => "input/Parity/Test_parity_12_6_1000.dat", 
  "Parity3" => "input/Parity/Test_parity_12_8_1000.dat"
);

my $DATA_FILE = "output/dat.temp";
open (DATA, ">$DATA_FILE");
foreach my $data_set (@DATA_SETS)
{
  foreach my $algorithm (@ALGORITHMS)
  {
    my $cv_error = -1;
    my $execution_time = 0;
    foreach my $i (1..$m)
    {
      my $k;
      if ($data_size{$data_set} > 100)
      {
        $k = 10;
      }
      else
      {
        $k = $data_size{$data_set};
      }

      if ($algorithm ne "es" or $features{$data_set} < 21)
      {
        # Performs feature selection
        my $selected_features;
        my ($t0, $t1);
        $t0 = [gettimeofday];
        system ("bin/perform_feature_selection.pl " .
          "$dat_file{$data_set} $features{$data_set} " .
          "$labels{$data_set} $cost_function{$algorithm} " .
          " $algorithm > $LOG_FILE");
        $t1 = [gettimeofday];
        $execution_time = tv_interval ($t0, $t1);
        open LOG, $LOG_FILE;
        while (<LOG>)
        {
          if ($_ =~ /\<(\d+)\>\s+\:\s+(\S+)/)
          {
            $selected_features = $1;
          }
        }
        close (LOG);

        # Performs cross-validation
        system ("./bin/svm_cross_validation.pl " .
          "$dat_file{$data_set} $features{$data_set} " .
          "$labels{$data_set} $k $selected_features > $LOG_FILE");
        open LOG, $LOG_FILE;
        while (<LOG>)
        {
          if ($_ =~ /Cross-validation\serror:\s(0.\d+)/)
          {
            $cv_error = $1;
          }
        }
        close (LOG);

        if ($cv_error == -1)
        {
          die "\nCould not perform cross validation on $data_set with " . 
          "model with features selected by $algorithm\n\n";
        }
      }

      printf DATA "\n%2d-run on %8s with %8s takes " .
      "%6.3f and has cross-validation error of %4.3f ",
      $i, $data_set, $algorithm, $execution_time, $cv_error;

      printf "\n%2d-run on %8s with %8s takes " .
      "%6.3f and has cross-validation error of %4.3f ",
      $i, $data_set, $algorithm, $execution_time, $cv_error;
    }
  }
}
close (DATA);
