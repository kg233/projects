//prompt.cpp
#include <iostream>
#include <string>

#include "prompt.hpp"


//prompts for a lat in double//N,S 
std::string promptL()
{
    std::string coord;

    std::cin >> coord;
    // std::getline(std::cin, latD);

    return coord;
}

//location, name of location
std::string promptLoc()
{
    std::string loc;
    std::cin.ignore(1);
    std::getline(std::cin, loc);

    return loc;
}

//number of destinations
int promptRep()
{
    int rep;
    std::cin >> rep;
    return rep;
}

//find the double value in each coordinate
double parseStr(std::string coord)
{
    double value = std::stod(coord);


    if (coord[coord.length() - 1] == 'S' or coord[coord.length() - 1] == 'W')
    {
        value *= -1;
    }

    return value;
}