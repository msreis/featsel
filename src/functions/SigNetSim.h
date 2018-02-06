//
// SigNetSim.h -- definition of the class "SigNetSim".
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

#ifndef SIGNETSIM_H_
#define SIGNETSIM_H_

#include "../global.h"
#include "../ElementSet.h"
#include "../ElementSubset.h"
#include "../CostFunction.h"

#define SIGNETSIM_PROGRAM "SigNetSim.py"

#define SIGNETSIM_INPUT_PATH "src/functions/SigNetSim.py"

#define SIGNETSIM_OUTPUT_PATH "output/SigNetSim.tmp"

class SigNetSim : public CostFunction
{

public:

  // Default constructor.
  //
  SigNetSim (ElementSet *);


  // Default destructor.
  //
  virtual ~SigNetSim ();


  // Returns the value of c(X), where X is a subset.
  //
  double cost (ElementSubset *);

};

#endif /* SIGNETSIM_H_ */
