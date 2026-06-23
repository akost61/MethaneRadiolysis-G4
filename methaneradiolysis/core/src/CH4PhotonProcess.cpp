#include "CH4PhotonProcess.hh"
#include "RecoilLookUp.hh"
#include "VoxelEnergyMap.hh"
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
    G4double energyLoss = fPhotonEnergy;
    G4int ix = (G4int)((position.x() + 100*cm) / (2*mm));
    G4int iy = (G4int)((position.y() + 100*cm) / (2*mm));
    G4int iz = (G4int)((position.z() + 120*cm) / (2*mm));
    auto* voxelMap = VoxelEnergyMap::GetInstance();

    if (energy <= fPhotonEnergy) {
        aParticleChange.ProposeEnergy(0.0);
        aParticleChange.ProposeLocalEnergyDeposit(energy);
        aParticleChange.ProposeTrackStatus(fStopAndKill);
        ClearNumberOfInteractionLengthLeft();
        voxelMap->Deposit(ix, iy, iz, 0.0, energy);

        return &aParticleChange;
    }
    G4double betaAngle = GetAngularDist().SampleAngle(energy);

    G4ThreeVector newDir = GetAngularDist().RotateDirection(oldDir, energy, betaAngle);
    G4double recoilEnergy = GetRecoil().SelectRecoil(energy, betaAngle);

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
    am->FillNtupleIColumn(1, 0, G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID() + 1);
    am->FillNtupleDColumn(1, 1, position.x() / mm);
    am->FillNtupleDColumn(1, 2, position.y() / mm);
    am->FillNtupleDColumn(1, 3, position.z() / mm);
    am->FillNtupleDColumn(1, 4, fPhotonEnergy / eV);
    am->FillNtupleDColumn(1, 5, track.GetGlobalTime() / ns);
    am->AddNtupleRow(1);

    energyLoss = energyLoss + recoilEnergy;

    G4double newEnergy = energy - energyLoss;

    if (newEnergy < GetCutOffEnergy()) {
        aParticleChange.ProposeLocalEnergyDeposit(recoilEnergy + newEnergy);
        aParticleChange.ProposeEnergy(0.0);
        aParticleChange.ProposeTrackStatus(fStopAndKill);
        voxelMap->Deposit(ix, iy, iz, 0.0, recoilEnergy+ newEnergy);
    } else {
        aParticleChange.ProposeMomentumDirection(newDir);
        aParticleChange.ProposeLocalEnergyDeposit(recoilEnergy);
        aParticleChange.ProposeEnergy(newEnergy);
        aParticleChange.ProposeTrackStatus(fAlive);
        voxelMap->Deposit(ix, iy, iz, 0.0, recoilEnergy);

    }

    ClearNumberOfInteractionLengthLeft();
    return &aParticleChange;
}