#ifndef ANGLEDISTRIBUTION_HH
#define ANGLEDISTRIBUTION_HH
#include <vector>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include "Randomize.hh"
#include "G4SystemOfUnits.hh"
#include "G4Types.hh"
#include "G4ThreeVector.hh"

class AngularDistribution {
    public:
        void LoadCSV(const std::string& filename);
        std::vector<G4double> SelectDistribution(G4double kineticEnergy) const;
        G4double SampleAngle(G4double kineticEnergy) const;
        G4ThreeVector RotateDirection(const G4ThreeVector& oldDirection,
                                                        G4double kineticEnergy) const;

                                
    private:
        std::vector<G4double> fEnergies;
        std::vector<G4double> fAngles;
        std::vector<std::vector<G4double>> fCDF;
    };

#endif