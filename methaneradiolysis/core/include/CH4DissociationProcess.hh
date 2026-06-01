#ifndef CH4DISSOCIATIONPROCESS_HH
#define CH4DISSOCIATIONPROCESS_HH

#include "CH4DiscreteProcess.hh"
#include <vector>
#include <string>
#include "G4ParticleTable.hh"
#include "G4DynamicParticle.hh"
#include "G4SystemOfUnits.hh"

class CH4DissociationProcess : public CH4DiscreteProcess {
public:
    explicit CH4DissociationProcess(const G4String& name,
                                    G4double thresholdEnergy,
                                    const std::vector<std::string>& products);
    ~CH4DissociationProcess() override = default;

    G4VParticleChange* PostStepDoIt(const G4Track& track,
                                    const G4Step&  step) override;

protected:
    G4double                 fThresholdEnergy;
    std::vector<std::string> fProducts;
};

#endif