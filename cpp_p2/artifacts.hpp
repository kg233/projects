#ifndef ARTIFACTS_HPP
#define ARTIFACTS_HPP

struct Artifacts{
    unsigned int length;
    unsigned int* max_points;
    unsigned int* weight;
};


// void allocArray(Artifacts& artifacts);

// void take_values(Artifacts& artifacts, unsigned int* pointer);

void deallocArtifacts(const Artifacts& artifacts);

Artifacts makeArtifacts();
// void take_weights(Artifacts& artifacts);

#endif