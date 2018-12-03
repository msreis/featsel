//
// UCSR.cpp -- definition of the class "UCSR".
//
//    This file is part of the featsel program
//    Copyright (C) 2016 Gustavo E. Matos
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

// Reis, M.S., Estrela, G., Ferreira, C.E., and Barrera, B. (2018).
// "Optimal Boolean lattice-based algorithms for the The U-curve
// optimization problem"
// Information Sciences, doi.org/10.1016/j.ins.2018.08.060.


#ifndef UCSR_H_
#define UCSR_H_

#include "../ROBDD.h"
#include "../global.h"
#include "../Solver.h"
#include "UCSRToolBox.h"

class UCSR : public Solver
{

private:

    // The collection of restrictions
    //
    ROBDD * restrictions;
    //ROBDD * lower_restriction, * upper_restriction;

    // Variables to study the performance of this algorithm.
    //
    int elapsed_time_of_all_calls_of_the_minima_exhausting;

    int elapsed_time_consulting_restrictions;

    int elapsed_time_reducing_restrictions;

    int elapsed_time_updating_restrictions;

    unsigned int number_of_calls_of_minimum_exhausting;

    unsigned int number_of_restrictions_updates;

    unsigned int number_of_restrictions_consults;


public:

    // Default constructor.
    //
    UCSR ();


    // Default destructor.
    //
    virtual ~UCSR ();


    // Runs the UCSR algorithm, getting up to 'max_size_of_minima_list' minimum subsets.
    //
    void get_minima_list (unsigned int);

};

#endif /* UCSR_H_ */
