#include "ActionInitialization.hh"
#include "EventAction.hh"
#include "PrimaryGeneratorAction.hh"
#include "RunAction.hh"
#include "SteppingAction.hh"

ActionInitialization::ActionInitialization(
    DetectorConstruction* detectorConstruction)
    : G4VUserActionInitialization(),
      fDetectorConstruction(detectorConstruction) {}

ActionInitialization::~ActionInitialization() {}

void ActionInitialization::BuildForMaster() const {
  auto PrimaryGenAction = new PrimaryGeneratorAction();
  auto event_action     = new EventAction();
  SetUserAction(
      new RunAction(event_action, fDetectorConstruction, PrimaryGenAction));
  // SetUserAction(new SteppingAction(event_action));
}

void ActionInitialization::Build() const {
  auto PrimaryGenAction = new PrimaryGeneratorAction();
  SetUserAction(PrimaryGenAction);
  auto event_action = new EventAction();
  SetUserAction(event_action);
  auto run_action =
      new RunAction(event_action, fDetectorConstruction, PrimaryGenAction);
  SetUserAction(run_action);
  SetUserAction(new SteppingAction(event_action));
}
