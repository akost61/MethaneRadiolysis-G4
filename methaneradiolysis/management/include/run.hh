#ifndef RUN_HH
#define RUN_HH

#include "G4UserRunAction.hh"
#include "VoxelEnergyMap.hh"
#include "stepping.hh"

class RunAction : public G4UserRunAction {
public:
    RunAction(SteppingAction* sa);
    RunAction();
    ~RunAction();

    void BeginOfRunAction(const G4Run*) override;
    void EndOfRunAction(const G4Run*)   override;

    VoxelEnergyMap* GetMap() const { return fMap; }

private:
    VoxelEnergyMap* fMap = nullptr;
    SteppingAction*            fSteppingAction;
    std::map<std::string, int> fBlockCounts;   // accumulated over 100 runs
    int                        fRunCounter;
    std::string                fFilename;

    void WriteRow();

};

#endif