#ifndef ND3PROCESS_HH
#define ND3PROCESS_HH

#include "CH4DissociationProcess.hh"


//EIE3 : CH₄ + e⁻ -> CH* + H₂ + H* + e⁻
class ND3Process : public CH4DissociationProcess
{
public:
    ND3Process() : CH4DissociationProcess("ND3Process", 9.46*eV, {"CH*", "H2", "H*"}){}
    protected:
    G4double GetCrossSection(G4double energy_eV) const override {
        return GetCrossSectionTable().GetND3(energy_eV/eV);
    }
        

};

#endif