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
my $LOG_FILE       = "output/hybrid.log";
my $INPUT_DIR      = "input/";
my $MAX_NUMBER_OF_COST_FUNCTION_CALLS = 1000000;

my @DATA_SETS = (
  "Promoters" 
  # "Optdigits", "Mus2",
  # "Arrhythmia"
  # , 
  # "Madelon");
  #, "Gisette"
  );
my %labels    = ("Promoters"  => 2,
                 "Optdigits"  => 10,
                 "Musk2"      => 2,
                 "Arrhythmia" => 16,
                 "Madelon"    => 2,
                 "Gisette"    => 2);
my %features =  ("Promoters"  => 57,
                 "Optdigits"  => 64,
                 "Musk2"      => 166,
                 "Arrhythmia" => 279,
                 "Madelon"    => 500,
                 "Gisette"    => 5000);
my %data_size = ("Promoters"  => 106,
                 "Optdigits"  => 5620,
                 "Musk2"      => 6598,
                 "Arrhythmia" => 452,
                 "Madelon"    => 2600,
                 "Gisette"    => 1000);
my %dat_file =  ("Promoters" => "input/Promoters/Test_01_A.dat",
  "Optdigits" => "input/Optdigits/Test_01_A.dat",
  "Musk2" => "input/Musk2/Test_01_A.dat",
  "Arrhythmia" => "input/Arrhythmia/Test_01_A.dat",
  "Madelon" => "input/Madelon/Test_01_A.dat",
  "Gisette" => "input/Gisette/Test_01_A.dat");

foreach my $data_set (@DATA_SETS)
{
  my @ranked_features;
  my $pucs_features;
  my $spec_cmi_features;
  my $hybrid_features;
  my $k;

  # Find features selected by PUCS
  my $featsel_call_str = "$FEATSEL_BIN " . 
    "-n $features{$data_set} -l $labels{$data_set} -c mce " .
    "-f $dat_file{$data_set} -a pucs > $LOG_FILE";
  system ($featsel_call_str);
  open LOG, $LOG_FILE;
  while (<LOG>)
  {
    if ($_ =~ /\<(\d+)\>\s+\:\s+(\S+)/)
    {
      $pucs_features = $1;
    }
  }
  close (LOG);

  # print "PUCS selected: $pucs_features\n";

  # Find features ranking by SpecCMI
  $featsel_call_str = "$FEATSEL_BIN " . 
    "-n $features{$data_set} -l $labels{$data_set} -c cmi " .
    "-f $dat_file{$data_set} -a spec_cmi > $LOG_FILE";
  system ($featsel_call_str);
  open LOG, $LOG_FILE;
  while (<LOG>)
  {
    if ($_ =~ /(\d+)\s+:.*/)
    {
      my $feature = $1;
      push (@ranked_features, $feature);
    }
  }
  close (LOG);

  # While features selected from spec cmi do not cover features
  # selected by PUCS we design two operators: the first one with
  # k best features from SpecCMI; the second operator has the union
  # of the best k features from SpecCMI
  $k = 10;
  print ("\nFor dataset $data_set:\n");
  do
  {
    my $hybrid_err;
    my $spec_cmi_err;
    my $k_best_feat = ranked_best (\@ranked_features, $k);
    $hybrid_features = join_features ($k_best_feat, $pucs_features);
    my $card = $hybrid_features =~ tr/1/1/;
    $spec_cmi_features = ranked_best (\@ranked_features, $card);

    # Performs cross-validation
    system ("./bin/svm_cross_validation.pl " .
      "$dat_file{$data_set} $features{$data_set} " .
      "$labels{$data_set} 10 $hybrid_features > $LOG_FILE");
    open LOG, $LOG_FILE;
    while (<LOG>)
    {
      if ($_ =~ /Cross-validation\serror:\s(0.\d+)/)
      {
        $hybrid_err = $1;
      }
    }
    close (LOG);

    system ("./bin/svm_cross_validation.pl " .
      "$dat_file{$data_set} $features{$data_set} " .
      "$labels{$data_set} 10 $spec_cmi_features > $LOG_FILE");
    open LOG, $LOG_FILE;
    while (<LOG>)
    {
      if ($_ =~ /Cross-validation\serror:\s(0.\d+)/)
      {
        $spec_cmi_err = $1;
      }
    }
    close (LOG);

    if (!defined $hybrid_err || !defined $spec_cmi_features)
    {
      die "\nCould not perform cross validation on $data_set\n\n";
    }

    print ("\nk = $k\n");
    print ("Hybrid  features: $hybrid_features\n");
    print ("SpecCMI features: $spec_cmi_features\n");
    printf ("Hybrid  feature selection has error %6.3f\n", $hybrid_err);
    printf ("SpecCMI feature selection has error %6.3f\n", $spec_cmi_err);
    $k++;
  } while (!covers ($spec_cmi_features, $pucs_features));
}


sub ranked_best
{
  my @rank = @{$_[0]};
  my $k = $_[1];
  my @feat_arr = (0) x scalar @rank;
  for (my $i = 0; $i < $k; $i++)
  {
    $feat_arr[$rank[$i] - 1] = 1;
  }
  return join ('', @feat_arr);
}


sub join_features
{
  my $features1 = $_[0];
  my $features2 = $_[1];
  my @feature_arr1 = split ('', $features1);
  my @feature_arr2 = split ('', $features2);
  my @joint_features = ();
  for (my $i = 0; $i < scalar @feature_arr1; $i++)
  {
    my $m = $feature_arr1[$i] || $feature_arr2[$i];
    push (@joint_features, $m);
  }
  return join ('', @joint_features);
}

sub covers
{
  my $features1 = $_[0];
  my $features2 = $_[1];
  my @feature_arr1 = split ('', $features1);
  my @feature_arr2 = split ('', $features2);
  for (my $i = 0; $i < scalar @feature_arr1; $i++)
  {
    if ($feature_arr1[$i] < $feature_arr2[$i])
    {
      return 0;
    }
  }
  return 1;
}

