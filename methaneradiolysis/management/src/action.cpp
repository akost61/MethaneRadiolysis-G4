#include "action.hh"
#include "generator.hh"
#include "run.hh"
#include "stepping.hh"
#include "event.hh"

MyActionInitialization::MyActionInitialization(G4double initialEnergy, G4double cutOffEnergy) : fInitialEnergy(initialEnergy), fcutOffEnergy(cutOffEnergy) {}
MyActionInitialization::~MyActionInitialization() {}

void MyActionInitialization::BuildForMaster() const
{
    SetUserAction(new RunAction());
}

void MyActionInitialization::Build() const
{
    SteppingAction* steppingAction = new SteppingAction(fcutOffEnergy);
    RunAction*      runAction      = new RunAction(steppingAction);

    SetUserAction(runAction);
    SetUserAction(steppingAction);
    SetUserAction(new EventAction(steppingAction, runAction));
    SetUserAction(new MyPrimaryGenerator(fInitialEnergy));
}
