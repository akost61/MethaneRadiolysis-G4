#ifndef CONSTRUCTION_HH
#define CONSTRUCTION_HH

#include "G4VUserDetectorConstruction.hh"
#include "G4VPhysicalVolume.hh"
#include "G4LogicalVolume.hh"
#include "G4Box.hh"
#include "G4PVPlacement.hh"
#include "G4NistManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4Tubs.hh"
#include "G4Colour.hh"
#include "G4VisAttributes.hh"
#include "G4SDManager.hh"

class MyDetectorConstruction: public G4VUserDetectorConstruction
{
    public:
    MyDetectorConstruction(G4double radius, G4double height, G4double temperature, G4double pressure);
    ~MyDetectorConstruction();

    virtual G4VPhysicalVolume *Construct();

    private:
    G4double fCylinderRadius;
    G4double fCylinderHeight;
    G4double fPressure;
    G4double fTemperature;
};

#endif