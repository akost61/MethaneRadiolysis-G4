#ifndef CHStar_h
#define CHStar_h

#include "G4ParticleDefinition.hh"

class CHStar
{
public:
    static G4ParticleDefinition* Definition();
private:
    CHStar() = delete;
};

#endif
