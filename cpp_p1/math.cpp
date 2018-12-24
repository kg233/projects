//math.cpp

#include <cmath>

#include "math.hpp"

const double PI = 3.141592653589793;
const double EARTH_RAD = 3959.9;

//converts coordinate degree to radians
double _crd_to_rad(double coord)
{
    return coord * PI/180;
}

//finds the difference between two doubles, and returns the abs value of it
double _diff(double first, double second)
{
    return abs(first-second);
}


//part a of the haversine function
double _haversine_a(double lat1, double lon1, double lat2, double lon2)
{
    
    return std::pow(std::sin(_diff(_crd_to_rad(lat1),_crd_to_rad(lat2))/2), 2) +
    (
        std::cos(_crd_to_rad(lat1)) * std::cos(_crd_to_rad(lat2)) * 
        std::pow(std::sin(_diff(_crd_to_rad(lon1),_crd_to_rad(lon2))/2), 2)
    );
}

//part b of the haversine function
double _haversine_b(double hav_a)
{
    return (2 * std::atan2(std::sqrt(hav_a), std::sqrt(1-hav_a)));
}

//part c of the haversine function
double _haversine(double hav_c)
{
    return EARTH_RAD * hav_c;
}

//calculates distance between two locations
double calc_dist(double lat1, double lon1, double lat2, double lon2)
{
    return _haversine(_haversine_b(_haversine_a(lat1, lon1, lat2, lon2)));
}