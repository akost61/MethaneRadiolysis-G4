#ifndef CH4PHOTONPROCESS_HH
#define CH4PHOTONPROCESS_HH

#include "CH4DiscreteProcess.hh"
#include "G4SystemOfUnits.hh"
#include "G4Gamma.hh"
#include "G4DynamicParticle.hh"
#include "G4RandomDirection.hh"
#include "G4SystemOfUnits.hh"

// Handles excitation channels that produce a photon (Lyman, Balmer, CH*, C lines).
// Emits a gamma with fixed fPhotonEnergy in a uniformly random direction and
// records the emission to the photon ROOT ntuple. The electron also loses
// recoilEnergy from elastic momentum transfer to the CH4 molecule.
class CH4PhotonProcess : public CH4DiscreteProcess {
public:
    // photonEnergy: fixed energy of the emitted gamma (characteristic line energy).
    explicit CH4PhotonProcess(const G4String& name,
                              G4double photonEnergy);
    ~CH4PhotonProcess() override = default;

    G4VParticleChange* PostStepDoIt(const G4Track& track,
                                    const G4Step&  step) override;

protected:
    G4double fPhotonEnergy;   // energy of the emitted gamma
};

#endif