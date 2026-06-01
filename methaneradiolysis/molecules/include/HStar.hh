#ifndef HStar_h
#define HStar_h

#include "G4ParticleDefinition.hh"

class HStar
{
public:
    static G4ParticleDefinition* Definition();
private:
    HStar() = delete;
};

#endif
