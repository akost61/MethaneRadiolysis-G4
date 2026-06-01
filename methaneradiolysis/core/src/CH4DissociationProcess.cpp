#include "CH4DissociationProcess.hh"
#include "G4AnalysisManager.hh"
#include "G4RunManager.hh"
#include "G4Run.hh"


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
    G4double newEnergy = energy - fThresholdEnergy;

    if (energy <= fThresholdEnergy) {
        aParticleChange.ProposeEnergy(0.0);
        aParticleChange.ProposeLocalEnergyDeposit(energy);
        aParticleChange.ProposeTrackStatus(fStopAndKill);
        ClearNumberOfInteractionLengthLeft();
        return &aParticleChange;
    }


    G4double betaAngle = GetAngularDist().SampleAngle(energy);

    G4ThreeVector newDir = GetAngularDist().RotateDirection(oldDir, energy);

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
        am->FillNtupleDColumn(1, 0, position.x() / CLHEP::mm);
        am->FillNtupleDColumn(1, 1, position.y() / CLHEP::mm);
        am->FillNtupleDColumn(1, 2, position.z() / CLHEP::mm);
        am->FillNtupleDColumn(1, 3, track.GetTrackID());        // electron ID
        am->FillNtupleSColumn(1, 4, productName);
        am->FillNtupleDColumn(1, 5, track.GetGlobalTime() / CLHEP::ns);
        am->FillNtupleIColumn(1, 6, G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID() + 1);
        am->AddNtupleRow(1);
    }




    aParticleChange.ProposeMomentumDirection(newDir);
    aParticleChange.ProposeLocalEnergyDeposit(fThresholdEnergy);
    aParticleChange.ProposeEnergy(newEnergy);
    aParticleChange.ProposeTrackStatus(fAlive);
    ClearNumberOfInteractionLengthLeft();
    return &aParticleChange;
}