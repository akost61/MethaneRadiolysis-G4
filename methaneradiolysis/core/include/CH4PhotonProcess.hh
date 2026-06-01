#ifndef CH4PHOTONPROCESS_HH
#define CH4PHOTONPROCESS_HH

#include "CH4DiscreteProcess.hh"
#include "G4SystemOfUnits.hh"
#include "G4Gamma.hh"
#include "G4DynamicParticle.hh"
#include "G4RandomDirection.hh"
#include "G4SystemOfUnits.hh"

class CH4PhotonProcess : public CH4DiscreteProcess {
public:
    explicit CH4PhotonProcess(const G4String& name,
                              G4double photonEnergy);
    ~CH4PhotonProcess() override = default;

    G4VParticleChange* PostStepDoIt(const G4Track& track,
                                    const G4Step&  step) override;

protected:
    G4double fPhotonEnergy;
};

#endif