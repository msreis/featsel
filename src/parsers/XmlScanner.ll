/*
 XmlScanner.ll -- a Flex scanner for a XML Parser.

    This file is part of the featsel program
    Copyright (C) 2015  Marcelo S. Reis

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


%{                                          

# include <cstdlib>
# include <cfloat>
# include <errno.h>
# include <limits.h>
# include <string>
# include "XmlParserDriver.h"
# include "XmlParser.hpp"

# define yyterminate() return token::END

%}

%option noyywrap nounput batch debug

DIGIT   [0-9]

STRING  [a-zA-Z_][a-zA-Z_0-9]*

BLANK   [ \t]

XML     \<\?[xX][mM][lL].*\?\>

%{
# define YY_USER_ACTION  yylloc->columns (yyleng);
%}

%%

%{
  yylloc->step ();
%}

{BLANK}+   yylloc->step ();

[\n]+      yylloc->lines (yyleng); yylloc->step ();

%{
  typedef yy::XmlParser::token token;
%}

{XML}                         return token::XML_HEADER;

"<ElementSet>"                return token::OPEN_ELEM_SET;

"</ElementSet>"               return token::CLOSE_ELEM_SET;

"<SetName>"                  return token::OPEN_SET_NAME;

"</SetName>"                 return token::CLOSE_SET_NAME;

"<SetValue>"                  return token::OPEN_SET_VALUE;

"</SetValue>"                 return token::CLOSE_SET_VALUE;

"<NumberOfElements>"          return token::OPEN_NUM_ELEM;

"</NumberOfElements>"         return token::CLOSE_NUM_ELEM;

"<MaxNumberOfElementValues>"  return token::OPEN_MAX_NUM_VAL;

"</MaxNumberOfElementValues>" return token::CLOSE_MAX_NUM_VAL;

"<Element>"                   return token::OPEN_ELEM;

"</Element>"                  return token::CLOSE_ELEM;

"<Cost>"                      return token::OPEN_COST;

"</Cost>"                     return token::CLOSE_COST;

"<name>"                      return token::OPEN_NAME;

"</name>"                     return token::CLOSE_NAME;

"<value>"                     return token::OPEN_VALUE;

"</value>"                    return token::CLOSE_VALUE;

"-"?{DIGIT}+                  {
  errno = 0;
  long n = strtol (yytext, NULL, 10);
  if (!((INT_MIN <= n) && (n <= INT_MAX) && (errno != ERANGE)))
    driver.error (*yylloc, "Flex error: Integer is out of range!\n");
  yylval->ival = n;
  return token::INT_VALUE;
}

{STRING}+                     {
  yylval->sval = new std::string (yytext);
  return token::STR_VALUE;
}

"-"?{DIGIT}+"."{DIGIT}+       {
  errno = 0;
  double d = strtod (yytext, NULL);
  if (!((d <= DBL_MAX) && (errno != ERANGE)))
    driver.error (*yylloc, "Flex error: Float is out of range!\n");
  yylval->dval = d;
  return token::FLT_VALUE;
}

.  driver.error (*yylloc, "Flex error: Invalid character!\n");

%%


void XmlParserDriver::scan_begin ()
{
  yy_flex_debug = trace_scanning;
  if (file == "-")
    yyin = stdin;
  else if (!(yyin = fopen (file.c_str (), "r")))
  {
    error (std::string ("Flex error: Cannot open ") + 
    file + std::string ("!\n"));
    exit (1);
  }
}


void XmlParserDriver::scan_end ()
{
  fclose (yyin);
}

