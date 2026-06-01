#ifndef HALPHAPROCESS_HH
#define HALPHAPROCESS_HH

#include "CH4PhotonProcess.hh"
#include "G4SystemOfUnits.hh"

class HAlphaProcess : public CH4PhotonProcess {
public:
    HAlphaProcess(): CH4PhotonProcess("H-alpha",1.89  * eV) {}
    protected:
    G4double GetCrossSection(G4double energy_eV) const override {
        return GetCrossSectionTable().GetHa(energy_eV/eV);
    }
        
};

#endif