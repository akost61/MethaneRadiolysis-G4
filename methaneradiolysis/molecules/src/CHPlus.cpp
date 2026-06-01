#include "CHPlus.hh"
#include "G4ParticleTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"
 
G4ParticleDefinition* CHPlus::Definition()
{
    G4ParticleDefinition* existing =
        G4ParticleTable::GetParticleTable()->FindParticle("CH+");
    if (existing) return existing;
 
    return new G4ParticleDefinition(
        "CH+",
        13.019 * CLHEP::amu_c2,
        0.0,
        1.0 * CLHEP::eplus,
        0, 0, 0, 0, 0, 0,
        "ion", 0, 0, 0,
        true, -1.0, nullptr, false
    );
}
 