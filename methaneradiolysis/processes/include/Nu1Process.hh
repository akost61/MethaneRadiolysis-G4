#ifndef NU1PROCESS_HH
#define NU1PROCESS_HH

#include "CH4RotationalVibrationalProcess.hh"
#include "G4SystemOfUnits.hh"

// ν₁ (A₁) symmetric C–H stretch vibrational mode
class Nu1Process : public CH4RotationalVibrationalProcess {
public:
   Nu1Process() : CH4RotationalVibrationalProcess("Nu1Process", 3.62e-1 * eV){}
protected:
    G4double GetCrossSection(G4double energy_eV) const override {
        return GetCrossSectionTable().GetNu1(energy_eV/eV);
    }
        
};

#endif