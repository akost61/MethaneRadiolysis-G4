#ifndef RUN_HH
#define RUN_HH

#include "G4UserRunAction.hh"
#include "VoxelEnergyMap.hh"
#include "stepping.hh"
#include <map>
#include <mutex>
#include <string>
#include <vector>

class RunAction : public G4UserRunAction {
public:
    RunAction(SteppingAction* sa);
    RunAction();
    ~RunAction();

    void BeginOfRunAction(const G4Run*) override;
    void EndOfRunAction(const G4Run*)   override;

    VoxelEnergyMap* GetMap() const { return fMap; }

    // Called from EventAction on worker thread (no lock — worker-local)
    void RecordEventCounts(int eventID, const std::map<std::string, int>& counts);

    // Called by worker EndOfRunAction to send its batch to master
    void MergeEventData(const std::vector<std::pair<int, std::map<std::string, int>>>& data) const;

private:
    VoxelEnergyMap* fMap = nullptr;
    SteppingAction* fSteppingAction;
    std::string     fFilename;

    // Worker-local: accumulated per-event snapshots for this thread
    std::vector<std::pair<int, std::map<std::string, int>>> fEventData;

    // Master-only: all event snapshots merged from workers
    mutable std::vector<std::pair<int, std::map<std::string, int>>> fAllEventData;
    mutable std::mutex fMutex;

    void WriteAllRows();
};

#endif
