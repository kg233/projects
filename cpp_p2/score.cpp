#include "score.hpp"
#include "prompt.hpp"
#include "students.hpp"
#include "artifacts.hpp"
#include <iostream>
#include <string>
void input_scores(Student& student, unsigned int length){
    // student.grades = new int[length];
    for (unsigned int ind = 0; ind < length; ind++){
        std::string inp = read();
        student.grades[ind] = std::stoi(inp);

    }
    ignore_one();
}

int findStudent(const Student* studentArray){
    unsigned int id = read_to_int();
    // std::cout << "id:" << id << std::endl;

    for (unsigned int ind = 0; ind < studentArray[0].length; ++ind){
        if (id == studentArray[ind].id){
            return ind;
        }
    }
    return -1;
}

void init_student_grades(Student* studentArray, const unsigned int& length){
    for (unsigned int student_ind = 0; student_ind < studentArray[0].length; student_ind++){
        studentArray[student_ind].grades = new int[length];
        for (unsigned int grade_ind = 0; grade_ind < length; grade_ind ++){
            studentArray[student_ind].grades[grade_ind] = 0;
        }
    }
}

void map_student_score(Student* studentArray, unsigned int artifactSize){

    init_student_grades(studentArray, artifactSize);

    unsigned int reps = askNum();
    for (unsigned int i = 0; i < reps; ++i){
        // std::cout << "loop" << i << std::endl;
        int ind = findStudent(studentArray);
        if (ind != -1){

            input_scores(studentArray[ind], artifactSize);
        }
        else{
            std::string dump;
            std::getline(std::cin, dump);
        }

        

    }

}

void deallocGrades(Student* studentArray){
    for (unsigned int i = 0; i < studentArray[0].length; ++i){
        delete[] studentArray[i].grades;
    }
}

void calc_final_score(const Artifacts& artifacts, Student* studentArray){
    for (unsigned int grade_ind = 0; grade_ind < artifacts.length; ++grade_ind){
        for (unsigned int stud_ind = 0; stud_ind < studentArray[0].length; stud_ind++){

            double perc = (studentArray[stud_ind].grades[grade_ind]*1.0)/(artifacts.max_points[grade_ind]*1.0);

            double p = perc * artifacts.weight[grade_ind];
            studentArray[stud_ind].score += p;

        }
    }
    print_total_scores(studentArray);
    deallocArtifacts(artifacts);
    deallocGrades(studentArray);
}
