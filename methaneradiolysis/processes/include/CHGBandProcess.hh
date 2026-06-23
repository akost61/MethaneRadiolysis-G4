#ifndef CHGBAND_PROCESS_HH
#define CHGBAND_PROCESS_HH

#include "CH4PhotonProcess.hh"

// CH G-band emission (B²Σ⁻→X²Π electronic transition)
class CHGBandProcess : public CH4PhotonProcess
{
public:
    CHGBandProcess(): CH4PhotonProcess("CH G-Band", 2.88*eV){}
    protected:
    G4double GetCrossSection(G4double energy_eV) const override {
        return GetCrossSectionTable().GetCHG(energy_eV/eV);
    }
        

};

#endif