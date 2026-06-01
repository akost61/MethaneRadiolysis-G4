#ifndef CPlus_h
#define CPlus_h

#include "G4ParticleDefinition.hh"

class CPlus
{
public:
    static G4ParticleDefinition* Definition();
private:
    CPlus() = delete;
};

#endif
