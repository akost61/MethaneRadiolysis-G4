#ifndef HPlus_h
#define HPlus_h

#include "G4ParticleDefinition.hh"

class HPlus
{
public:
    static G4ParticleDefinition* Definition();
private:
    HPlus() = delete;
};

#endif
