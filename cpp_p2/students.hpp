#ifndef STUDENTS_HPP
#define STUDENTS_HPP


#include <string>

struct Student{
    unsigned int length;
    unsigned int id;
    char option;
    std::string name;
    int* grades;
    double score = 0;
};

// void takeID(Student& student);

// void takeOption(Student& student);

// void takeName(Student& student);

// Student* makeStudent();

// Student** StudentArray();

// void deallocStudentArrays(Student** p);
// Student makeStudent();

Student* StudentArray();

void deallocStudentArrays(Student* p);



#endif