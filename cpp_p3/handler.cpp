//handler.cpp
#include "handler.hpp"

handler::handler()
    :command{"INVALID", "INVALID", "INVALID"}, DEBUGGING{false}
{

}


void handler::query_commands(){
    command_reset();

    std::string test;
    std::getline(std::cin, test);

    std::istringstream ss(test);
    std::string token;

    int temp = 0;

    while (ss >> token){
        if (temp < 3){
            command[temp] = token;
            temp ++;
        }

        else{
            command[0] = "INVALID";
            break;
        }
    }

}

void handler::print_command() const{
    std::cout << "\"" << command[0]<<  "\"" << " | " <<  "\"" <<command[1]<<  "\"" << " | " <<  "\"" << command[2]<< "\"" <<std::endl;
}

void handler::command_reset(){
    command[0] = "INVALID";
    command[1] = "INVALID";
    command[2] = "INVALID";
}



void handler::do_things(){
    while (true){
        query_commands();
        if (command[0] == "QUIT"){
            std::cout<<"GOODBYE" << std::endl;
            break;
        }
        std::cout<< reader() <<std::endl;
  }

}

std::string handler::reader(){
    if (command[0] == "INVALID") {
        return "INVALID";
    }
    if (command[0] == "CLEAR" and command[1] == "INVALID" and command[2] == "INVALID"){
        map.empty_all_buckets();
        return "CLEARED";
    }
    if (command[0] == "REMOVE" and command[1] != "INVALID" and command[2] == "INVALID"){
        if (map.contains(command[1])){
            map.remove(command[1]);
            return "REMOVED";
        }
        return "NONEXISTENT";
    }
    if (command[0] == "LOGIN" and command[1] != "INVALID" and command[2] != "INVALID"){
        if (map.contains(command[1]) and map.value(command[1]) == command[2]){
            return "SUCCEEDED";
        }
        return "FAILED";
    }
    if (command[0] == "CREATE" and command[1] != "INVALID" and command[2] != "INVALID"){
        if (map.contains(command[1])){
            return "EXISTS";
        }
        map.add(command[1], command[2]);
        return "CREATED";
    }

    if (command[0] == "DEBUG" and command[1] == "ON" and command[2] == "INVALID"){
        if (DEBUGGING){
            return "ON ALREADY";
        }
        DEBUGGING = true;
        return "ON NOW";
    }
    if (command[0] == "DEBUG" and command[1] == "OFF" and command[2] == "INVALID"){
        if (DEBUGGING){
            DEBUGGING = false;
            return "OFF NOW";
        }
        return "OFF ALREADY";
    }
    if (command[0] == "LOGIN" and command[1] == "COUNT" and command[2] == "INVALID"){
        if (DEBUGGING){
            return std::to_string(map.size());
        }
        return "INVALID";
        
    }
    if (command[0] == "BUCKET" and command[1] == "COUNT" and command[2] == "INVALID"){
        if (DEBUGGING){
            return std::to_string(map.bucketCount());
        }
        return "INVALID";
    }
    if (command[0] == "LOAD" and command[1] == "FACTOR" and command[2] == "INVALID"){
        if (DEBUGGING){
            return std::to_string(map.loadFactor());
        }
        return "INVALID";
    }
    if (command[0] == "MAX" and command[1] == "BUCKET" and command[2] == "SIZE"){
        if (DEBUGGING){
            return std::to_string(map.maxBucketSize());
        }
        return "INVALID";
    }
    else{
        return "INVALID";
    }

}