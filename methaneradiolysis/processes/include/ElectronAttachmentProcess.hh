#ifndef ELECTRONATTACHMENTPROCESS_HH
#define ELECTRONATTACHMENTPROCESS_HH

#include "CH4DiscreteProcess.hh"
#include "G4ParticleTable.hh"
#include "G4AnalysisManager.hh"
#include "G4RunManager.hh"
#include "RecoilLookUp.hh"

// CH₄ + e⁻ → H⁻ + CH₃*  (electron attachment, EA channel)
// The electron is absorbed entirely — the track is stopped and killed.
// Products H- and CH3* are created as stationary secondaries and recorded to the ROOT ntuple.
// 3.93 eV is deposited as chemical potential energy; the remainder is kinetic energy loss.
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
    G4double energyPE = 3.93 * eV;
    G4ThreeVector position = step.GetPostStepPoint()->GetPosition();
    const std::vector<std::string> fProducts = {"H-", "CH3*"};

    G4int ix = (G4int)((position.x() + 100*cm) / (2*mm));
    G4int iy = (G4int)((position.y() + 100*cm) / (2*mm));
    G4int iz = (G4int)((position.z() + 120*cm) / (2*mm));
    auto* voxelMap = VoxelEnergyMap::GetInstance();

    voxelMap->Deposit(ix, iy, iz, energyPE, energy - energyPE);


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
        am->FillNtupleIColumn(0, 0, G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID() + 1);
        am->FillNtupleDColumn(0, 1, track.GetTrackID());        //  electron ID
        am->FillNtupleDColumn(0, 2, position.x() / CLHEP::mm);
        am->FillNtupleDColumn(0, 3, position.y() / CLHEP::mm);
        am->FillNtupleDColumn(0, 4, position.z() / CLHEP::mm); 
        am->FillNtupleSColumn(0, 5, productName);
        am->FillNtupleDColumn(0, 6, track.GetGlobalTime() / CLHEP::ns);
        am->AddNtupleRow(0);
    }


    aParticleChange.ProposeLocalEnergyDeposit(energy);
    aParticleChange.ProposeEnergy(0.0);
    aParticleChange.ProposeTrackStatus(fStopAndKill);



    ClearNumberOfInteractionLengthLeft();

    return &aParticleChange;
}

};

#endif
