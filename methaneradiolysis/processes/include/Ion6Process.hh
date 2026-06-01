#ifndef ION6PROCESS_HH
#define ION6PROCESS_HH

#include "CH4IonizationProcess.hh"


//Ion_6 : CH₄ + e⁻ -> CH₂* + H₂⁺ + 2e⁻
// 


class Ion6Process : public CH4IonizationProcess {
    public:

    Ion6Process() : CH4IonizationProcess("Ion6Process", 20.42*eV, {"CH2*", "H2+"}){}

    protected:
    G4double GetCrossSection(G4double energy_eV) const override {
        return GetCrossSectionTable().GetIon6(energy_eV/eV);
    }
        

};


#endif