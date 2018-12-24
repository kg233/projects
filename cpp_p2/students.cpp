#include <string>
#include <iostream>
#include "students.hpp"
#include "prompt.hpp"

struct Student;

void takeID(Student& student){
    std::string input_id = read();
    student.id = std::stoi(input_id);
}

void takeOption(Student& student){
    std::string input_opt = read();
    student.option = input_opt[0];
}

void takeName(Student& student){
    std::string name;
    ignore_one();
    std::getline(std::cin, name);
    student.name = name;
}

/*Student* makeStudent(){
    Student* result = new Student;
    takeID(*result);
    takeOption(*result);
    takeName(*result);
    return result;
}

Student** StudentArray(){
    unsigned int reps = askNum();
    Student** students_pointer = new Student*[reps];


    for (int ind = 0; ind < reps; ++ind){
        students_pointer[ind] = makeStudent();
    }

    students_pointer[0] -> length = reps; //making a note of the length of the array so I can deallocate it 

    return students_pointer;

}

void deallocStudentArrays(Student** p){
    for (int ind = 0; ind < p[0] -> length; ++ind){
        delete p[ind];
        std::cout << ind << std::endl;
    }

    delete[] p;
}*/
Student makeStudent(){
    Student result;
    takeID(result);
    takeOption(result);
    takeName(result);
    return result;
}

Student* StudentArray(){
    unsigned int reps = askNum();
    Student* students_pointer = new Student[reps];


    for (unsigned int ind = 0; ind < reps; ++ind){
        students_pointer[ind] = makeStudent();
    }

    students_pointer[0].length = reps;

    return students_pointer;

}

void deallocStudentArrays(Student* p){

    delete[] p;
}

