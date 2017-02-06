/*************************************************************************
 * Copyright (C) 2017 by Rohit Dureja                                    *
 *                                                                       *
 * This file is part of SimplePDR.                                       *
 *                                                                       *
 *  SimplePDR is free software: you can redistribute it and/or modify    *
 *  it under the terms of the GNU General Public License as published by *
 *  the Free Software Foundation, either version 3 of the License, or    *
 *  (at your option) any later version.                                  *
 *                                                                       *
 *  SimplePDR is distributed in the hope that it will be useful,         *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of       *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the        *
 *  GNU General Public License for more details.                         *
 *                                                                       *
 *  You should have received a copy of the GNU General Public License    *
 *  along with SimplePDR.  If not, see <http://www.gnu.org/licenses/>.   *
 *************************************************************************/

#include "utils.h"
#include <sstream>

void split(const std::string &s, const char* delim,
        std::vector<std::string> & v) {
    char * dup = strdup(s.c_str());
    char * token = std::strtok(dup, delim);
    while (token != NULL) {
        v.push_back(std::string(token));
        token = std::strtok(NULL, delim);
    }
    free(dup);
}

/**
 *
 */
void Clause::add_literal(signed char literal) {
    literals.push_back(literal);
}

std::vector<signed char> * Clause::get_literals() {
    return &literals;
}

// utility to convert anthing to a string
template<typename T> std::string tostring(const T& x) {
    std::ostringstream os;
    os << x;
    return os.str();
}

/* Utility to generate SMTLIB2 strings for each clause in the
 * passed vector
 */
void generate_smtlib2_string(std::vector<Clause *> * clauses,
        std::vector<std::string> &str_clause,
        std::map<unsigned char, std::string> * map) {
    for (unsigned int i = 0; i < clauses->size(); ++i) {
        Clause * clause = (*clauses)[i]; // get i-th clause
        std::vector<signed char> * literals = clause->get_literals();
        if (literals->size() == 1) {
            str_clause.push_back("(assert");
            str_clause[i] =
                    (*literals)[0] > 0 ?
                            str_clause[i] + " " + (*map)[(*literals)[0]] :
                            str_clause[i] + " (not " + (*map)[-(*literals)[0]]
                                    + ")";
            str_clause[i] = str_clause[i] + ")";
        } else {
            str_clause.push_back("(assert (or");
            // iterative over literals in i-th clause
            for (unsigned int j = 0; j < literals->size(); ++j) {
                str_clause[i] =
                        (*literals)[j] > 0 ?
                                str_clause[i] + " " + (*map)[(*literals)[j]] :
                                str_clause[i] + " (not "
                                        + (*map)[-(*literals)[j]] + ")";
            }
            str_clause[i] = str_clause[i] + "))";
        }
    }
}
