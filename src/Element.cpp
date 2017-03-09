//
// Element.cpp -- implementation of the class "Element".
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

#include "Element.h"

Element::Element (unsigned int number, string element_name)
{
  max_number_of_values = max_value = number_of_values = 0;
  if (element_name.length () > 0)
    name = element_name;
  else
    name = "s";
  if (number > 0)
  {
    max_number_of_values = number;
    value = new (nothrow) int [max_number_of_values];
    if (value == 0)
      cout << "Error in Element: could not allocate memory for element!\n";
  }
  else
    value = NULL;
}


Element::Element (Element * elm) 
{
  this->max_number_of_values = elm->max_number_of_values;
  this->name = elm->name;
  this->number_of_values = elm->number_of_values;
  this->value = new int[number_of_values];
  for (unsigned int i = 0; i < number_of_values; i++)
    this->value[i] = elm->value[i];
}


Element::~Element ()
{
  if (value != 0)
    delete [] value;
}


bool Element::add_element_value (int current_value)
{
  if (number_of_values < max_number_of_values)
  {
    value [number_of_values] = current_value;
    number_of_values++;
    if (current_value > (int) max_value)
      max_value = current_value;
    return true;
  }
  else
    return false;
}


void Element::print_element ()
{
  unsigned int i;
  cout << name << " : ";
  for (i = 0; i < number_of_values; i++)
  {
    cout << value [i] << " ";
  }
  cout << endl;
}


string Element::get_element_name ()
{
  return name;
}


void Element::set_element_name (string new_name)
{
  name = new_name;
}


int Element::get_element_value (unsigned int index)
{
  if (index < number_of_values)
    return value [index];
  else
  {
    cout << "Error in Element::get_element_value: index " <<
            index << " is out of range!" << endl;
    return -1;
  }
}


unsigned int Element::get_number_of_values ()
{
  return number_of_values;
}


unsigned int Element::get_max_value ()
{
  return max_value;
}


unsigned int Element::get_max_number_of_values ()
{
  return max_number_of_values;
}

