#ifndef CH3Star_h
#define CH3Star_h

#include "G4ParticleDefinition.hh"

class CH3Star
{
public:
    static G4ParticleDefinition* Definition();
private:
    CH3Star() = delete;
};

#endif
