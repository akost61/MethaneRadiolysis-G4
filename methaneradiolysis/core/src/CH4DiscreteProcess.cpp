#include "CH4DiscreteProcess.hh"

G4double CH4DiscreteProcess::fCutOffEnergy = 0.0;
#include "G4Electron.hh"
#include "G4Material.hh"
#include "G4PhysicsFreeVector.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"
#include <functional>

CH4DiscreteProcess::CH4DiscreteProcess(const G4String& name)
    : G4VDiscreteProcess(name, fElectromagnetic)
{}

G4bool CH4DiscreteProcess::IsApplicable(const G4ParticleDefinition& particle) {
    return &particle == G4Electron::Electron();
}

   
G4double CH4DiscreteProcess::GetMeanFreePath(const G4Track&    track,
                                              G4double,
                                              G4ForceCondition* condition) {
    *condition = NotForced;
    const G4Material* mat = track.GetMaterial();
    if (!mat || mat->GetName() != "CH4") return DBL_MAX;

    G4double energy_eV = track.GetKineticEnergy();

    G4double sigma = GetCrossSection(energy_eV) * mm2;  

    G4double rho = mat->GetDensity();
    G4double M = 16.043 * g/mole;
    G4double nMol = (rho * Avogadro) / M;

    if (sigma <= 0.0) return DBL_MAX;

    double numberDensity = 2.471378e16;

    return 1.0 / (nMol * sigma);
}
