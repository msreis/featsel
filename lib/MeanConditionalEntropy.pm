#
# MeanConditionalEntropy.pm -- Perl module for instance generation of mce
#                              cost function. 
#
#    This file is part of the featsel program
#    Copyright (C) 2016  Marcelo S. Reis
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

package MeanConditionalEntropy;

use strict;


# This sub receives an instance size, a maximum allowed value for an element,
# an input directory path, an instance id number and creates at that directory
# a XML file containing a random subset sum instance of the specified size.
#

my $p = 95;          # Probability that a given realization will be ignored
                     # during the creation of a random mce instance.

my $MAX_SAMPLING_VALUE = 5;  # Maximum sampling per realization 
                             # for MCE estimation

sub random_mce_instance
{
  my ($size, $MAX_ELEMENT_VALUE, $INPUT_DIR, $id) = @_;

  my $file_name = sprintf "$INPUT_DIR/Test_%03d_%04d.dat", $size, $id;

  open (DAT, ">$file_name")
    or die "Cannot create '$file_name' file!\n";

  foreach my $i (0..(2 ** $size - 1))
  {
    if (rand (100) > $p)
    {
      my $subset = sprintf "%0". $size . "b", $i;

      foreach my $b (0..$size)
      {
        printf DAT "%s ", substr ($subset, $b, 1);
      }

      printf DAT "%d %d\n",
        rand ($MAX_SAMPLING_VALUE), rand ($MAX_SAMPLING_VALUE);
    }
  }
  close (DAT);
}

# End of module
#
1;


