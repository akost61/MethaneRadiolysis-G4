#ifndef CHPlus_h
#define CHPlus_h

#include "G4ParticleDefinition.hh"

class CHPlus
{
public:
    static G4ParticleDefinition* Definition();
private:
    CHPlus() = delete;
};

#endif
