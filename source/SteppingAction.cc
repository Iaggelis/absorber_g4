#include "SteppingAction.hh"
#include "EventAction.hh"
#include "SteppingMessenger.hh"

#include <G4Event.hh>
#include <G4EventManager.hh>
#include <G4ParticleDefinition.hh>
#include <G4ParticleTypes.hh>
#include <G4ProcessManager.hh>
#include <G4SDManager.hh>
#include <G4Step.hh>
#include <G4StepPoint.hh>
#include <G4SteppingManager.hh>
#include <G4Track.hh>
#include <G4TrackStatus.hh>
#include <G4VPhysicalVolume.hh>

SteppingAction::SteppingAction(EventAction* ea)
    : fOneStepPrimaries(false), fEventAction(ea) {
  fSteppingMessenger = new SteppingMessenger(this);

  // fExpectedNextStatus = Undefined;
}

SteppingAction::~SteppingAction() {}

void SteppingAction::UserSteppingAction(const G4Step* theStep) {
  // using LXe code from examples/extended/optical
  // G4Track* theTrack = theStep->GetTrack();

  // if (theTrack->GetCurrentStepNumber() == 1)
  //   fExpectedNextStatus = Undefined;

  // G4StepPoint* thePrePoint    = theStep->GetPreStepPoint();
  // G4VPhysicalVolume* thePrePV = thePrePoint->GetPhysicalVolume();

  G4StepPoint* thePostPoint    = theStep->GetPostStepPoint();
  G4VPhysicalVolume* thePostPV = thePostPoint->GetPhysicalVolume();

  // G4OpBoundaryProcessStatus boundaryStatus           = Undefined;
  // static G4ThreadLocal G4OpBoundaryProcess* boundary = nullptr;

  // find the boundary process only once
  // if (!boundary) {
  //   G4ProcessManager *pm =
  //       theStep->GetTrack()->GetDefinition()->GetProcessManager();
  //   G4int nprocesses = pm->GetProcessListLength();
  //   G4ProcessVector *pv = pm->GetProcessList();
  //   G4int i;
  //   for (i = 0; i < nprocesses; ++i) {
  //     if ((*pv)[i]->GetProcessName() == "OpBoundary") {
  //       boundary = (G4OpBoundaryProcess *)(*pv)[i];
  //       break;
  //     }
  //   }
  // }

  // NOTE: HACK because of weird macros
  // fOneStepPrimaries = true;
  // if (theTrack->GetParentID() == 0) {

  //   if (fOneStepPrimaries && thePrePV->GetName() == "gap0") {
  //     theTrack->SetTrackStatus(fStopAndKill);
  //   }
  // }

  if (!thePostPV) { // out of world
    fExpectedNextStatus = Undefined;
    return;
  }
}
