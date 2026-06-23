#ifndef HGAMMA_PROCESS_HH
#define HGAMMA_PROCESS_HH

#include "CH4PhotonProcess.hh"

// Balmer-gamma (n=5→2) photon emission from excited H*
class HGammaProcess : public CH4PhotonProcess
{
public:
    HGammaProcess(): CH4PhotonProcess("H-gamma", 2.86*eV){}

protected:
    G4double GetCrossSection(G4double energy_eV) const override {
        return GetCrossSectionTable().GetHg(energy_eV/eV);
    }
        
};

#endif