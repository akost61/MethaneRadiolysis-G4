#ifndef ION4PROCESS_HH
#define ION4PROCESS_HH

#include "CH4IonizationProcess.hh"

//Ion_4 : CH₄ + e⁻ -> CH₃* + H⁺ +  2e⁻
// 



class Ion4Process : public CH4IonizationProcess {
public:

    Ion4Process() : CH4IonizationProcess("Ion4Process", 18.28*eV, {"CH3*", "H+"}){}
    
    protected:
    G4double GetCrossSection(G4double energy_eV) const override {
        return GetCrossSectionTable().GetIon4(energy_eV/eV);
    }
        

};


#endif