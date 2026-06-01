#include "construction.hh"

MyDetectorConstruction::MyDetectorConstruction() {}
MyDetectorConstruction::~MyDetectorConstruction() {}

G4VPhysicalVolume* MyDetectorConstruction::Construct()
{
    G4NistManager* nist = G4NistManager::Instance();
    G4Material* vacuumMat = nist->FindOrBuildMaterial("G4_Galactic");

    G4double world_sizeXY = 100*cm;
    G4double world_sizeZ  = 120*cm;

    G4Box* solidWorld = new G4Box("World",
                                   world_sizeXY,
                                   world_sizeXY,
                                   world_sizeZ);

    G4LogicalVolume* logicWorld = new G4LogicalVolume(solidWorld, vacuumMat, "LogicWorld");

    G4VisAttributes* worldVis = new G4VisAttributes(G4Colour(0.5, 0.5, 1.0));
    worldVis->SetVisibility(true);
    worldVis->SetForceWireframe(true);
    logicWorld->SetVisAttributes(worldVis);

    G4VPhysicalVolume* physWorld = new G4PVPlacement(0,
                                                     G4ThreeVector(0, 0, 0),
                                                     logicWorld,
                                                     "PhysWorld",
                                                     0, false, 0, true);

    G4Material* nistMethane = nist->FindOrBuildMaterial("G4_METHANE");

    G4double temperature    = 298.15 * kelvin;   
    G4double pressure       = 1.0   * atmosphere;

    G4double densitySTP  = nistMethane->GetDensity();          
    G4double density25C  = densitySTP * (273.15*kelvin / temperature)
                                      * (pressure / atmosphere);

    G4Material* methane25C = new G4Material("CH4",
                                             density25C,
                                             nistMethane,
                                             kStateGas,
                                             temperature,
                                             pressure);

    G4cout << "CH4 density: " << density25C/(kg/m3) << " kg/m3" << G4endl;

    G4double cylinder_radius = 50  * cm;
    G4double cylinder_height = 60  * cm;
    G4double cylinder_face   = 10  * cm;
    G4double cylinder_center_z = cylinder_face + cylinder_height/2;

    G4Tubs* solidMethane = new G4Tubs("solidMethane",
                                       0.*cm, cylinder_radius,
                                       cylinder_height/2,
                                       0.*deg, 360.*deg);

    G4LogicalVolume* logicMethane = new G4LogicalVolume(solidMethane,
                                                         methane25C,
                                                         "logicMethane");

    G4VPhysicalVolume* physMethane = new G4PVPlacement(0,
                                                        G4ThreeVector(0, 0, cylinder_center_z),
                                                        logicMethane,
                                                        "physMethane",
                                                        logicWorld,
                                                        false, 0, true);

    G4VisAttributes* methaneVis = new G4VisAttributes(G4Colour(1.0, 0.0, 0.0, 0.5));
    methaneVis->SetVisibility(true);
    methaneVis->SetForceSolid(true);
    logicMethane->SetVisAttributes(methaneVis);

    return physWorld;
}