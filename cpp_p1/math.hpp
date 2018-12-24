//math.hpp

#ifndef MATH_HPP
#define MATH_HPP

//converts coordinate degree to radians
double _crd_to_rad(double coord);

//finds the difference between two doubles, and returns the abs value of it
double _diff(double first, double second);

//part a of the haversine function
double _haversine_a(double lat1, double lon1, double lat2, double lon2);

//part b of the haversine function
double _haversine_b(double hav_a);

//part c of the haversine function
double _haversine(double hav_c);

//calculates distance between two locations
double calc_dist(double lat1, double lon1, double lat2, double lon2);












#endif