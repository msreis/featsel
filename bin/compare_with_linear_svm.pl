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
use Time::HiRes qw (gettimeofday tv_interval)

# Main definitions
my $FEATSEL_BIN    = "./bin/featsel";
my $LOG            = "output/result.log";
my $INPUT_DIR      = "input/";
# Number of repetitions on runs over a dataset
my $m = 20;
my $MAX_NUMBER_OF_COST_FUNCTION_CALLS = 1000000;

my @ALGORITHMS = ("pucs",
          "es",
          "sffs",
          "spec_cmi");
my %cost_function ("pucs" => "mce",
           "es" => "mce",
           "sffs" => "mce",
           "spec_cmi" => "cmi");

my @DATA_SETS = ("Iris", "Car", "Wine", "Zoo", "Waveform");
my %labels    = ("Iris" => 3, "Car" => 4, "Zoo" => 7,  "Wine" => 3,
         "Waveform" => 3);
my %features =  ("Iris" => 4, "Car" => 6, "Zoo" => 15, "Wine" => 13,
         "Waveform" => 21);
my %dat_file =  ("Iris" => "input/Iris/Test_01_A.dat",
         "Car"  => "input/Car/Test_01_A.dat",
         "Wine" => "input/Wine/Test_01_A.dat",
         "Zoo"  => "input/Zoo/Test_15_3.dat");

foreach my $data_set (@DATA_SETS)
{
  foreach my $algorithm (@ALGORITHMS)
  {
    foreach my $i (1..$m)
    {
      my $t0 = [gettimeofday];
      system ("$FEATSEL_BIN " . 
              " -n " . $features{$data_set} . 
              " -a " . $algorithm . 
              " -l " . $labels{$data_set} .
              " -c " . $cost_function{$algorithm} .
              " -t " . $MAX_NUMBER_OF_COST_FUNCTION_CALLS .
              " -f " . $dat_file{$data_set} . 
              " > $LOG");
      my $t1 = [gettimeofday];

      printf "%2d featsel run on $data_set with $algorithm, required time = %f\n",
        $i, tv_interval ($t0, $t1);

      
  }
}