//
// XmlParser.yy -- Bison grammar and definitions for a XML parser.
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

%skeleton "lalr1.cc"                          // C++
%require "2.4.1"
%defines
%define parser_class_name { XmlParser }

%code requires {
# include <string>
class XmlParserDriver;
}

// The parsing context.
%parse-param { XmlParserDriver& driver }
%lex-param   { XmlParserDriver& driver }
%locations
%initial-action
{
  // Initialize the initial location.
  @$.begin.filename = @$.end.filename = &driver.file;
};

%debug
%error-verbose

// Symbols.
%union
{
  int           ival;
  float         dval;
  std::string  *sval;
};

%code 
{
  #include "XmlParserDriver.h"
  std::stringstream out;
  pair <map <string, float>::iterator,bool> ret;
}

%token        END      0        "end of file"
%token <sval> OPEN_ELEM_SET     "open ElementSet"
%token <sval> CLOSE_ELEM_SET    "close ElementSet"
%token <sval> OPEN_SET_NAME     "open SetName"
%token <sval> CLOSE_SET_NAME    "close SetName"
%token <sval> OPEN_SET_VALUE    "open SetValue"
%token <sval> CLOSE_SET_VALUE   "close SetValue"
%token <sval> OPEN_NUM_ELEM     "open NumberOfElements"
%token <sval> CLOSE_NUM_ELEM    "close NumberOfElements"
%token <sval> OPEN_MAX_NUM_VAL  "open MaxNumberOfElementValues"
%token <sval> CLOSE_MAX_NUM_VAL "close MaxNumberOfElementValues"
%token <sval> OPEN_ELEM         "open Element"
%token <sval> CLOSE_ELEM        "close Element"
%token <sval> OPEN_COST         "open Cost"
%token <sval> CLOSE_COST        "close Cost"
%token <sval> OPEN_NAME         "open name"
%token <sval> CLOSE_NAME        "close name"
%token <sval> OPEN_VALUE        "open value"
%token <sval> CLOSE_VALUE       "close value"
%token <sval> XML_HEADER        "XML header"
%token <sval> STR_VALUE         "string value"
%token <ival> INT_VALUE         "integer value"
%token <dval> FLT_VALUE         "floating point value"

%type  <sval> set
%type  <sval> element
%type  <sval> value

%printer    { debug_stream () << *$$; } "string value"
%destructor { delete $$; } "string value"
%printer    { debug_stream () << $$; } <sval>


// Grammar
%%

%start input;

input: /* empty */

| "XML header" "open ElementSet" set "close ElementSet"

| "open ElementSet" set "close ElementSet"
;


set:  /* empty */

element

| set element
;
          

element:                

"open Element" value "close Element"    

| "open Cost" cost "close Cost"

| "open SetName" "string value" "close SetName"
{
  driver.set_name = *$2;  
  delete $2; 
}

| "open SetValue" "floating point value" "close SetValue" 
{
  driver.set_value = $2;  
}

| "open SetValue" "integer value" "close SetValue"
{
  driver.set_value = $2;
}

  
| "open NumberOfElements" "integer value" "close NumberOfElements"
{
  driver.number_of_elements = $2;
  driver.list_of_elements = new Element*[$2];
  if (driver.list_of_elements == 0)
    std::cout << "Error: could not allocate memory for" << 
    " ElementSet elements!" << std::endl;
}

| "open MaxNumberOfElementValues" "integer value" "close MaxNumberOfElementValues"
{ 
  driver.number_of_values = $2;
}
;
     

cost:

"open name" "string value" "close name" "open value" "floating point value" "close value"
{ 
  ret = driver.explicit_cost.insert (pair<string, float> (*$2, $5));
  if (!ret.second)
    std::cout << "Bison error: Error adding explicit cost function element "
    << $2 << std::endl;
  delete $2;   
} 
;


value:

"open name" "string value" "close name"
{ 
  driver.current_element++;
  driver.list_of_elements[driver.current_element-1] = 
  new Element(driver.number_of_values, "elem");
  if (driver.list_of_elements[driver.current_element-1] == 0)
	  std::cout << "Bison error: could not allocate memory for current Element!"
	  << std::endl;
  driver.list_of_elements[driver.current_element-1]->set_element_name (*$2); 
  delete $2;   
}
             
| "open name" "integer value" "close name"
{ 
  out << $2;
  driver.current_element++;
  driver.list_of_elements[driver.current_element-1] = 
  new Element(driver.number_of_values, "elem");
  if (driver.list_of_elements[driver.current_element-1] == 0)
    std::cout << "Bison error: could not allocate memory for current Element!"
    << std::endl;
  driver.list_of_elements[driver.current_element-1]->set_element_name
  (out.str());
  out.clear ();  
  out.str (std::string ());
}

| value "open value" "integer value" "close value" 
{ 
  driver.list_of_elements[driver.current_element-1]->add_element_value ($3);
}
;


%%

void yy::XmlParser::error
(const yy::XmlParser::location_type& l, const std::string& m)
{
  driver.error (l, m);
}

