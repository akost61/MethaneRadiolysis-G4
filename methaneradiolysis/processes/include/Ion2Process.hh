#ifndef ION2PROCESS_HH
#define ION2PROCESS_HH
#include "CH4IonizationProcess.hh"


//Ion_2 : CH₄ + e⁻ -> CH₃⁺ + H* +  2e⁻
// 


class Ion2Process : public CH4IonizationProcess {
public:
    Ion2Process() : CH4IonizationProcess("Ion2Process", 14.52*eV, {"CH3+", "H*"}) {}
protected:
    G4double GetCrossSection(G4double energy_eV) const override {
        return GetCrossSectionTable().GetIon2(energy_eV/eV);
    }
        
};


#endif