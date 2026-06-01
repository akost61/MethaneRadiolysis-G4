#ifndef ION3PROCESS_HH
#define ION3PROCESS_HH


#include "CH4IonizationProcess.hh"


//Ion_3 : CH₄ + e⁻ -> CH₂⁺ + H₂ +  2e⁻
// 


class Ion3Process : public CH4IonizationProcess {
public:

    Ion3Process() : CH4IonizationProcess("Ion3Process", 15.30*eV, {"CH2+", "H2"}){}

protected:
    G4double GetCrossSection(G4double energy_eV) const override {
        return GetCrossSectionTable().GetIon3(energy_eV/eV);
    }
        
};

#endif