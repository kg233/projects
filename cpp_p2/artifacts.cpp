#include "artifacts.hpp"
#include "prompt.hpp"
#include <string>

struct Artifacts;

void allocArray(Artifacts& artifacts){
    unsigned int size = askNum();

    artifacts.length = size;
    artifacts.max_points = new unsigned int[size];
    artifacts.weight = new unsigned int[size];
}

void take_values(Artifacts& artifacts, unsigned int* pointer){
    for (unsigned int ind = 0; ind < artifacts.length; ind++){
        std::string inp = read();
        pointer[ind] = std::stoi(inp);

    }
    ignore_one();
}

Artifacts makeArtifacts(){
    Artifacts result;
    allocArray(result);
    take_values(result, result.max_points);
    take_values(result, result.weight);
    return result;
}

void deallocArtifacts(const Artifacts& artifacts){
    delete[] artifacts.max_points;
    delete[] artifacts.weight;
}