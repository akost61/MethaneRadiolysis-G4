
#include "HPlus.hh"
#include "G4ParticleTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"
 
G4ParticleDefinition* HPlus::Definition()
{
    G4ParticleDefinition* existing =
        G4ParticleTable::GetParticleTable()->FindParticle("H+");
    if (existing) return existing;
 
    return new G4ParticleDefinition(
        "H+",
        1.008 * CLHEP::amu_c2,
        0.0,
        1.0 * CLHEP::eplus,
        0, 0, 0, 0, 0, 0,
        "ion", 0, 0, 0,
        true, -1.0, nullptr, false
    );
}
 