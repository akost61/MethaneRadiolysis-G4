#ifndef CH4ROTATIONALVIBRATIONALPROCESS_HH
#define CH4ROTATIONALVIBRATIONALPROCESS_HH

#include "CH4DiscreteProcess.hh"
#include "globals.hh"

class CH4RotationalVibrationalProcess : public CH4DiscreteProcess {
    public:
        CH4RotationalVibrationalProcess(const G4String& name, G4double depositEnergy);

        ~CH4RotationalVibrationalProcess() override = default;

        G4VParticleChange* PostStepDoIt(const G4Track& track,
                                        const G4Step&  step) override;

    protected:
            G4double fEnergyDeposit;

};


#endif