#ifndef CH4Plus_h
#define CH4Plus_h

#include "G4ParticleDefinition.hh"

class CH4Plus
{
public:
    static G4ParticleDefinition* Definition();
private:
    CH4Plus() = delete;
};

#endif