#ifndef ELECTRONATTACHMENTPROCESS_HH
#define ELECTRONATTACHMENTPROCESS_HH

#include "CH4DiscreteProcess.hh"
#include "G4ParticleTable.hh"
#include "G4AnalysisManager.hh"
#include "G4RunManager.hh"



class ElectronAttachmentProcess : public CH4DiscreteProcess
{
public:
    ElectronAttachmentProcess() : CH4DiscreteProcess("EAProcess"){}
    protected:
    G4double GetCrossSection(G4double energy_eV) const override {
        return GetCrossSectionTable().GetEA(energy_eV/eV);
    }
        

    G4VParticleChange* PostStepDoIt(const G4Track& track, const G4Step&  step) override
{

    aParticleChange.Initialize(track);
    auto* am = G4AnalysisManager::Instance();

    G4double energy = track.GetKineticEnergy();
    G4ThreeVector position = step.GetPostStepPoint()->GetPosition();
    const std::vector<std::string> fProducts = {"H-", "CH3*"};


    G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
    for (const std::string& productName : fProducts) {
        G4ParticleDefinition* productDef = particleTable->FindParticle(productName);
        if (!productDef) {
            G4cerr << GetProcessName() << ": particle '" << productName
                   << "' not found in particle table." << G4endl;
            continue;
        }
        G4DynamicParticle* product =
            new G4DynamicParticle(productDef, G4ThreeVector(0, 0, 1), 0.0);
        G4Track* productTrack =
            new G4Track(product, track.GetGlobalTime(), position);
        productTrack->SetTouchableHandle(track.GetTouchableHandle());
        aParticleChange.AddSecondary(productTrack);
        am->FillNtupleIColumn(1, 0, G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID() + 1);
        am->FillNtupleDColumn(1, 1, track.GetTrackID());        //  electron ID
        am->FillNtupleDColumn(1, 2, position.x() / CLHEP::mm);
        am->FillNtupleDColumn(1, 3, position.y() / CLHEP::mm);
        am->FillNtupleDColumn(1, 4, position.z() / CLHEP::mm); 
        am->FillNtupleSColumn(1, 5, productName);
        am->FillNtupleDColumn(1, 6, track.GetGlobalTime() / CLHEP::ns);
        am->AddNtupleRow(1);
    }


    aParticleChange.ProposeLocalEnergyDeposit(energy);
    aParticleChange.ProposeEnergy(0.0);
    aParticleChange.ProposeTrackStatus(fStopAndKill);



    ClearNumberOfInteractionLengthLeft();

    return &aParticleChange;
}

};

#endif
