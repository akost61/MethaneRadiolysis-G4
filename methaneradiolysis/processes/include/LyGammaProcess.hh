#ifndef LYGAMMAPROCESS_HH
#define LYGAMMAPROCESS_HH

#include "CH4PhotonProcess.hh"
#include "G4SystemOfUnits.hh"

// Lyman-gamma (n=4→1) photon emission from excited H*
class LyGammaProcess : public CH4PhotonProcess {
public:
    LyGammaProcess(): CH4PhotonProcess("Ly-gamma",12.75  * eV){}
    protected:
    G4double GetCrossSection(G4double energy_eV) const override {
        return GetCrossSectionTable().GetLyg(energy_eV/eV);
    }
        
};

#endif