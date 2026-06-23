#ifndef NU2PROCESS_HH
#define NU2PROCESS_HH

#include "CH4RotationalVibrationalProcess.hh"
#include "G4SystemOfUnits.hh"

// ν₂ (E) degenerate H–C–H bending mode
class Nu2Process : public CH4RotationalVibrationalProcess {
public:
    Nu2Process() : CH4RotationalVibrationalProcess("Nu2Process", 1.90e-1 * eV){}
    protected:
    G4double GetCrossSection(G4double energy_eV) const override {
        return GetCrossSectionTable().GetNu2(energy_eV/eV);
    }
        
};

#endif