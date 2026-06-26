
#ifndef STEPPING_HH
#define STEPPING_HH

#include "G4UserSteppingAction.hh"
#include "G4ParticleChange.hh"
#include "globals.hh"
#include <map>
#include <string>


class SteppingAction : public G4UserSteppingAction
{
public:
    SteppingAction(G4double cutOffEnergy);
    virtual ~SteppingAction();
    virtual void UserSteppingAction(const G4Step* step);
    void ResetCounts() { fProcessCount.clear(); };
    const std::map<std::string, int>& GetProcessCounts() const {return fProcessCount; };

private:
    G4double fCutOffEnergy;
    G4ParticleChange aParticleChange;
    std::map<std::string, int> fProcessCount;

};

#endif