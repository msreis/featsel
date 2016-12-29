#
# HammingDistance.pm -- Perl module for instance generation of hamming_distance
#                       cost function. 
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

package HammingDistance;

use strict;

# This sub receives an instance size, a maximum allowed value for an element,
# an input directory path, an instance id number and creates at that directory
# a XML file containing a random Hamming distance instance.
#
sub random_hamming_distance_instance
{
  my ($size, $MAX_ELEMENT_VALUE, $INPUT_DIR, $id) = @_;

  my $file_name = sprintf "$INPUT_DIR/Test_%03d_%04d.xml", $size, $id;

  open (XML, ">$file_name")
    or die "Cannot create '$file_name' file!\n";

  printf XML "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
  printf XML "<ElementSet>\n";
  printf XML "  <SetName> Test_%03d_%04d </SetName>\n", $size, $id;
  printf XML "  <MaxNumberOfElementValues> 1 </MaxNumberOfElementValues>\n";
  printf XML "  <NumberOfElements> $size </NumberOfElements>\n";

  foreach my $i (1..$size)
  {
    printf XML "  <Element>\n    <name>e$_</name>\n";
    printf XML "    <value> %d  </value>\n  </Element>", rand (1);
  }

  printf XML "\n</ElementSet>\n";

  close (XML);
}

# End of module
#
1;

