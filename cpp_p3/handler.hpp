//handler.hpp

#ifndef HANDLER_HPP
#define HANDLER_HPP

#include <iostream>
#include <string>
#include "HashMap.hpp"
#include <sstream>

class handler{
public:

    handler();
    //handler(std::function<unsigned int(const std::string&)>)

    void query_commands();

    void print_command() const;

    void do_things();

    void command_reset();

    std::string reader();



private:

    std::string command[3];

    bool DEBUGGING;


    HashMap map;

};





















#endif