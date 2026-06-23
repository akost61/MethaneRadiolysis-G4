#ifndef C3PROCESS_HH
#define C3PROCESS_HH

#include "CH4PhotonProcess.hh"
#include "G4SystemOfUnits.hh"

// CIII doubly-ionized carbon emission line
class C3Process : public CH4PhotonProcess {
public:
    C3Process()
    : CH4PhotonProcess("CIII",6.48  * eV){}

    protected:
    G4double GetCrossSection(G4double energy_eV) const override {
        return GetCrossSectionTable().GetC3(energy_eV/eV);
    }

};

#endif