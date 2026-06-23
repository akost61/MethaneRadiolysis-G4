#ifndef NU4PROCESS_HH
#define NU4PROCESS_HH

#include "CH4RotationalVibrationalProcess.hh"
#include "G4SystemOfUnits.hh"

// ν₄ (T₂) degenerate C–H bending vibrational mode
class Nu4Process : public CH4RotationalVibrationalProcess {
public:
    Nu4Process() : CH4RotationalVibrationalProcess("Nu4Process", 1.62e-1 * eV){}
protected:
    G4double GetCrossSection(G4double energy_eV) const override {
        return GetCrossSectionTable().GetNu4(energy_eV/eV);
    }
        
};

#endif