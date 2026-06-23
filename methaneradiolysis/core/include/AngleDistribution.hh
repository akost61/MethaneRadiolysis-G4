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

// Samples the post-scatter polar angle (beta) of the primary electron from a
// tabulated CDF loaded from CSV. Energies are stored as ln(E/eV) for
// log-linear interpolation between table columns.

class AngularDistribution {
    public:
        // Parses CSV: first row = incident energies (eV), remaining rows = angle (deg) + CDF values.
        void LoadCSV(const std::string& filename);

        // Interpolates the CDF column for a given kinetic energy (log-linear in energy).
        std::vector<G4double> SelectDistribution(G4double kineticEnergy) const;

        // Samples a scattering angle via inverse-CDF lookup.
        G4double SampleAngle(G4double kineticEnergy) const;

        // Rotates oldDirection by betaAngle with a uniformly random azimuthal angle.
        G4ThreeVector RotateDirection(const G4ThreeVector& oldDirection,
                                                        G4double kineticEnergy, G4double betaAngle) const;


    private:
        std::vector<G4double> fEnergies;                    // ln(E/eV) column headers
        std::vector<G4double> fAngles;                      // scattering angles (rad)
        std::vector<std::vector<G4double>> fCDF;            // CDF[angle_row][energy_col]
    };

#endif