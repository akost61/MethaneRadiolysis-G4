#include "CH4Plus.hh"
#include "G4ParticleTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"

G4ParticleDefinition* CH4Plus::Definition()
{
    G4ParticleDefinition* existing =
        G4ParticleTable::GetParticleTable()->FindParticle("CH4+");
    if (existing) return existing;

    G4ParticleDefinition* def = new G4ParticleDefinition(
        "CH4+",                 
        16.04 * CLHEP::amu_c2,
        0.0,                    
        1.0 * CLHEP::eplus,    
        0,                      
        0,                      
        0,                      
        0,                    
        0,                   
        0,                  
        "ion",                 
        0,                     
        0,                    
        0,                    
        true,                 
        -1.0,                  
        nullptr,               
        false                   
    );

    return def;
}