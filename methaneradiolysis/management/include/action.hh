#ifndef ACTION_HH
#define ACTION_HH

#include "G4VUserActionInitialization.hh"
#include "generator.hh"


class MyActionInitialization: public G4VUserActionInitialization{
    public:
        MyActionInitialization(G4double initialEnergy, G4double cutOffEnergy);
        ~MyActionInitialization();
        void BuildForMaster() const override;
        void Build() const override;
    private:
        G4double fInitialEnergy;
        G4double fcutOffEnergy;

};



#endif