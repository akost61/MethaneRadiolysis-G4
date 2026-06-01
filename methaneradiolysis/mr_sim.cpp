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

int main(int argc, char** argv){

    G4double cutOffEnergy  = (argc > 1) ? std::stod(argv[1]) * eV  :  7.0 *eV;
    G4double initialEnergy = (argc > 2) ? std::stod(argv[2]) * keV : 100.0 * keV;
    int      nRuns         = (argc > 3) ? std::stoi(argv[3])       : 10;

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


    return 0;
}