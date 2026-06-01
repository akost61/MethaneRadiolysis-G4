#ifndef C1PROCESS_HH
#define C1PROCESS_HH

#include "CH4PhotonProcess.hh"
#include "G4SystemOfUnits.hh"

class C1Process : public CH4PhotonProcess {
public:
    C1Process()
    : CH4PhotonProcess("CI",7.49  * eV){}

    protected:
    G4double GetCrossSection(G4double energy_eV) const override {
        return GetCrossSectionTable().GetC1(energy_eV/eV);
    }

};

#endif