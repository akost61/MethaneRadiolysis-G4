#ifndef HBETA_PROCESS_HH
#define HBETA_PROCESS_HH


#include "CH4PhotonProcess.hh"

// Balmer-beta (n=4→2) photon emission from excited H*
class HBetaProcess : public CH4PhotonProcess
{
public:
    HBetaProcess(): CH4PhotonProcess("H-beta", 2.55*eV) {}
protected:
    G4double GetCrossSection(G4double energy_eV) const override {
        return GetCrossSectionTable().GetHb(energy_eV/eV);
    }
        

};

#endif