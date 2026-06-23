#ifndef NU3PROCESS_HH
#define NU3PROCESS_HH

#include "CH4RotationalVibrationalProcess.hh"
#include "G4SystemOfUnits.hh"

// ν₃ (T₂) asymmetric C–H stretch vibrational mode
class Nu3Process : public CH4RotationalVibrationalProcess {
public:
    Nu3Process() : CH4RotationalVibrationalProcess("Nu3Process", 3.74e-1 * eV){}
    protected:
    G4double GetCrossSection(G4double energy_eV) const override {
        return GetCrossSectionTable().GetNu3(energy_eV/eV);
    }
        
};

#endif