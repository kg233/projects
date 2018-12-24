#ifndef PROMPT_HPP
#define PROMPT_HPP

#include <string>
#include "students.hpp"

unsigned int askNum();

std::string read();

void ignore_one();

unsigned int read_to_int();

void print_total_scores(Student* student);

#endif