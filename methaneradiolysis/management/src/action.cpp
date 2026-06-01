#include "action.hh"
#include "generator.hh"
#include "run.hh"
#include "stepping.hh"

MyActionInitialization::MyActionInitialization(G4double initialEnergy, G4double cutOffEnergy) : fInitialEnergy(initialEnergy), fcutOffEnergy(cutOffEnergy) {}
MyActionInitialization::~MyActionInitialization() {}

void MyActionInitialization::BuildForMaster() const
{
    SetUserAction(new RunAction());
}

void MyActionInitialization::Build() const
{
    SetUserAction(new RunAction());
    SetUserAction(new SteppingAction(fcutOffEnergy));
    SetUserAction(new MyPrimaryGenerator(fInitialEnergy));
}