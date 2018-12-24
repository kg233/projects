#include "prompt.hpp"
#include <iostream>
#include <string>
#include "students.hpp"

unsigned int askNum(){
    std::string result;
    std::getline(std::cin, result);
    return std::stoi(result);
}

std::string read(){
    std::string result;
    std::cin >> result;
    return result;
}

unsigned int read_to_int(){
    std::string result;
    std::cin >> result;
    return std::stoi(result);
}

void ignore_one(){
    std::cin.ignore(1);
}

void print_total_scores(Student* student){
    std::cout<<"TOTAL SCORES"<<std::endl;
    for (unsigned int ind = 0; ind < student[0].length; ++ind){
        std::cout<<student[ind].id<<" "<<student[ind].name<<" " << student[ind].score << std::endl;
    }

}