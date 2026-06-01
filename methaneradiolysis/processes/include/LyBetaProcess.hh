#ifndef LYBETAPROCESS_HH
#define LYBETAPROCESS_HH

#include "CH4PhotonProcess.hh"
#include "G4SystemOfUnits.hh"

class LyBetaProcess : public CH4PhotonProcess {
public:
    LyBetaProcess(): CH4PhotonProcess("Ly-beta",12.08  * eV){}
    protected:
    G4double GetCrossSection(G4double energy_eV) const override {
        return GetCrossSectionTable().GetLyb(energy_eV/eV);
    }
        
};

#endif