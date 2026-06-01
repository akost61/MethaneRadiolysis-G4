#include "generator.hh"

MyPrimaryGenerator::MyPrimaryGenerator(G4double incomingEnergy)
{
    fParticleGun = new G4ParticleGun(1);

    G4ParticleDefinition* electron =
        G4ParticleTable::GetParticleTable()->FindParticle("e-");

    fParticleGun->SetParticleDefinition(electron);

    fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0., 0., 1.));

    fParticleGun->SetParticleEnergy(incomingEnergy);
}

void MyPrimaryGenerator::GeneratePrimaries(G4Event *anEvent){

    //Define the pencil beam
    G4double radius = 0.5*cm;
    G4double r = radius * std::sqrt(G4UniformRand());
    G4double phi = 2.0 * CLHEP::pi * G4UniformRand();

    // Beam cross-section in x-y plane
    G4double x = r * std::cos(phi);
    G4double y = r * std::sin(phi);

    G4double z = 0*cm;

    fParticleGun->SetParticlePosition(G4ThreeVector(x, y, z));
    fParticleGun->GeneratePrimaryVertex(anEvent);


};

MyPrimaryGenerator::~MyPrimaryGenerator(){
    delete fParticleGun;
}