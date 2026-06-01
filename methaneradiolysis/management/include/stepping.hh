
#ifndef STEPPING_HH
#define STEPPING_HH

#include "G4UserSteppingAction.hh"
#include "globals.hh"


class SteppingAction : public G4UserSteppingAction
{
public:
    SteppingAction(G4double cutOffEnergy);
    virtual ~SteppingAction();
    
    virtual void UserSteppingAction(const G4Step* step);
private:
    G4double fCutOffEnergy;

};

#endif