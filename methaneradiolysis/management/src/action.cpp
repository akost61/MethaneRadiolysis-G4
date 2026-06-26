#include "action.hh"
#include "generator.hh"
#include "run.hh"
#include "stepping.hh"

MyActionInitialization::MyActionInitialization(G4double initialEnergy, G4double cutOffEnergy) : fInitialEnergy(initialEnergy), fcutOffEnergy(cutOffEnergy) {}
MyActionInitialization::~MyActionInitialization() {}

void MyActionInitialization::BuildForMaster() const
{
    SetUserAction(new RunAction());  // no stepping action needed here
}

void MyActionInitialization::Build() const
{
    SteppingAction* steppingAction = new SteppingAction(fcutOffEnergy);
    RunAction* runAction = new RunAction(steppingAction);

    SetUserAction(runAction);
    SetUserAction(steppingAction);
    SetUserAction(new MyPrimaryGenerator(fInitialEnergy));
}