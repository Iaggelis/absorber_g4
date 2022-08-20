#ifndef DETECTORMESSENGER_H_
#define DETECTORMESSENGER_H_

#include <G4UImessenger.hh>
#include <globals.hh>

class DetectorConstruction;
class G4UIcmdWithABool;
class G4UIcmdWithADouble;
class G4UIcmdWithADoubleAndUnit;

class DetectorMessenger : public G4UImessenger {
public:
  DetectorMessenger(DetectorConstruction*);
  virtual ~DetectorMessenger();

  virtual void SetNewValue(G4UIcommand*, G4String);

private:
  DetectorConstruction* fDetectorConstruction;

  G4UIdirectory* fDirectory;
  G4UIcmdWithABool* fAbsorberLenUnits;
  G4UIcmdWithADouble* fAbsorberInRadiationLengths;
  G4UIcmdWithADoubleAndUnit* fAbsorberIncm;
};

#endif // MYDETECTORMESSENGER_H_
