//
// DatParserDriver.h -- definition of the class "DatParserDriver".
//
//    This file is part of the featsel program
//    Copyright (C) 2015  Marcelo S. Reis
//
//    This program is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with this program.  If not, see <http://www.gnu.org/licenses/>.
//

#ifndef DAT_PARSER_DRIVER_HH
#define DAT_PARSER_DRIVER_HH

#include <string>
#include <map>
#include "../Element.h"


class DatParserDriver
{

public:

  DatParserDriver ();

  virtual ~DatParserDriver ();

  // Run the parser for n number of elements, l number of labels and input
  // file f, returning 0 on success.
  //
  int parse (unsigned int n, unsigned int l, string f);

  // Variable to store data parsed from DAT file.
  //
  Element * * list_of_elements;
  
  unsigned int number_of_elements, number_of_labels, max_number_of_values;

};

#endif // ! DAT_PARSER_DRIVER_HH

