#include "CH4IonizationProcess.hh"
#include "G4Electron.hh"
#include "G4DynamicParticle.hh"
#include "G4ParticleTable.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"
#include "VoxelEnergyMap.hh"
// #include "G4AnalysisManager.hh"
#include "G4RunManager.hh"
#include "G4Run.hh"
#include "RecoilLookUp.hh"
#include <cmath>
#include "VoxelEnergyMap.hh"


G4double CH4IonizationProcess::fgSecondaryMinEnergy = 0.0;

CH4IonizationProcess::CH4IonizationProcess(const G4String&                 name,
                                             G4double                        ionizationPotential,
                                             const std::vector<std::string>& products)
    : CH4DiscreteProcess(name),
      fIonizationPotential(ionizationPotential),
      fProducts(products)
{}


G4VParticleChange* CH4IonizationProcess::PostStepDoIt(const G4Track& track,
                                                       const G4Step&  step)
{
    aParticleChange.Initialize(track);
    // auto* am = G4AnalysisManager::Instance();

    G4double      energy   = track.GetKineticEnergy();
    G4ThreeVector oldDir   = track.GetMomentumDirection();
    G4ThreeVector position = step.GetPostStepPoint()->GetPosition();

    G4int ix = (G4int)((position.x() + 100*cm) / (2*mm));
    G4int iy = (G4int)((position.y() + 100*cm) / (2*mm));
    G4int iz = (G4int)((position.z() + 120*cm) / (2*mm));
    auto* voxelMap = VoxelEnergyMap::GetInstance();
    
    G4double betaAngle = GetAngularDist().SampleAngle(energy);

    G4double recoilEnergy = GetRecoil().SelectRecoil(energy, betaAngle);

    G4ThreeVector newDir = GetAngularDist().RotateDirection(oldDir, energy, betaAngle);

    G4double energyLoss = fIonizationPotential + recoilEnergy;

    if (energy <= fIonizationPotential) {
        aParticleChange.ProposeEnergy(0.0);
        aParticleChange.ProposeLocalEnergyDeposit(energy);
        aParticleChange.ProposeTrackStatus(fStopAndKill);
        ClearNumberOfInteractionLengthLeft();
        voxelMap->Deposit(ix, iy, iz, 0.0, energy);
        return &aParticleChange;
    }

    G4double secondaryEnergy = GetSecondaryEnergy(energy);
    energyLoss += secondaryEnergy;

    G4double newEnergy = energy - energyLoss;

    G4ThreeVector secondaryDirection = RotateSecondaryDirection(oldDir);


    G4DynamicParticle* electron =
        new G4DynamicParticle(G4Electron::Definition(),
                                secondaryDirection,
                                secondaryEnergy);
    G4Track* secondaryTrack =
        new G4Track(electron, track.GetGlobalTime(), position);
    secondaryTrack->SetTouchableHandle(track.GetTouchableHandle());
    aParticleChange.AddSecondary(secondaryTrack);

    

    G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
    for (const std::string& productName : fProducts) {
        G4ParticleDefinition* productDef = particleTable->FindParticle(productName);
        if (!productDef) {
            G4cerr << GetProcessName() << ": particle '" << productName
                   << "' not found in particle table." << G4endl;
            continue;
        }
        G4DynamicParticle* product =
            new G4DynamicParticle(productDef, G4ThreeVector(0, 0, 1), 0.0);
        G4Track* productTrack =
            new G4Track(product, track.GetGlobalTime(), position);
        productTrack->SetTouchableHandle(track.GetTouchableHandle());
        aParticleChange.AddSecondary(productTrack);
        // am->FillNtupleIColumn(0, 0, G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID() + 1);
        // am->FillNtupleDColumn(0, 1, track.GetTrackID());
        // am->FillNtupleDColumn(0, 2, position.x() / CLHEP::mm);
        // am->FillNtupleDColumn(0, 3, position.y() / CLHEP::mm);
        // am->FillNtupleDColumn(0, 4, position.z() / CLHEP::mm);
        // am->FillNtupleSColumn(0, 5, productName);
        // am->FillNtupleDColumn(0, 6, track.GetGlobalTime() / CLHEP::ns);
        // am->AddNtupleRow(0);
    }

    if (newEnergy < GetCutOffEnergy()) {
        aParticleChange.ProposeLocalEnergyDeposit(fIonizationPotential + recoilEnergy + newEnergy);
        aParticleChange.ProposeEnergy(0.0);
        aParticleChange.ProposeTrackStatus(fStopAndKill);
        voxelMap->Deposit(ix, iy, iz, fIonizationPotential, recoilEnergy + newEnergy);

    } else {
        aParticleChange.ProposeMomentumDirection(newDir);
        aParticleChange.ProposeLocalEnergyDeposit(fIonizationPotential + recoilEnergy);
        aParticleChange.ProposeEnergy(newEnergy);
        aParticleChange.ProposeTrackStatus(fAlive);
        voxelMap->Deposit(ix, iy, iz, fIonizationPotential, recoilEnergy);

    }

    ClearNumberOfInteractionLengthLeft();
    return &aParticleChange;
}






