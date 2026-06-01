
#ifndef PHYSICS_HH
#define PHYSICS_HH
#include "G4ProcessManager.hh"
#include "G4Electron.hh"
#include "G4Positron.hh"
#include "G4Gamma.hh"
#include "G4Proton.hh"
#include "ElasticProcess.hh"
#include "Ion1Process.hh"
#include "Ion2Process.hh"
#include "Ion3Process.hh"
#include "Ion4Process.hh"
#include "Ion5Process.hh"
#include "Ion6Process.hh"
#include "Ion7Process.hh"

#include "ElectronAttachmentProcess.hh"

#include "ND1Process.hh"
#include "ND2Process.hh"
#include "ND3Process.hh"

#include "Nu1Process.hh"
#include "Nu2Process.hh"
#include "Nu3Process.hh"
#include "Nu4Process.hh"
#include "J3Process.hh"
#include "J4Process.hh"


#include "LyAlphaProcess.hh"
#include "LyBetaProcess.hh"
#include "LyGammaProcess.hh"
#include "HAlphaProcess.hh"
#include "HBetaProcess.hh"
#include "HDeltaProcess.hh"
#include "HGammaProcess.hh"
#include "CHGBandProcess.hh"
#include "C1Process.hh"
#include "C3Process.hh"
#include "C4Process.hh"


#include "CH2Plus.hh"
#include "CH2Star.hh"
#include "CH3Plus.hh"
#include "CH3Star.hh"
#include "CH4Plus.hh"
#include "CHPlus.hh"
#include "CHStar.hh"
#include "CPlus.hh"
#include "H2Molecule.hh"
#include "H2Plus.hh"
#include "HPlus.hh"
#include "HStar.hh"
#include "HNeg.hh"

#include "G4VModularPhysicsList.hh"

class MyPhysics : public G4VUserPhysicsList
{
public:

    void SetCuts() override;
    void ConstructProcess() override;
    void ConstructParticle() override;
};

#endif