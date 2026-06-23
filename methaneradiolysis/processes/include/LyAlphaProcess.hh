#ifndef LYALPHAPROCESS_HH
#define LYALPHAPROCESS_HH

#include "CH4PhotonProcess.hh"
#include "G4SystemOfUnits.hh"

// Lyman-alpha (n=2→1) photon emission from excited H*
class LyAlphaProcess : public CH4PhotonProcess {
public:
    LyAlphaProcess(): CH4PhotonProcess("Ly-alpha",10.20  * eV){}
    protected:
    G4double GetCrossSection(G4double energy_eV) const override {
        return GetCrossSectionTable().GetLya(energy_eV/eV);
    }
        
};

#endif