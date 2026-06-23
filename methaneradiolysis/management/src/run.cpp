#include "run.hh"
#include "G4Run.hh"
#include "G4MTRunManager.hh"
#include "G4AnalysisManager.hh"
#include "G4SystemOfUnits.hh"
#include "VoxelEnergyMap.hh"

RunAction::RunAction() : G4UserRunAction()
{
    auto analysisManager = G4AnalysisManager::Instance();
    analysisManager->SetNtupleMerging(true);
    analysisManager->SetVerboseLevel(1);
    analysisManager->SetFileName("output.root");

    analysisManager->CreateNtuple("particles", "Secondary Particle Creation");
    analysisManager->CreateNtupleIColumn("runID");
    analysisManager->CreateNtupleDColumn("electronTrackID");
    analysisManager->CreateNtupleDColumn("x");
    analysisManager->CreateNtupleDColumn("y");
    analysisManager->CreateNtupleDColumn("z");
    analysisManager->CreateNtupleSColumn("particleName");
    analysisManager->CreateNtupleDColumn("globalTime_ns");
    analysisManager->FinishNtuple(0);

    analysisManager->CreateNtuple("photons", "Photon Creation");
    analysisManager->CreateNtupleIColumn("runID");
    analysisManager->CreateNtupleDColumn("x");
    analysisManager->CreateNtupleDColumn("y");
    analysisManager->CreateNtupleDColumn("z");
    analysisManager->CreateNtupleDColumn("energy");
    analysisManager->CreateNtupleDColumn("globalTime_ns");
    analysisManager->FinishNtuple(1);

    analysisManager->CreateNtuple("escaped", "Energy Escapes Methane");
    analysisManager->CreateNtupleIColumn("runID");
    analysisManager->CreateNtupleDColumn("energy");
    analysisManager->FinishNtuple(2);

}

RunAction::~RunAction() {}

void RunAction::BeginOfRunAction(const G4Run*)
{
    auto analysisManager = G4AnalysisManager::Instance();
    analysisManager->OpenFile();


    fMap = VoxelEnergyMap::GetInstance();
    fMap->SetDimensions(1000, 1000, 1200);
}

void RunAction::EndOfRunAction(const G4Run*)
{
    auto analysisManager = G4AnalysisManager::Instance();
    analysisManager->Write();
    analysisManager->CloseFile();

    if (!IsMaster()) {
        // get the master RunAction and merge worker map into it
        const auto* masterAction = static_cast<const RunAction*>(
            G4MTRunManager::GetMasterRunManager()->GetUserRunAction());
        masterAction->GetMap()->Merge(fMap);
    } else {

        fMap->SaveToFile("voxel_map.txt");
        G4cout << "[VoxelEnergyMap] " << fMap->OccupiedVoxels()
               << " voxels hit, saved to voxel_map.txt" << G4endl;
    }
}