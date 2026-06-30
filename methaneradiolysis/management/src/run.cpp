#include "run.hh"
#include "G4Run.hh"
#include "G4MTRunManager.hh"
#include "G4SystemOfUnits.hh"
#include "VoxelEnergyMap.hh"
#include <fstream>
#include <sstream>


RunAction::RunAction()
    : fSteppingAction(nullptr), fFilename("process_counts.csv")
{
}

RunAction::RunAction(SteppingAction* sa)
    : fSteppingAction(sa), fFilename("process_counts.csv")
{
}


RunAction::~RunAction() {}

void RunAction::BeginOfRunAction(const G4Run*)
{
    if (fSteppingAction) fSteppingAction->ResetCounts();

    fMap = VoxelEnergyMap::GetInstance();
    fMap->SetDimensions(1000, 1000, 1200);
}

void RunAction::EndOfRunAction(const G4Run*)
{
    if (!IsMaster()) {
        const auto* masterAction = static_cast<const RunAction*>(
            G4MTRunManager::GetMasterRunManager()->GetUserRunAction());
        masterAction->GetMap()->Merge(fMap);
        if (fSteppingAction)
            masterAction->MergeCounts(fSteppingAction->GetProcessCounts());
    } else {
        fMap->SaveToFile("voxel_map.txt");
        G4cout << "[VoxelEnergyMap] " << fMap->OccupiedVoxels()
               << " voxels hit, saved to voxel_map.txt" << G4endl;
        if (!fBlockCounts.empty()) {
            WriteRow();
            fBlockCounter++;
            fBlockCounts.clear();
        }
    }
}

void RunAction::MergeCounts(const std::map<std::string, int>& counts) const
{
    std::lock_guard<std::mutex> lock(fMutex);
    for (const auto& [name, count] : counts)
        fBlockCounts[name] += count;
}

void RunAction::WriteRow()
{
    bool fileExists = std::filesystem::exists(fFilename);

    if (!fileExists) {
        fColumnNames.clear();
        for (const auto& [name, count] : fBlockCounts)
            fColumnNames.push_back(name);

        std::ofstream hdr(fFilename);
        hdr << "block,";
        for (const auto& name : fColumnNames)
            hdr << name << ",";
        hdr << "\n";
    } else if (fColumnNames.empty()) {
        std::ifstream in(fFilename);
        std::string header;
        std::getline(in, header);
        std::istringstream ss(header);
        std::string token;
        std::getline(ss, token, ','); // skip "block"
        while (std::getline(ss, token, ','))
            if (!token.empty()) fColumnNames.push_back(token);
    }

    std::ofstream file(fFilename, std::ios::app);
    file << (fBlockCounter + 1) << ",";
    for (const auto& name : fColumnNames) {
        auto it = fBlockCounts.find(name);
        file << (it != fBlockCounts.end() ? it->second : 0) << ",";
    }
    file << "\n";
    file.close();
}
