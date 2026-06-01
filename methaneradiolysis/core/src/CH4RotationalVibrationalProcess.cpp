#include "CH4RotationalVibrationalProcess.hh"

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

    if (energy <= fEnergyDeposit){
        aParticleChange.ProposeEnergy(0.0);
        aParticleChange.ProposeLocalEnergyDeposit(energy);
        aParticleChange.ProposeTrackStatus(fStopAndKill);
        ClearNumberOfInteractionLengthLeft();
        return &aParticleChange;
    }

    G4double betaAngle = GetAngularDist().SampleAngle(energy);

    G4ThreeVector newDir = GetAngularDist().RotateDirection(oldDir, energy);

    G4double newEnergy  = energy - fEnergyDeposit;

    aParticleChange.ProposeMomentumDirection(newDir);
    aParticleChange.ProposeLocalEnergyDeposit(fEnergyDeposit);
    aParticleChange.ProposeEnergy(newEnergy);
    aParticleChange.ProposeTrackStatus(fAlive);

    ClearNumberOfInteractionLengthLeft();
    return &aParticleChange;
}