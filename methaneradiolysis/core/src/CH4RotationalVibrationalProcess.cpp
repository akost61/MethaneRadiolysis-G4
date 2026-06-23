#include "CH4RotationalVibrationalProcess.hh"
#include "VoxelEnergyMap.hh"

CH4RotationalVibrationalProcess::CH4RotationalVibrationalProcess(const G4String& name, G4double energyDeposit)
    : CH4DiscreteProcess(name),
      fEnergyDeposit(energyDeposit)
{}

G4VParticleChange* CH4RotationalVibrationalProcess::PostStepDoIt(const G4Track& track,
                                                       const G4Step&  step)
{
    aParticleChange.Initialize(track);

    G4double      energy   = track.GetKineticEnergy();
    G4ThreeVector oldDir   = track.GetMomentumDirection();
    G4ThreeVector position = step.GetPostStepPoint()->GetPosition();
    G4double      energyDeposit = fEnergyDeposit;
    G4int ix = (G4int)((position.x() + 100*cm) / (2*mm));
    G4int iy = (G4int)((position.y() + 100*cm) / (2*mm));
    G4int iz = (G4int)((position.z() + 120*cm) / (2*mm));
    auto* voxelMap = VoxelEnergyMap::GetInstance();


    if (energy <= fEnergyDeposit){
        aParticleChange.ProposeEnergy(0.0);
        aParticleChange.ProposeLocalEnergyDeposit(energy);
        aParticleChange.ProposeTrackStatus(fStopAndKill);
        ClearNumberOfInteractionLengthLeft();
        voxelMap->Deposit(ix, iy, iz, 0.0, energy);
        return &aParticleChange;
    }

    G4double betaAngle = GetAngularDist().SampleAngle(energy);

    G4double recoilEnergy = GetRecoil().SelectRecoil(energy, betaAngle);
    G4ThreeVector newDir = GetAngularDist().RotateDirection(oldDir, energy, betaAngle);
    energyDeposit = energyDeposit + recoilEnergy;
    G4double newEnergy  = energy - energyDeposit;

    if (newEnergy < GetCutOffEnergy()) {
        aParticleChange.ProposeLocalEnergyDeposit(energyDeposit + newEnergy);
        aParticleChange.ProposeEnergy(0.0);
        aParticleChange.ProposeTrackStatus(fStopAndKill);
        voxelMap->Deposit(ix, iy, iz, 0.0, energyDeposit + newEnergy);
    } else {
        aParticleChange.ProposeMomentumDirection(newDir);
        aParticleChange.ProposeLocalEnergyDeposit(energyDeposit);
        aParticleChange.ProposeEnergy(newEnergy);
        aParticleChange.ProposeTrackStatus(fAlive);
        voxelMap->Deposit(ix, iy, iz, 0.0, energyDeposit);


    }

    ClearNumberOfInteractionLengthLeft();
    return &aParticleChange;
}