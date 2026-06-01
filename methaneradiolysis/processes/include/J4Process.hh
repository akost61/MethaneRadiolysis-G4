#ifndef J4PROCESS_HH
#define J4PROCESS_HH

#include "CH4RotationalVibrationalProcess.hh"
#include "G4SystemOfUnits.hh"

class J4Process : public CH4RotationalVibrationalProcess {
public:
    J4Process() : CH4RotationalVibrationalProcess("J4Process", 1.3e-2 * eV){}
    protected:
    G4double GetCrossSection(G4double energy_eV) const override {
        return GetCrossSectionTable().GetJ4(energy_eV/eV);
    }
        
};

#endif