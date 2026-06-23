#ifndef RUN_HH
#define RUN_HH

#include "G4UserRunAction.hh"
#include "VoxelEnergyMap.hh"

class RunAction : public G4UserRunAction {
public:
    RunAction();
    ~RunAction();

    void BeginOfRunAction(const G4Run*) override;
    void EndOfRunAction(const G4Run*)   override;

    VoxelEnergyMap* GetMap() const { return fMap; }

private:
    VoxelEnergyMap* fMap = nullptr;
};

#endif