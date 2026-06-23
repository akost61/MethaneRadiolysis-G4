#ifndef CH4ROTATIONALVIBRATIONALPROCESS_HH
#define CH4ROTATIONALVIBRATIONALPROCESS_HH

#include "CH4DiscreteProcess.hh"
#include "globals.hh"

// Handles rotational (J3, J4) and vibrational (Nu_1 through Nu_4) excitation
// channels of CH4. No secondary particles are produced; the electron simply
// loses fEnergyDeposit + recoilEnergy to internal molecular excitation.
class CH4RotationalVibrationalProcess : public CH4DiscreteProcess {
    public:
        // depositEnergy: the characteristic energy of the excitation mode (e.g. rotational quantum, vibrational mode energy).
        CH4RotationalVibrationalProcess(const G4String& name, G4double depositEnergy);

        ~CH4RotationalVibrationalProcess() override = default;

        G4VParticleChange* PostStepDoIt(const G4Track& track,
                                        const G4Step&  step) override;

    protected:
        G4double fEnergyDeposit;   // fixed energy deposited per interaction

};


#endif