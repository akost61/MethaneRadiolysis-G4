#include "CH4DissociationProcess.hh"
#include "RecoilLookUp.hh"
#include "G4AnalysisManager.hh"
#include "G4RunManager.hh"
#include "G4Run.hh"
#include "VoxelEnergyMap.hh"


CH4DissociationProcess::CH4DissociationProcess(const G4String& name,
                                                G4double thresholdEnergy,
                                                const std::vector<std::string>& products)
    : CH4DiscreteProcess(name),
      fThresholdEnergy(thresholdEnergy),
      fProducts(products)
{}

G4VParticleChange* CH4DissociationProcess::PostStepDoIt(const G4Track& track,
                                                         const G4Step&  step)
{
    aParticleChange.Initialize(track);
    auto* am = G4AnalysisManager::Instance();

    G4double      energy   = track.GetKineticEnergy();
    G4ThreeVector oldDir   = track.GetMomentumDirection();
    G4ThreeVector position = step.GetPostStepPoint()->GetPosition();
    G4int ix = (G4int)((position.x() + 100*cm) / (2*mm));
    G4int iy = (G4int)((position.y() + 100*cm) / (2*mm));
    G4int iz = (G4int)((position.z() + 120*cm) / (2*mm));
    auto* voxelMap = VoxelEnergyMap::GetInstance();


    if (energy <= fThresholdEnergy) {
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

    G4double energyDeposit = fThresholdEnergy + recoilEnergy;


    // Stationary products
    G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
    for (const std::string& productName : fProducts) {
        G4ParticleDefinition* productDef =
            particleTable->FindParticle(productName);
        if (!productDef) {
            G4cerr << GetProcessName() << ": particle '" << productName
                   << "' not found in particle table." << G4endl;
            continue;
        }
        G4DynamicParticle* product =
            new G4DynamicParticle(productDef, G4ThreeVector(0,0,1), 0.0);
        G4Track* productTrack =
            new G4Track(product, track.GetGlobalTime(), position);
        productTrack->SetTouchableHandle(track.GetTouchableHandle());
        aParticleChange.AddSecondary(productTrack);
        am->FillNtupleIColumn(0, 0, G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID() + 1);
        am->FillNtupleDColumn(0, 1, track.GetTrackID());
        am->FillNtupleDColumn(0, 2, position.x() / CLHEP::mm);
        am->FillNtupleDColumn(0, 3, position.y() / CLHEP::mm);
        am->FillNtupleDColumn(0, 4, position.z() / CLHEP::mm);
        am->FillNtupleSColumn(0, 5, productName);
        am->FillNtupleDColumn(0, 6, track.GetGlobalTime() / CLHEP::ns);
        am->AddNtupleRow(0);
    }

    G4double newEnergy = energy - energyDeposit;


    if (newEnergy < GetCutOffEnergy()) {
        aParticleChange.ProposeLocalEnergyDeposit(energyDeposit + newEnergy);
        aParticleChange.ProposeEnergy(0.0);
        aParticleChange.ProposeTrackStatus(fStopAndKill);
        voxelMap->Deposit(ix, iy, iz, fThresholdEnergy, newEnergy + recoilEnergy);

    } else {
        aParticleChange.ProposeMomentumDirection(newDir);
        aParticleChange.ProposeLocalEnergyDeposit(energyDeposit);
        aParticleChange.ProposeEnergy(newEnergy);
        aParticleChange.ProposeTrackStatus(fAlive);
        voxelMap->Deposit(ix, iy, iz, fThresholdEnergy, recoilEnergy);

    }

    ClearNumberOfInteractionLengthLeft();
    return &aParticleChange;
}