G4double CH4IonizationProcess::SecondaryAverageEnergy(G4double kineticEnergy) const
{
    const CrossSectionTable& fTable = GetCrossSectionTable();

    double MSP;
    double methaneMass = 2.6637e-23;
    double E_eV = kineticEnergy / eV;

    if (kineticEnergy < 1000 * eV) {
        return ((28.3829/ 985.4) * E_eV -0.42054) * eV; 
    } else {
        MSP = (0.8213 * std::pow(kineticEnergy, -0.769)) * 1e6;
    }

    double elastic = fTable.GetElastic(E_eV) / 100;
    double ion1    = fTable.GetIon1(E_eV)    / 100;
    double ion2    = fTable.GetIon2(E_eV)    / 100;
    double ion3    = fTable.GetIon3(E_eV)    / 100;
    double ion4    = fTable.GetIon4(E_eV)    / 100;
    double ion5    = fTable.GetIon5(E_eV)    / 100;
    double ion6    = fTable.GetIon6(E_eV)    / 100;
    double ion7    = fTable.GetIon7(E_eV)    / 100;
    double nd1     = fTable.GetND1(E_eV)     / 100;
    double nd2     = fTable.GetND2(E_eV)     / 100;
    double nd3     = fTable.GetND3(E_eV)     / 100;
    double ea      = fTable.GetEA(E_eV)      / 100;
    double nu1     = fTable.GetNu1(E_eV)     / 100;
    double nu2     = fTable.GetNu2(E_eV)     / 100;
    double nu3     = fTable.GetNu3(E_eV)     / 100;
    double nu4     = fTable.GetNu4(E_eV)     / 100;
    double j3      = fTable.GetJ3(E_eV)      / 100;
    double j4      = fTable.GetJ4(E_eV)      / 100;
    double lya     = fTable.GetLya(E_eV)     / 100;
    double lyb     = fTable.GetLyb(E_eV)     / 100;
    double lyg     = fTable.GetLyg(E_eV)     / 100;
    double ha      = fTable.GetHa(E_eV)      / 100;
    double hb      = fTable.GetHb(E_eV)      / 100;
    double hg      = fTable.GetHg(E_eV)      / 100;
    double hd      = fTable.GetHd(E_eV)      / 100;
    double chg     = fTable.GetCHG(E_eV)     / 100;
    double c3      = fTable.GetC3(E_eV)      / 100;
    double c4      = fTable.GetC4(E_eV)      / 100;
    double c1      = fTable.GetC1(E_eV)      / 100;

    struct Channel {
        double crossSection;
        double energyLoss;
    };

    std::vector<Channel> channels = {
        {ion1,  12.60}, {ion2,  14.52}, {ion3,  15.30}, {ion4,  18.28},
        {ion5,  20.10}, {ion6,  20.42}, {ion7,  19.67},
        {nd1,    4.68}, {nd2,    4.95}, {nd3,    9.46},
        {ea,     0.00},
        {nu1,  3.62e-1}, {nu2, 1.90e-1}, {nu3, 3.74e-1}, {nu4, 1.62e-1},
        {j3,   7.8e-3},  {j4,  1.3e-2},
        {lya,  10.20}, {lyb,  12.08}, {lyg,  12.75},
        {ha,    1.89}, {hb,    2.55}, {hg,    2.86}, {hd,    3.03},
        {chg,   2.88}, {c3,    6.48}, {c4,    7.49}, {c1,    8.00},
        {elastic, 0.00}
    };

    double weightedEnergyLoss = 0.0;
    for (const auto& ch : channels) {
        weightedEnergyLoss += ch.crossSection * ch.energyLoss;
    }

    double ionWeightedSum = 0.0;
    for (int i = 0; i < 7; i++) {
        ionWeightedSum += channels[i].crossSection;
    }

    return (((MSP * methaneMass) - weightedEnergyLoss) / ionWeightedSum) * eV;
}


G4double CH4IonizationProcess::GetSecondaryEnergy(G4double kineticEnergy) const
{
    G4double rnd = G4UniformRand();
    double Ymax = (kineticEnergy - fIonizationPotential) / (2.0 * eV); 
    double Ymin = 2.0;                                  
    G4double secondaryAverage = SecondaryAverageEnergy(kineticEnergy) / eV; 
    if (Ymin >= Ymax)
    {
        return 0;
    }

    secondaryAverage = std::max(Ymin + 1e-6, std::min(secondaryAverage, Ymax - 1e-6));

    double exponent = ((Ymin - secondaryAverage) / (secondaryAverage - Ymax));

    if ((((Ymin - Ymax) * std::pow((1 - rnd), exponent)) + Ymax) < 0.0 )
    {
        return 0;
        G4cout<< theProcessName << G4endl;
        G4cout << "Incoming Energy: " << kineticEnergy / eV<< G4endl;
        G4cout << "Secondary Energy Average: " << secondaryAverage << G4endl;
        G4cout << "Ymax: " << Ymax <<G4endl;
        G4cout << "Ymin: " << Ymin <<G4endl;
        G4cout << "random: "<< rnd << G4endl;
        G4cout << "exponent: " << exponent <<G4endl;
        G4cout << "Secondary Energy: " << (((Ymin - Ymax) * std::pow((1 - rnd), exponent)) + Ymax) <<G4endl;
        G4cout <<G4endl;
    }


    return (((Ymin - Ymax) * std::pow((1 - rnd), exponent)) + Ymax) * eV;

}



G4ThreeVector CH4IonizationProcess::RotateSecondaryDirection(const G4ThreeVector& oldDirection)
{
    G4double cosTheta = 2.0 * G4UniformRand() - 1.0;
    G4double sinTheta = std::sqrt(1.0 - cosTheta * cosTheta);
    G4double phi      = 2.0 * CLHEP::pi * G4UniformRand(); 

    G4ThreeVector newDirection(
        sinTheta * std::cos(phi),
        sinTheta * std::sin(phi),
        cosTheta
    );

    return newDirection.unit();
}
