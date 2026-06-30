#include "run.hh"
#include "G4Run.hh"
#include "G4MTRunManager.hh"
#include "G4SystemOfUnits.hh"
#include "VoxelEnergyMap.hh"
#include <algorithm>
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
    fMap = VoxelEnergyMap::GetInstance();
    fMap->SetDimensions(1000, 1000, 1200);
}

void RunAction::EndOfRunAction(const G4Run*)
{
    if (!IsMaster()) {
        const auto* masterAction = static_cast<const RunAction*>(
            G4MTRunManager::GetMasterRunManager()->GetUserRunAction());
        masterAction->GetMap()->Merge(fMap);
        masterAction->MergeEventData(fEventData);
        fEventData.clear();
    } else {
        fMap->SaveToFile("voxel_map.txt");
        G4cout << "[VoxelEnergyMap] " << fMap->OccupiedVoxels()
               << " voxels hit, saved to voxel_map.txt" << G4endl;
        WriteAllRows();
        fAllEventData.clear();
    }
}

void RunAction::RecordEventCounts(int eventID, const std::map<std::string, int>& counts)
{
    fEventData.push_back({eventID, counts});
}

void RunAction::MergeEventData(const std::vector<std::pair<int, std::map<std::string, int>>>& data) const
{
    std::lock_guard<std::mutex> lock(fMutex);
    fAllEventData.insert(fAllEventData.end(), data.begin(), data.end());
}

void RunAction::WriteAllRows()
{
    if (fAllEventData.empty()) return;

    std::sort(fAllEventData.begin(), fAllEventData.end(),
              [](const auto& a, const auto& b) { return a.first < b.first; });

    // Collect union of all process names
    std::vector<std::string> columns;
    {
        std::map<std::string, bool> seen;
        for (const auto& [id, counts] : fAllEventData)
            for (const auto& [name, _] : counts)
                seen[name] = true;
        for (const auto& [name, _] : seen)
            columns.push_back(name);
    }

    bool fileExists = std::filesystem::exists(fFilename);

    if (!fileExists) {
        std::ofstream hdr(fFilename);
        hdr << "event,";
        for (const auto& name : columns) hdr << name << ",";
        hdr << "\n";
    }

    std::ofstream file(fFilename, std::ios::app);
    for (const auto& [eventID, counts] : fAllEventData) {
        file << (eventID + 1) << ",";
        for (const auto& name : columns) {
            auto it = counts.find(name);
            file << (it != counts.end() ? it->second : 0) << ",";
        }
        file << "\n";
    }
}
