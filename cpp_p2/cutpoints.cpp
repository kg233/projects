#include "cutpoints.hpp"
#include "students.hpp"
#include <string>
#include <iostream>
#include "prompt.hpp"



char find_letter_grade(const double* cuts, Student student){

    if (student.score >= cuts[0]){
        return 'A';
    }
    else{
        if (student.score >= cuts[1]){
            return 'B';
        }
        else{
            if (student.score >= cuts[2]){
                return 'C';

            }
            else{
                if (student.score >= cuts[3]){
                    return 'D';
                }
                else{
                    return 'F';
                }
            }
        }
    }
}

std::string find_pass_grade(const double* cuts, Student student){
    if (student.score >= cuts[2]){
        return "P";
    }
    else{
        return "NP";
    }
}

double* create_cutpoint(){
    double* cuts = new double[4];
    std::cin >> cuts[0];
    std::cin >> cuts[1];
    std::cin >> cuts[2];
    std::cin >> cuts[3];
    std::cin.ignore(1);
    return cuts;
}

void print_cut_result(Student* studentArray, unsigned int num){
    double* cuts = create_cutpoint();
    std::cout << "CUTPOINT SET "<< num << std::endl;
    for (unsigned int ind = 0; ind < studentArray[0].length; ++ind){
        if (studentArray[ind].option == 'G'){
            std::cout<< studentArray[ind].id<< " "<< studentArray[ind].name<< " "<< find_letter_grade(cuts, studentArray[ind])
            << std::endl;
        }
        else{
            std::cout<< studentArray[ind].id<< " "<< studentArray[ind].name<< " "<< find_pass_grade(cuts, studentArray[ind])
            << std::endl;
        }
    }
    delete[] cuts;
}

void repeat_print_cutResult(Student* studentArray){
    unsigned int reps = askNum();
    for (unsigned int num = 1; num <= reps; ++num){
        print_cut_result(studentArray, num);
    }
    deallocStudentArrays(studentArray);

}