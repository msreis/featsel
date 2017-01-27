//
// XmlParserDriver.cpp -- implementation of the class "XmlParserDriver".
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

#include "XmlParser.hpp"
#include "XmlParserDriver.h"

XmlParserDriver::XmlParserDriver () : trace_scanning (false),
trace_parsing (false)
{
  list_of_elements = NULL;
  number_of_elements = 0;
  number_of_values = 0;
  current_element = 0;
  set_value = 0;
  set_name = "";
}


XmlParserDriver::~XmlParserDriver ()
{
  if (list_of_elements != NULL)
  {
    for (unsigned int i = 0; i < number_of_elements; i++)
      delete list_of_elements[i];
    delete [] list_of_elements;
  }
}

int XmlParserDriver::parse (const std::string &f)
{
  file = f;
  scan_begin ();

  yy::XmlParser parser (*this);
  parser.set_debug_level (trace_parsing);
  int res = parser.parse ();

  scan_end ();
  return res;
}


void XmlParserDriver::error (const yy::location &l, const std::string &m)
{
  std::cerr << l << ": " << m << std::endl;
}


void XmlParserDriver::error (const std::string &m)
{
  std::cerr << m << std::endl;
}

