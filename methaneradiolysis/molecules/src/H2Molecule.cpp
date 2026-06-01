#include "H2Molecule.hh"
#include "G4ParticleTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"
 
G4ParticleDefinition* H2Molecule::Definition()
{
    G4ParticleDefinition* existing =
        G4ParticleTable::GetParticleTable()->FindParticle("H2");
    if (existing) return existing;
 
    return new G4ParticleDefinition(
        "H2",
        2.016 * CLHEP::amu_c2,
        0.0,
        0.0,
        0, 0, 0, 0, 0, 0,
        "nucleus", 0, 0, 0,
        true, -1.0, nullptr, false
    );
}
 