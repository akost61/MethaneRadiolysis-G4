#include "physics.hh"
#include "G4DecayPhysics.hh"


void MyPhysics::ConstructParticle(){

    G4Electron::ElectronDefinition();
    G4Positron::PositronDefinition();
    G4Gamma::GammaDefinition();
    G4Proton::ProtonDefinition();

    CH2Plus::Definition();
    CH2Star::Definition();
    CH3Plus::Definition();
    CH3Star::Definition();
    CH4Plus::Definition();
    CHPlus::Definition();
    CHStar::Definition();
    CPlus::Definition();
    H2Molecule::Definition();
    H2Plus::Definition();
    HPlus::Definition();
    HStar::Definition();
    HNeg::Definition();


    G4DecayPhysics* decayPhysics = new G4DecayPhysics();
    decayPhysics->ConstructParticle();

}

void MyPhysics::ConstructProcess()
{

    AddTransportation();
    G4ParticleDefinition* electron = G4Electron::Electron();
    G4ProcessManager* pManager = electron->GetProcessManager();   
    
    ElasticProcess* elasticProcess = new ElasticProcess();
    pManager->AddDiscreteProcess(elasticProcess);

    Ion1Process* ion1process = new Ion1Process();
    pManager->AddDiscreteProcess(ion1process);

    Ion2Process* ion2process = new Ion2Process();
    pManager->AddDiscreteProcess(ion2process);

    Ion3Process* ion3process = new Ion3Process();
    pManager->AddDiscreteProcess(ion3process);

    Ion4Process* ion4process = new Ion4Process();
    pManager->AddDiscreteProcess(ion4process);

    Ion5Process* ion5process = new Ion5Process();
    pManager->AddDiscreteProcess(ion5process);

    Ion6Process* ion6process = new Ion6Process();
    pManager->AddDiscreteProcess(ion6process);

    Ion7Process* ion7process = new Ion7Process();
    pManager->AddDiscreteProcess(ion7process);

    ND1Process* ND1process = new ND1Process();
    pManager->AddDiscreteProcess(ND1process);

    ND2Process* ND2process = new ND2Process();
    pManager->AddDiscreteProcess(ND2process);

    ND3Process* ND3process = new ND3Process();
    pManager->AddDiscreteProcess(ND3process);

    Nu1Process* Nu1process = new Nu1Process();
    pManager->AddDiscreteProcess(Nu1process);

    Nu2Process* Nu2process = new Nu2Process();
    pManager->AddDiscreteProcess(Nu2process);

    Nu3Process* Nu3process = new Nu3Process();
    pManager->AddDiscreteProcess(Nu3process);

    Nu4Process* Nu4process = new Nu4Process();
    pManager->AddDiscreteProcess(Nu4process);

    J3Process* J3process = new J3Process();
    pManager->AddDiscreteProcess(J3process);

    J4Process* J4process = new J4Process();
    pManager->AddDiscreteProcess(J4process);


    LyAlphaProcess* LyAlphaprocess = new LyAlphaProcess();
    pManager->AddDiscreteProcess(LyAlphaprocess);

    LyBetaProcess* LyBetaprocess = new LyBetaProcess();
    pManager->AddDiscreteProcess(LyBetaprocess);

    LyGammaProcess* LyGammaprocess = new LyGammaProcess();
    pManager->AddDiscreteProcess(LyGammaprocess);

    HAlphaProcess* HAlphaprocess = new HAlphaProcess();
    pManager->AddDiscreteProcess(HAlphaprocess);

    HBetaProcess* HBetaprocess = new HBetaProcess();
    pManager->AddDiscreteProcess(HBetaprocess);

    HGammaProcess* HGammaprocess = new HGammaProcess();
    pManager->AddDiscreteProcess(HGammaprocess);

    HDeltaProcess* HDeltaprocess = new HDeltaProcess();
    pManager->AddDiscreteProcess(HDeltaprocess);

    CHGBandProcess* CHGBandprocess = new CHGBandProcess();
    pManager->AddDiscreteProcess(CHGBandprocess);

    C1Process* C1process = new C1Process();
    pManager->AddDiscreteProcess(C1process);

    C3Process* C3process = new C3Process();
    pManager->AddDiscreteProcess(C3process);

    C4Process* C4process = new C4Process();
    pManager->AddDiscreteProcess(C4process);

    ElectronAttachmentProcess* electronattachmentProcess = new ElectronAttachmentProcess();
    pManager->AddDiscreteProcess(electronattachmentProcess);


    G4cout << "Custom elastic and inelastic processes registered for electrons" << G4endl;
}

void MyPhysics::SetCuts(){
    defaultCutValue = 0.00001*mm;
    SetCutsWithDefault();
}