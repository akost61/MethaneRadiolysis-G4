#include <iostream>
#include <string>
#include "G4MTRunManager.hh"
#include "G4UImanager.hh"
#include "construction.hh"
#include "physics.hh"
#include "action.hh"
#include "CH4IonizationProcess.hh"
#include "CH4DiscreteProcess.hh"
#include "root_species.hh"

int main(int argc, char** argv) {

    G4double cutOffEnergy   = 7.0   * eV;
    G4double initialEnergy  = 100.0 * keV;
    int      nRuns          = (argc > 1) ? std::stoi(argv[1]) : 300000;
    G4double cylinderRadius = 50.0  * cm;
    G4double cylinderHeight = 60.0  * cm;
    G4double temperature    = 293.15 * kelvin;
    G4double pressure       = 1.0   * atmosphere;

    G4int nThreads = G4Threading::G4GetNumberOfCores();
    if (nThreads <= 0) nThreads = 4;

    G4cout << "NUMBER OF THREADS: " << nThreads << G4endl;

    G4cout << "-----------------------------ENERGY---------------------------" << G4endl;
    G4cout << "Cut Off Energy: " << cutOffEnergy / eV   << " eV"  << G4endl;
    G4cout << "Initial Energy: " << initialEnergy / keV << " keV" << G4endl;
    G4cout << "Number of Simulations: " << nRuns        << G4endl;
    G4cout << "--------------------------------------------------------------" << G4endl;

    G4cout << "------------------------CH4 DIMENSIONS------------------------" << G4endl;
    G4cout << "CH4 Cylinder Height: " << cylinderHeight / cm  << " cm"  << G4endl;
    G4cout << "CH4 Cylinder Radius: " << cylinderRadius / cm  << " cm"  << G4endl;
    G4cout << "Temperature: "         << temperature / kelvin << " K"   << G4endl;
    G4cout << "Pressure: "            << pressure / atmosphere << " atm" << G4endl;
    G4cout << "--------------------------------------------------------------" << G4endl;

    CH4IonizationProcess::SetSecondaryMinEnergy(cutOffEnergy);
    CH4DiscreteProcess::SetCutOffEnergy(cutOffEnergy);

    G4MTRunManager* runManager = new G4MTRunManager();
    runManager->SetNumberOfThreads(nThreads);


    runManager->SetUserInitialization(new MyDetectorConstruction(
        cylinderRadius, cylinderHeight, temperature, pressure));
    runManager->SetUserInitialization(new MyPhysics());
    runManager->SetUserInitialization(new MyActionInitialization(initialEnergy, cutOffEnergy));
    runManager->Initialize();

    G4UImanager* UImanager = G4UImanager::GetUIpointer();

    auto cmd = [&](const std::string& c) { UImanager->ApplyCommand(c); };

    cmd("/tracking/verbose 0");

    for (int i = 1; (i * 10000) <= nRuns; i++){
        G4cout<< "hi: "<< (i*10000)<<G4endl;
        cmd("/run/beamOn 10000");
    }

    // cmd("/run/beamOn " + std::to_string(nRuns));
    G4cout << "---beamOn complete---" << G4endl;

    delete runManager;
    G4cout << "---runManager deleted---" << G4endl;

    G4cout << "-----------------------------REPORT-----------------------------" << G4endl;
    // root_species(nRuns, cylinderHeight / cm, cylinderRadius / cm);
    G4cout << "----------------------------------------------------------------" << G4endl;

    return 0;
}