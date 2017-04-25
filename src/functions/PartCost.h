//
// PartCost.h -- definition of the class "PartCost".
//
//    This file is part of the featsel program
//    Copyright (C) 2017 Gustavo Estrela
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

#ifndef PARTCOST_H_
#define PARTCOST_H_


#include "../global.h"
#include "../ElementSet.h"
#include "../ElementSubset.h"
#include "../CostFunction.h"
#include "../PartitionNode.h"

class PartCost : public CostFunction
{

protected:

  // Stores the information about the partition being evaluated
  //
  PartitionNode * partition;


  // Stores the orignal cost function
  //
  CostFunction * orig_cost_f;

public:

  // Default constructor.
  //
  PartCost (CostFunction *, PartitionNode *);


  // Default destructor.
  //
  virtual ~PartCost ();


  // Returns the value of c(X), where X is a subset.
  //
  double cost (ElementSubset *);

};

#endif /* PARTCOST_H_ */
