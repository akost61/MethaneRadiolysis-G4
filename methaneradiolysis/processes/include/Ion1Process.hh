#ifndef ION1PROCESS_HH
#define ION1PROCESS_HH

#include "CH4IonizationProcess.hh"
#include "G4SystemOfUnits.hh"

//Ion_1 : CH₄ + e⁻ -> CH4⁺ +  2e⁻

class Ion1Process : public CH4IonizationProcess {
public:
    Ion1Process()
    : CH4IonizationProcess("Ion1Process",
                            12.6 * eV,
                            {"CH4+"}){}
    protected:
    G4double GetCrossSection(G4double energy_eV) const override {
        return GetCrossSectionTable().GetIon1(energy_eV/eV);
    }
        
};

#endif