#ifndef H2Plus_h
#define H2Plus_h

#include "G4ParticleDefinition.hh"

class H2Plus
{
public:
    static G4ParticleDefinition* Definition();
private:
    H2Plus() = delete;
};

#endif
