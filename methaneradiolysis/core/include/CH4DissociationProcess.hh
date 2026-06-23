#ifndef CH4DISSOCIATIONPROCESS_HH
#define CH4DISSOCIATIONPROCESS_HH

#include "CH4DiscreteProcess.hh"
#include <vector>
#include <string>
#include "G4ParticleTable.hh"
#include "G4DynamicParticle.hh"
#include "G4SystemOfUnits.hh"

// Handles neutral dissociation (ND) channels of CH4 by electron impact.
// On each interaction the electron loses thresholdEnergy + recoilEnergy.
// Stationary secondary particles listed in fProducts are created at the
// interaction vertex and recorded to the ROOT ntuple.
class CH4DissociationProcess : public CH4DiscreteProcess {
public:
    // thresholdEnergy: energy deposited to break the CH4 bond.
    // products: Geant4 particle names of the dissociation fragments.
    explicit CH4DissociationProcess(const G4String& name,
                                    G4double thresholdEnergy,
                                    const std::vector<std::string>& products);
    ~CH4DissociationProcess() override = default;

    G4VParticleChange* PostStepDoIt(const G4Track& track,
                                    const G4Step&  step) override;

protected:
    G4double                 fThresholdEnergy;   // energy required for dissociation
    std::vector<std::string> fProducts;          // names of the secondary fragments
};

#endif