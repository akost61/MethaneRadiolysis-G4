#include "run.hh"
#include "G4Run.hh"

RunAction::RunAction() : G4UserRunAction()
{

    auto analysisManager = G4AnalysisManager::Instance();
    analysisManager->SetNtupleMerging(true);
    analysisManager->SetVerboseLevel(1);
    analysisManager->SetFileName("output.root");
    
    if (isMaster) {
        analysisManager->CreateNtuple("electrons", "All Electrons");
        analysisManager->CreateNtupleDColumn("energy");
        analysisManager->CreateNtupleDColumn("x");
        analysisManager->CreateNtupleDColumn("y");
        analysisManager->CreateNtupleDColumn("z");
        analysisManager->CreateNtupleDColumn("edep");
        analysisManager->CreateNtupleDColumn("trackID");
        analysisManager->CreateNtupleDColumn("parentID");
        analysisManager->CreateNtupleDColumn("length");
        analysisManager->CreateNtupleSColumn("proc");
        analysisManager->CreateNtupleIColumn("runID");
        analysisManager->FinishNtuple();  // ntuple ID = 0

        analysisManager->CreateNtuple("particles", "Secondary Particle Creation");
        analysisManager->CreateNtupleDColumn("x");
        analysisManager->CreateNtupleDColumn("y");
        analysisManager->CreateNtupleDColumn("z");
        analysisManager->CreateNtupleDColumn("electronTrackID");
        analysisManager->CreateNtupleSColumn("particleName");
        analysisManager->CreateNtupleDColumn(1, "globalTime_ns");
        analysisManager->CreateNtupleIColumn(1, "runID");

        analysisManager->FinishNtuple();  // ntuple ID = 1

        analysisManager->CreateNtuple("photons", "Photon Creation");
        analysisManager->CreateNtupleDColumn("x");
        analysisManager->CreateNtupleDColumn("y");
        analysisManager->CreateNtupleDColumn("z");
        analysisManager->CreateNtupleDColumn("energy");
        analysisManager->CreateNtupleDColumn("globalTime_ns");
        analysisManager->CreateNtupleIColumn("runID");
        analysisManager->FinishNtuple();  // ntuple ID = 2
    }
}

RunAction::~RunAction(){}

void RunAction::BeginOfRunAction(const G4Run*)
{
    auto analysisManager = G4AnalysisManager::Instance();
    analysisManager->OpenFile();
}

void RunAction::EndOfRunAction(const G4Run*)
{
    auto analysisManager = G4AnalysisManager::Instance();
    analysisManager->Write();
    analysisManager->CloseFile();
}