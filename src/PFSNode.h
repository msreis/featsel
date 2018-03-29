//
// PFSNode.h -- definition of the class "PFSNode".
//
//    This file is part of the featsel program
//    Copyright (C) 2017  Marcelo S. Reis, Gustavo Estrela
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

#ifndef PFSNODE_H_
#define PFSNODE_H_

#include "global.h"
#include "ElementSubset.h"

typedef struct MyNode
{
  ElementSubset * vertex,
                * adjacent;
  unsigned int leftmost;
  float cost;
} PFSNode;

#endif /* PFSNODE_H_ */