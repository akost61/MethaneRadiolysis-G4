#ifndef C4PROCESS_HH
#define C4PROCESS_HH

#include "CH4PhotonProcess.hh"
#include "G4SystemOfUnits.hh"

class C4Process : public CH4PhotonProcess {
public:
    C4Process()
    : CH4PhotonProcess("CIV", 8.00 * eV){}
protected:
    G4double GetCrossSection(G4double energy_eV) const override {
        return GetCrossSectionTable().GetC4(energy_eV/eV);
    }
        
};

#endif