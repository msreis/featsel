//
// XmlParserDriver.h -- definition of the class "XmlParserDriver".
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

#ifndef XML_PARSER_DRIVER_HH
#define XML_PARSER_DRIVER_HH

#include <string>
#include <map>
#include "XmlParser.hpp"
#include "../Element.h"


// Tell Flex the lexer prototype.
//
# define YY_DECL                                    \
  yy::XmlParser::token_type                         \
  yylex (yy::XmlParser::semantic_type * yylval,     \
         yy::XmlParser::location_type * yylloc,     \
         XmlParserDriver &driver)


// Declare the lexer prototype for the parser's sake.
//
YY_DECL;


// Conduct the whole scanning and parsing.
//
class XmlParserDriver
{

public:

  XmlParserDriver ();

  virtual ~XmlParserDriver ();

  // Handling the scanner.
  //
  void scan_begin ();

  void scan_end ();

  bool trace_scanning;


  // Run the parser and return 0 on success.
  //
  int parse (const std::string &f);

  std::string file;

  bool trace_parsing;


  // Error handling.
  //
  void error (const yy::location& l, const std::string& m);

  void error (const std::string& m);


  // Variables to store data parsed from XML file.
  //
  Element * * list_of_elements;

  map <string, float> explicit_cost;

  float set_value;

  string set_name;

  unsigned int number_of_elements;

  unsigned int number_of_values;

  unsigned int current_element;

};

#endif // ! XML_PARSER_DRIVER_HH

