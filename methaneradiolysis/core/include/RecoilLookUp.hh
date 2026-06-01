#ifndef RECOILLOOKUP_HH
#define RECOILLOOKUP_HH
#include <vector>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include "G4SystemOfUnits.hh"
#include "G4Types.hh"

class RecoilLookUp {
    public:
        void LoadCSV(const std::string& filename);
        std::vector<G4double> SelectDistribution(G4double kineticEnergy) const;
        G4double SelectRecoil(G4double kineticEnergy, G4double betaAngle) const;

    private:
        std::vector<G4double> fEnergies;
        std::vector<G4double> fAngles;
        std::vector<std::vector<G4double>> fRecoil;
    
};




#endif