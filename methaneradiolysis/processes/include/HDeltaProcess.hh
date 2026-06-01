#ifndef HDELTA_PROCESS_HH
#define HDELTA_PROCESS_HH

#include "CH4PhotonProcess.hh"


class HDeltaProcess : public CH4PhotonProcess
{
public:
    HDeltaProcess(): CH4PhotonProcess("H-delta", 3.03*eV){}
protected:
    G4double GetCrossSection(G4double energy_eV) const override {
        return GetCrossSectionTable().GetHd(energy_eV/eV);
    }
        
};

#endif