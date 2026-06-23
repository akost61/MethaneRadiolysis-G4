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
#include "G4VParticleChange.hh"
#include "G4AnalysisManager.hh"


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

    auto* am = G4AnalysisManager::Instance();

    aParticleChange.Initialize(*track);

    if (step->GetPostStepPoint()->GetPhysicalVolume() == nullptr)
    {
        if (track->GetDefinition() == G4Electron::Definition()){
            am->FillNtupleIColumn(2, 0, G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID() + 1);
            am->FillNtupleDColumn(2, 1, step->GetPostStepPoint()->GetKineticEnergy() / eV );
            am->AddNtupleRow(2);

        }
        return;
    }

}
