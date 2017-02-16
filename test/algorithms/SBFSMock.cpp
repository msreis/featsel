//
// SBFSMock.cpp -- implementation of the mock class "SBFSMock".
//
//    This file is part of the featsel program
//    Copyright (C) 2017  Marcelo S. Reis
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

#include "SBFSMock.h"


SBFSMock::SBFSMock () : SBFS ()
{
  // TODO Auto-generated constructor stub
}


SBFSMock::~SBFSMock ()
{
  // TODO Auto-generated destructor stub
}


unsigned int SBFSMock::get_best_element_test (ElementSubset * X)
{
  return get_best_element (X);
}


unsigned int SBFSMock::get_worst_element_test (ElementSubset * X)
{
  return get_worst_element (X);
}
