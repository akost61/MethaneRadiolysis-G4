#include "event.hh"
#include "run.hh"
#include "G4Event.hh"

EventAction::EventAction(SteppingAction* sa, RunAction* ra)
    : fSteppingAction(sa), fRunAction(ra) {}

void EventAction::BeginOfEventAction(const G4Event*) {
    fSteppingAction->ResetCounts();
}

void EventAction::EndOfEventAction(const G4Event* event) {
    fRunAction->RecordEventCounts(event->GetEventID(),
                                  fSteppingAction->GetProcessCounts());
}
