#ifndef H2Molecule_h
#define H2Molecule_h

#include "G4ParticleDefinition.hh"

class H2Molecule
{
public:
    static G4ParticleDefinition* Definition();
private:
    H2Molecule() = delete;
};

#endif
