#include "stepping.hh"
#include "G4Step.hh"
#include "G4Track.hh"
#include "G4StepPoint.hh"
#include "G4VProcess.hh"
#include "G4SystemOfUnits.hh"
#include "G4RunManager.hh"
#include "G4Run.hh"
#include "G4AnalysisManager.hh"
#include "G4Electron.hh"

SteppingAction::SteppingAction(G4double cutOffEnergy) 
: G4UserSteppingAction(), fCutOffEnergy(cutOffEnergy)
{
}

SteppingAction::~SteppingAction()
{
}

void SteppingAction::UserSteppingAction(const G4Step* step)
{
    const G4Track* track = step->GetTrack();

    // ── Escape check (before Transportation filter) ──────────────────
    if (step->GetPostStepPoint()->GetPhysicalVolume() == nullptr)
    {
        if (track->GetDefinition() == G4Electron::Definition())
        {
            G4double escapeEnergy = step->GetPostStepPoint()->GetKineticEnergy();
            G4ThreeVector position = step->GetPostStepPoint()->GetPosition();
            auto analysisManager  = G4AnalysisManager::Instance();

            analysisManager->FillNtupleIColumn(0,0, G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID() + 1);
            analysisManager->FillNtupleDColumn(0,1, track->GetTrackID());
            analysisManager->FillNtupleDColumn(0,2, track->GetParentID());
            analysisManager->FillNtupleSColumn(0,3, G4String("Escaped"));
            analysisManager->FillNtupleDColumn(0,4, position.x()/mm);
            analysisManager->FillNtupleDColumn(0,5, position.y()/mm);
            analysisManager->FillNtupleDColumn(0,6, position.z()/mm);
            analysisManager->FillNtupleDColumn(0,7, escapeEnergy/eV);
            analysisManager->FillNtupleDColumn(0,8, track->GetTrackLength()/mm);
            analysisManager->FillNtupleDColumn(0,9, escapeEnergy/eV);
            analysisManager->AddNtupleRow(0);
        }
        return;
    }
    // ─────────────────────────────────────────────────────────────────

    

    const G4VProcess* postProcess = step->GetPostStepPoint()->GetProcessDefinedStep();
    if (!postProcess) return;
    G4String processName = postProcess->GetProcessName();
    if (processName == "Transportation") return;

    if (track->GetDefinition() == G4Electron::Definition()) {
        G4double edep = step->GetTotalEnergyDeposit();
        G4double kineticEnergy = track->GetKineticEnergy();
        
        if (track->GetKineticEnergy() < fCutOffEnergy) {
            edep += track->GetKineticEnergy();
            kineticEnergy = 0.0 * eV;
            const_cast<G4Track*>(track)->SetTrackStatus(fStopAndKill);
        }

        if (processName != "ElasticProcess"){

            G4ThreeVector position = step->GetPostStepPoint()->GetPosition();
            auto analysisManager = G4AnalysisManager::Instance();
            
            analysisManager->FillNtupleIColumn(0, 0, G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID() + 1);
            analysisManager->FillNtupleDColumn(0, 1, track->GetTrackID());
            analysisManager->FillNtupleDColumn(0, 2, track->GetParentID());
            analysisManager->FillNtupleSColumn(0, 3, processName);
            analysisManager->FillNtupleDColumn(0, 4, position.x()/mm);
            analysisManager->FillNtupleDColumn(0, 5, position.y()/mm);
            analysisManager->FillNtupleDColumn(0, 6, position.z()/mm);
            analysisManager->FillNtupleDColumn(0, 7, edep/eV);
            analysisManager->FillNtupleDColumn(0, 8, track->GetTrackLength()/mm);
            analysisManager->FillNtupleDColumn(0, 9, kineticEnergy/eV);
            analysisManager->AddNtupleRow(0); 
        }
    }
}