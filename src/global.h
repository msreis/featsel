//
// global.h -- Global libraries, structures, and definitions.
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

#ifndef GLOBAL_H_
#define GLOBAL_H_

# include <cfloat>
# include <climits>
# include <cmath>
# include <cstdlib>
# include <ctime>
# include <fstream>
# include <iostream>
# include <list>
# include <map>
# include <sstream>
# include <string>

# define INFINITY_COST FLT_MAX

// This definition is used by the DatParserDriver and also for the
// MeanConditionalEntropy cost function.
//
// Use two labels for a binary classifier (for instance, W-operator estimation).
//
# define NUMBER_OF_LABELS 7


using namespace std;

#endif /* GLOBAL_H_ */

