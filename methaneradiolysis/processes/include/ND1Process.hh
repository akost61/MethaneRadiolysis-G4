#ifndef ND1PROCESS_HH
#define ND1PROCESS_HH

#include "CH4DissociationProcess.hh"
#include "G4SystemOfUnits.hh"

class ND1Process : public CH4DissociationProcess {
public:
    ND1Process(): CH4DissociationProcess("ND1Process",
                                  4.68 * eV,
                                  {"CH3*", "H*"}){}
    protected:
    G4double GetCrossSection(G4double energy_eV) const override {
        return GetCrossSectionTable().GetND1(energy_eV/eV);
    }
        
};

#endif