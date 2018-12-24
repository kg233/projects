#ifndef SCORE_HPP
#define SCORE_HPP

#include "students.hpp"
#include "artifacts.hpp"
// void input_scores(Student& student, unsigned int length);
// int findStudent(const Student* studentArray);
void map_student_score(Student* studentArray, unsigned int artifactSize);
// void deallocGrades(Student* studentArray);
void calc_final_score(const Artifacts& artifacts, Student* studentArray);

#endif