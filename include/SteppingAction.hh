#ifndef STEPPINGACTION_H
#define STEPPINGACTION_H 1

#include <G4OpBoundaryProcess.hh>
#include <G4UserSteppingAction.hh>
#include <globals.hh>

class EventAction;
class SteppingMessenger;

class SteppingAction : public G4UserSteppingAction {
public:
  SteppingAction(EventAction*);
  virtual ~SteppingAction();
  virtual void UserSteppingAction(const G4Step*);

  void SetOneStepPrimaries(G4bool b) { fOneStepPrimaries = b; }
  auto GetOneStepPrimaries() -> G4bool { return fOneStepPrimaries; }

private:
  G4bool fOneStepPrimaries;
  SteppingMessenger* fSteppingMessenger;
  EventAction* fEventAction;

  G4OpBoundaryProcessStatus fExpectedNextStatus;
};

#endif
