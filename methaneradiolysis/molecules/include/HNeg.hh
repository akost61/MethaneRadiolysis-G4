#ifndef HNEG_h
#define HNEG_h

#include "G4ParticleDefinition.hh"

class HNeg
{
public:
    static G4ParticleDefinition* Definition();
private:
    HNeg() = delete;
};

#endif