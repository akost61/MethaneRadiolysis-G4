#ifndef EVENT_HH
#define EVENT_HH

#include "G4UserEventAction.hh"
#include "stepping.hh"

class RunAction;

class EventAction : public G4UserEventAction {
public:
    EventAction(SteppingAction* sa, RunAction* ra);
    void BeginOfEventAction(const G4Event*) override;
    void EndOfEventAction(const G4Event*) override;
private:
    SteppingAction* fSteppingAction;
    RunAction*      fRunAction;
};

#endif
