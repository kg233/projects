#include <iostream>
#include "students.hpp"
#include "artifacts.hpp"
#include "prompt.hpp"
#include "score.hpp"
#include "cutpoints.hpp"

int main()
{
    Artifacts artifacts = makeArtifacts();
    Student* student = StudentArray();
    map_student_score(student, artifacts.length);


    calc_final_score(artifacts,student);

    repeat_print_cutResult(student);






    return 0;
}

