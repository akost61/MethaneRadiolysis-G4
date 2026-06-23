#ifndef CH4IONIZATIONPROCESS_HH
#define CH4IONIZATIONPROCESS_HH

#include "CH4DiscreteProcess.hh"
#include <vector>
#include <string>

// Handles ionization channels (Ion_1 through Ion_7) of CH4 by electron impact.
// A delta electron is emitted and the parent electron loses
// ionizationPotential + recoilEnergy + secondaryEnergy. Ion fragment products
// are created as stationary secondaries and recorded to the ROOT ntuple.
class CH4IonizationProcess : public CH4DiscreteProcess {

public:
    // ionizationPotential: orbital binding energy for this channel.
    // products: Geant4 particle names of the ion fragments produced.
    explicit CH4IonizationProcess(const G4String&                 name,
                                  G4double                        ionizationPotential,
                                  const std::vector<std::string>& products);
    ~CH4IonizationProcess() override = default;

    G4double           GetCrossSection(G4double energy_eV) const override = 0;
    G4VParticleChange* PostStepDoIt(const G4Track& track,
                                    const G4Step&  step)   override;

    // Minimum kinetic energy allowed for the ejected secondary electron.
    static void SetSecondaryMinEnergy(G4double e) { fgSecondaryMinEnergy = e; }

protected:
    G4double                 fIonizationPotential;   // orbital binding energy
    std::vector<std::string> fProducts;              // ion fragment particle names

    static G4double fgSecondaryMinEnergy;

    // Samples an isotropic direction for the secondary (delta) electron.
    G4ThreeVector RotateSecondaryDirection(const G4ThreeVector& oldDirection);

    // Cross-section-weighted average energy loss allocated to the secondary electron.
    G4double SecondaryAverageEnergy(G4double kineticEnergy) const;

    // Samples secondary energy from a power-law distribution between fgSecondaryMinEnergy
    // and (kineticEnergy - fIonizationPotential)/2, centred on SecondaryAverageEnergy.
    G4double GetSecondaryEnergy(G4double kineticEnergy) const;
};

#endif // CH4IONIZATIONPROCESS_HH