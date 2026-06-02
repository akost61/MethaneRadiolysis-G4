#include <iostream>
#include <string>
#include <thread>
#include "G4MTRunManager.hh"
#include "G4VisManager.hh"
#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"
#include "G4UImanager.hh"

#include "construction.hh"
#include "physics.hh"
#include "action.hh"
#include "CH4IonizationProcess.hh"
#include "root_species.hh"
#include "root_process.hh"

int main(int argc, char** argv){

    G4double cutOffEnergy  = 7.0 * eV;
    G4double initialEnergy = 100.0 * keV;
    int      nRuns         = (argc > 1) ? std::stoi(argv[1]) : 10;

    unsigned int nThreads = std::thread::hardware_concurrency();
    if (nThreads == 0) nThreads = 4;

    CH4IonizationProcess::SetSecondaryMinEnergy(cutOffEnergy);

    G4MTRunManager *runManager = new G4MTRunManager();
    runManager->SetNumberOfThreads(nThreads);
    runManager->SetUserInitialization(new MyDetectorConstruction());
    runManager->SetUserInitialization(new MyPhysics());
    runManager->SetUserInitialization(new MyActionInitialization(initialEnergy, cutOffEnergy));
    runManager->Initialize();

    G4UImanager *UImanager = G4UImanager::GetUIpointer();
    UImanager->ApplyCommand("/tracking/verbose 0");
    UImanager->ApplyCommand("/run/beamOn " + std::to_string(nRuns));

    delete runManager;

    root_species(nRuns);
    root_process();


    return 0;
}