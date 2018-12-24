#include <iostream>
#include <string>

#include "prompt.hpp"
#include "math.hpp"

int main()
{

    std::string start_lat = promptL();
    std::string start_lon = promptL();
    std::string start_loc = promptLoc();

    int rep = promptRep();

    std::string max_lat;
    std::string max_lon;
    std::string max_loc;
    double max_dist = -1;

    std::string min_lat;
    std::string min_lon;
    std::string min_loc;
    double min_dist = -1;

    for (int i = 0; i < rep; ++i)
    {
        std::string curr_lat = promptL();
        std::string curr_lon = promptL();
        std::string curr_loc = promptLoc();

        double curr_dist = calc_dist(parseStr(curr_lat), parseStr(curr_lon), 
            parseStr(start_lat), parseStr(start_lon));

        if ( curr_dist > max_dist)
        {
            max_dist = curr_dist;
            max_lat = curr_lat;
            max_lon = curr_lon;
            max_loc = curr_loc;
        }

        if ( min_dist == -1 or curr_dist < min_dist)
        {
            min_dist = curr_dist;
            min_lat = curr_lat;
            min_lon = curr_lon;
            min_loc = curr_loc;
        }            
        
    }

    std::cout << "Start Location: " << start_lat << " " << start_lon << " ("<< start_loc << ")"<<std::endl;
    std::cout << "Closest Location: " << min_lat << " " << min_lon << " (" << min_loc << ") (" << min_dist << " miles)" << std::endl;
    std::cout << "Farthest Location: " << max_lat << " " << max_lon << " (" << max_loc << ") (" << max_dist << " miles)" <<std::endl;












    return 0;
}

