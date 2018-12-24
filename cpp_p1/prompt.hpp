//prompt.hpp



#ifndef PROMPT_HPP
#define PROMPT_HPP

#include <string>

//prompts for a lat in double//N,S, returns a float, neg if S or W, else pos
std::string promptL();

//location, name of location
std::string promptLoc();

//number of destinations
int promptRep();

//find the double value in each coordinate
double parseStr(std::string coord);


#endif