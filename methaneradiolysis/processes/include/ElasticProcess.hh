#ifndef ELASTICPROCESS_HH
#define ELASTICPROCESS_HH

#include "CH4DiscreteProcess.hh"
#include "RecoilLookUp.hh"
#include "VoxelEnergyMap.hh"

// CH₄ + e⁻ → CH₄ + e⁻  (elastic scattering)
// Deflects the electron and deposits recoil energy to the CH4 molecule.
// No secondary particles are produced.
class ElasticProcess : public CH4DiscreteProcess {

public:

    ElasticProcess():
    CH4DiscreteProcess("ElasticProcess"){}


    G4VParticleChange* PostStepDoIt(const G4Track& track,
                                                 const G4Step&  step) override
    {
        aParticleChange.Initialize(track);
        ClearNumberOfInteractionLengthLeft();

        G4double energy    = track.GetKineticEnergy();
        G4double betaAngle = GetAngularDist().SampleAngle(energy);
        G4ThreeVector oldDir = track.GetMomentumDirection();
        G4ThreeVector newDir = GetAngularDist().RotateDirection(oldDir, energy, betaAngle);
        G4double recoilEnergy = GetRecoil().SelectRecoil(energy, betaAngle);
        G4double newEnergy = energy - recoilEnergy;

        // --- voxel index ---
        G4ThreeVector position = step.GetPostStepPoint()->GetPosition();
        G4int ix = (G4int)((position.x() + 100*cm) / (2*mm));
        G4int iy = (G4int)((position.y() + 100*cm) / (2*mm));
        G4int iz = (G4int)((position.z() + 120*cm) / (2*mm));
        auto* voxelMap = VoxelEnergyMap::GetInstance();

        if (newEnergy < GetCutOffEnergy()) {
            aParticleChange.ProposeLocalEnergyDeposit(recoilEnergy + newEnergy);
            aParticleChange.ProposeEnergy(0.0);
            aParticleChange.ProposeTrackStatus(fStopAndKill);

            voxelMap->Deposit(ix, iy, iz, 0.0, recoilEnergy + newEnergy);

        } else {
            aParticleChange.ProposeMomentumDirection(newDir);
            aParticleChange.ProposeLocalEnergyDeposit(recoilEnergy);
            aParticleChange.ProposeEnergy(newEnergy);
            aParticleChange.ProposeTrackStatus(fAlive);

            voxelMap->Deposit(ix, iy, iz, 0.0, recoilEnergy);
        }

        return &aParticleChange;
    }

protected:

    G4double GetCrossSection(G4double energy_eV) const override {
        return GetCrossSectionTable().GetElastic(energy_eV/eV);
    }
};

#endif