#ifndef CH4DISCRETEPROCESS_HH
#define CH4DISCRETEPROCESS_HH

#include "G4VDiscreteProcess.hh"
#include "G4Track.hh"
#include "G4Step.hh"
#include "G4ParticleDefinition.hh"
#include "G4Electron.hh"
#include "G4Material.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"
#include "AngleDistribution.hh"
#include "RecoilLookUp.hh"
#include "CrossSectionTable.hh"

// Abstract base class for all electron-impact processes on CH4.
// Inherits from G4VDiscreteProcess and restricts applicability to electrons.
// Subclasses must implement GetCrossSection() and PostStepDoIt().
//
// Lookup tables (angular distribution, recoil energies, cross-sections) are
// shared across all process instances via a singleton (SharedData) so CSV files
// are only read once per simulation run.

class CH4DiscreteProcess : public G4VDiscreteProcess {

public:

    // Singleton holding shared lookup tables loaded at first instantiation.
    struct SharedData {
        static SharedData& Instance() {
            static SharedData instance;
            return instance;
        }

        AngularDistribution angularDist;
        RecoilLookUp        recoil;
        CrossSectionTable   table;

    private:
        SharedData() {
            angularDist.LoadCSV("Tables/beta_angle.csv");
            recoil.LoadCSV("Tables/recoil_energy.csv");
            table.LoadFromCSV("Tables/cross_section.csv");
        }

        SharedData(const SharedData&)            = delete;
        SharedData& operator=(const SharedData&) = delete;
        SharedData(SharedData&&)                 = delete;
        SharedData& operator=(SharedData&&)      = delete;
    };

    explicit CH4DiscreteProcess(const G4String& name);
    ~CH4DiscreteProcess() override = default;

    // Energy below which the electron track is stopped and killed rather than scattered.
    static void    SetCutOffEnergy(G4double e) { fCutOffEnergy = e; }
    static G4double GetCutOffEnergy()           { return fCutOffEnergy; }

    // Returns true only for electrons.
    G4bool IsApplicable(const G4ParticleDefinition& particle) override;

    // Computes the mean free path from the subclass cross-section and CH4 molecular number density.
    G4double GetMeanFreePath(const G4Track&    track,
                             G4double          previousStepSize,
                             G4ForceCondition* condition) override;

    G4VParticleChange* PostStepDoIt(const G4Track& track,
                                    const G4Step&  step)         override = 0;

protected:

    static G4double fCutOffEnergy;

    virtual G4double GetCrossSection(G4double energy_eV) const = 0;

    AngularDistribution& GetAngularDist() const {
        return SharedData::Instance().angularDist;
    }

    RecoilLookUp& GetRecoil() const {
        return SharedData::Instance().recoil;
    }

    CrossSectionTable& GetCrossSectionTable() const {
        return SharedData::Instance().table;
    }
};

#endif // CH4DISCRETEPROCESS_HH