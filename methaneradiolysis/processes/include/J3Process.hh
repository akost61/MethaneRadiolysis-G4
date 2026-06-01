#ifndef J3PROCESS_HH
#define J3PROCESS_HH

#include "CH4RotationalVibrationalProcess.hh"
#include "G4SystemOfUnits.hh"

class J3Process : public CH4RotationalVibrationalProcess {
public:
    J3Process() : CH4RotationalVibrationalProcess("J3Process", 7.8e-3 * eV){}
    protected:
    G4double GetCrossSection(G4double energy_eV) const override {
        return GetCrossSectionTable().GetJ3(energy_eV/eV);
    }
        
};

#endif