#include "CH2Star.hh"
#include "G4ParticleTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"
 
G4ParticleDefinition* CH2Star::Definition()
{
    G4ParticleDefinition* existing =
        G4ParticleTable::GetParticleTable()->FindParticle("CH2*");
    if (existing) return existing;
 
    return new G4ParticleDefinition(
        "CH2*",
        14.027 * CLHEP::amu_c2,
        0.0,
        0.0,
        0, 0, 0, 0, 0, 0,
        "nucleus", 0, 0, 0,
        true, -1.0, nullptr, false
    );
}
 