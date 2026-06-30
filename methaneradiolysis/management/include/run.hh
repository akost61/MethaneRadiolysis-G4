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
    void MergeCounts(const std::map<std::string, int>& counts) const;

private:
    VoxelEnergyMap*                    fMap = nullptr;
    SteppingAction*                    fSteppingAction;
    mutable std::map<std::string, int> fBlockCounts;
    mutable std::mutex                 fMutex;
    mutable std::vector<std::string>   fColumnNames;
    int                                fBlockCounter = 0;
    std::string                        fFilename;

    void WriteRow();
};

#endif
