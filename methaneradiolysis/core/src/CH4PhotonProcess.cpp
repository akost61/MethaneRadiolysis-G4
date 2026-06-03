#include "CH4PhotonProcess.hh"
#include "G4AnalysisManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4RunManager.hh"
#include "G4Run.hh"


CH4PhotonProcess::CH4PhotonProcess(const G4String& name,
                                   G4double photonEnergy)
    : CH4DiscreteProcess(name),
      fPhotonEnergy(photonEnergy)
{}

G4VParticleChange* CH4PhotonProcess::PostStepDoIt(const G4Track& track,
                                                   const G4Step&  step)
{
    aParticleChange.Initialize(track);


    G4double      energy   = track.GetKineticEnergy();
    G4ThreeVector oldDir   = track.GetMomentumDirection();
    G4ThreeVector position = step.GetPostStepPoint()->GetPosition();
    G4double newEnergy = energy - fPhotonEnergy;


    if (energy <= fPhotonEnergy) {
        aParticleChange.ProposeEnergy(0.0);
        aParticleChange.ProposeLocalEnergyDeposit(energy);
        aParticleChange.ProposeTrackStatus(fStopAndKill);
        ClearNumberOfInteractionLengthLeft();
        return &aParticleChange;
    }

    G4ThreeVector newDir = GetAngularDist().RotateDirection(oldDir, energy);
    G4double betaAngle = GetAngularDist().SampleAngle(energy);


    G4DynamicParticle* photon =
        new G4DynamicParticle(G4Gamma::Gamma(),
                            G4RandomDirection(),
                            fPhotonEnergy);
    G4Track* photonTrack =
        new G4Track(photon, track.GetGlobalTime(), position);
    photonTrack->SetTouchableHandle(track.GetTouchableHandle());
    aParticleChange.AddSecondary(photonTrack);
    aParticleChange.ProposeLocalEnergyDeposit(0);

    auto* am = G4AnalysisManager::Instance();
    am->FillNtupleIColumn(2, 0, G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID() + 1);
    am->FillNtupleDColumn(2, 1, position.x() / mm);
    am->FillNtupleDColumn(2, 2, position.y() / mm);
    am->FillNtupleDColumn(2, 3, position.z() / mm);
    am->FillNtupleDColumn(2, 4, fPhotonEnergy / eV);
    am->FillNtupleDColumn(2, 5, track.GetGlobalTime() / ns);
    am->AddNtupleRow(2);

    
    aParticleChange.ProposeMomentumDirection(newDir);
    aParticleChange.ProposeEnergy(newEnergy);
    aParticleChange.ProposeTrackStatus(fAlive);

    ClearNumberOfInteractionLengthLeft();
    return &aParticleChange;
}