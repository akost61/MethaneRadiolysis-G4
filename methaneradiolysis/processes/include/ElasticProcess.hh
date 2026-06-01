#ifndef CH4ELASTICPROCESS_HH
#define CH4ELASTICPROCESS_HH

#include "CH4DiscreteProcess.hh"

class ElasticProcess : public CH4DiscreteProcess {

public:

    ElasticProcess():
    CH4DiscreteProcess("ElasticProcess"){}


    G4VParticleChange* PostStepDoIt(const G4Track& track,
                                                 const G4Step&  /*step*/) override
{

    aParticleChange.Initialize(track);
    ClearNumberOfInteractionLengthLeft();

    G4double energy    = track.GetKineticEnergy();

    G4ThreeVector oldDir = track.GetMomentumDirection();

    G4ThreeVector newDir = GetAngularDist().RotateDirection(oldDir, energy);

    aParticleChange.ProposeMomentumDirection(newDir);

    aParticleChange.ProposeEnergy(energy);         
    aParticleChange.ProposeTrackStatus(fAlive);


    return &aParticleChange;
}

protected:

    G4double GetCrossSection(G4double energy_eV) const override {
        return GetCrossSectionTable().GetElastic(energy_eV/eV);
    }
};

#endif // CH4ELASTICPROCESS_HH