#include "run.hh"
#include "G4Run.hh"

RunAction::RunAction() : G4UserRunAction()
{

    auto analysisManager = G4AnalysisManager::Instance();
    analysisManager->SetNtupleMerging(true);
    analysisManager->SetVerboseLevel(1);
    analysisManager->SetFileName("output.root");
    
    analysisManager->CreateNtuple("electrons", "All Electrons");
    analysisManager->CreateNtupleIColumn("runID");        // 0
    analysisManager->CreateNtupleDColumn("electronID");   // 1
    analysisManager->CreateNtupleDColumn("parentID");     // 2
    analysisManager->CreateNtupleSColumn("proc");         // 3
    analysisManager->CreateNtupleDColumn("x");            // 4
    analysisManager->CreateNtupleDColumn("y");            // 5
    analysisManager->CreateNtupleDColumn("z");            // 6
    analysisManager->CreateNtupleDColumn("edep");         // 7
    analysisManager->CreateNtupleDColumn("length");       // 8
    analysisManager->CreateNtupleDColumn("energy");       // 9
    analysisManager->FinishNtuple();  // ntuple ID = 0

    analysisManager->CreateNtuple("particles", "Secondary Particle Creation");
    analysisManager->CreateNtupleIColumn("runID");           // 0
    analysisManager->CreateNtupleDColumn("electronTrackID"); // 1
    analysisManager->CreateNtupleDColumn("x");               // 2
    analysisManager->CreateNtupleDColumn("y");               // 3
    analysisManager->CreateNtupleDColumn("z");               // 4
    analysisManager->CreateNtupleSColumn("particleName");    // 5
    analysisManager->CreateNtupleDColumn("globalTime_ns");   // 6
    analysisManager->FinishNtuple();  // ntuple ID = 1

    analysisManager->CreateNtuple("photons", "Photon Creation");
    analysisManager->CreateNtupleIColumn("runID");        // 0
    analysisManager->CreateNtupleDColumn("x");            // 1
    analysisManager->CreateNtupleDColumn("y");            // 2
    analysisManager->CreateNtupleDColumn("z");            // 3
    analysisManager->CreateNtupleDColumn("energy");       // 4
    analysisManager->CreateNtupleDColumn("globalTime_ns");// 5
    analysisManager->FinishNtuple();  // ntuple ID = 2
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