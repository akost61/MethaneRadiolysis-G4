#ifndef CH2Star_h
#define CH2Star_h

#include "G4ParticleDefinition.hh"

class CH2Star
{
public:
    static G4ParticleDefinition* Definition();
private:
    CH2Star() = delete;
};

#endif
