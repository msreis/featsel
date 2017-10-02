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
my $m = 10;
my $MAX_NUMBER_OF_COST_FUNCTION_CALLS = 1000000;

my @ALGORITHMS = ("pucs");
  # ,
  #                  "sffs");
my %cost_function = ("pucs" => "mce", "sffs" => "mce");
my @DATA_SETS = (
                 "Iris", 
                 # "Promoters",
                 "Wine",
                 "Zoo",
                 # "Arrhythmia",
                 # "Musk2",
                 "Lung_cancer",
                 "Breast_cancer");
my %labels    = ("Iris" => 3, "Arrhythmia" => 16, "Musk2" => 2, 
                 "Promoters" => 2, "Wine" => 3, "Zoo" => 7,
                 "Lung_cancer" => 3, "Breast_cancer" => 2);
my %features = ("Iris" => 4, "Arrhythmia" => 279, "Musk2" => 166, 
                 "Promoters" => 57, "Wine" => 13, "Zoo" => 15,
                 "Lung_cancer" => 56, "Breast_cancer" => 10);
my %data_size = ("Iris" => 150, "Arrhythmia" => 452, "Musk2" => 6598,
                 "Promoters" => 106, "Wine" => 178, "Zoo" => 101,
                 "Lung_cancer" => 32, "Breast_cancer" => 699);
my %dat_file =  ("Iris" => "input/Iris/Test_01_A.dat",
                 "Arrhythmia" => "input/Arrhythmia/Test_01_A.dat",
                 "Musk2" => "input/Musk2/Test_01_A.dat",
                 "Promoters" => "input/Promoters/Test_01_A.dat",
                 "Wine" => "input/Wine/Test_01_A.dat",
                 "Zoo" => "input/Zoo/Test_15_3.dat",
                 "Lung_cancer" => "input/Lung_cancer/Test_01_A.dat",
                 "Breast_cancer" => "input/Breast_cancer/Test_01_A.dat");
  

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
