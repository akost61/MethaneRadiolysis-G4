#ifndef ND2PROCESS_HH
#define ND2PROCESS_HH

#include "CH4DissociationProcess.hh"


//EIE2 : CH₄ + e⁻ -> CH₂* + H₂ + e⁻
// 

class ND2Process : public CH4DissociationProcess
{
public:
    ND2Process(): CH4DissociationProcess("ND2Process", 4.95*eV, {"CH2*", "H2"}){}
    protected:
    G4double GetCrossSection(G4double energy_eV) const override {
        return GetCrossSectionTable().GetND2(energy_eV/eV);
    }
        
};

#endif