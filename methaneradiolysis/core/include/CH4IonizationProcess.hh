#ifndef CH4IONIZATIONPROCESS_HH
#define CH4IONIZATIONPROCESS_HH

#include "CH4DiscreteProcess.hh"
#include <vector>
#include <string>

class CH4IonizationProcess : public CH4DiscreteProcess {

public:
    explicit CH4IonizationProcess(const G4String&                 name,
                                  G4double                        ionizationPotential,
                                  const std::vector<std::string>& products);
    ~CH4IonizationProcess() override = default;

    G4double           GetCrossSection(G4double energy_eV) const override = 0;
    G4VParticleChange* PostStepDoIt(const G4Track& track,
                                    const G4Step&  step)   override;

    static void SetSecondaryMinEnergy(G4double e) { fgSecondaryMinEnergy = e; }

protected:
    G4double                 fIonizationPotential;
    std::vector<std::string> fProducts;

    static G4double fgSecondaryMinEnergy;

    G4ThreeVector RotateSecondaryDirection(const G4ThreeVector& oldDirection);

    G4double SecondaryAverageEnergy(G4double kineticEnergy) const;
    G4double GetSecondaryEnergy(G4double kineticEnergy) const;
};

#endif // CH4IONIZATIONPROCESS_HH