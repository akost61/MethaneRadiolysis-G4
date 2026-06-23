#ifndef RECOILLOOKUP_HH
#define RECOILLOOKUP_HH
#include <vector>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include "G4SystemOfUnits.hh"
#include "G4Types.hh"

// Returns the kinetic energy transferred to the CH4 molecule (recoil) for a
// given incident electron energy and scattering angle. Values are looked up from
// a 2D table (angle rows x energy columns) and interpolated with log-linear
// weighting in energy and linear weighting in angle.
class RecoilLookUp {
    public:
        // Parses CSV: first row = incident energies (eV), remaining rows = angle (deg) + recoil energies (eV).
        void LoadCSV(const std::string& filename);

        // Interpolates the recoil-vs-angle column for a given kinetic energy (log-linear in energy).
        std::vector<G4double> SelectDistribution(G4double kineticEnergy) const;

        // Returns the recoil energy (in G4 units) for the given energy and scattering angle.
        G4double SelectRecoil(G4double kineticEnergy, G4double betaAngle) const;

    private:
        std::vector<G4double> fEnergies;                    // ln(E/eV) column headers
        std::vector<G4double> fAngles;                      // scattering angles (rad)
        std::vector<std::vector<G4double>> fRecoil;         // recoil[angle_row][energy_col] in eV

};




#endif