#include "SteppingMessenger.hh"
#include "SteppingAction.hh"

#include <G4UIcmdWithABool.hh>
#include <G4UIdirectory.hh>

SteppingMessenger::SteppingMessenger(SteppingAction* step) : fStepping(step) {
  fOneStepPrimariesCmd = new G4UIcmdWithABool("/myDet/oneStepPrimaries", this);
  fOneStepPrimariesCmd->SetGuidance(
      "Only allows primaries to go one step before being killed.");
}

SteppingMessenger::~SteppingMessenger() { delete fOneStepPrimariesCmd; }

void SteppingMessenger::SetNewValue(G4UIcommand* command, G4String newValue) {
  if (command == fOneStepPrimariesCmd) {
    fStepping->SetOneStepPrimaries(
        fOneStepPrimariesCmd->GetNewBoolValue(newValue));
  }
}
