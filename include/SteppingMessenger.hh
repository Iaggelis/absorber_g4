#ifndef STEPPINGMESSENGER_H
#define STEPPINGMESSENGER_H 1

#include <G4UImessenger.hh>
#include <globals.hh>

class SteppingAction;
class G4UIcmdWithABool;

class SteppingMessenger : public G4UImessenger {
public:
  SteppingMessenger(SteppingAction*);
  virtual ~SteppingMessenger();

  virtual void SetNewValue(G4UIcommand*, G4String);

private:
  SteppingAction* fStepping;
  G4UIcmdWithABool* fOneStepPrimariesCmd;
};

#endif
