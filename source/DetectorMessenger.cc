#include "DetectorMessenger.hh"
#include "DetectorConstruction.hh"

#include <G4RunManager.hh>
#include <G4UIcmdWithABool.hh>
#include <G4UIcmdWithADouble.hh>
#include <G4UIcmdWithADoubleAndUnit.hh>

DetectorMessenger::DetectorMessenger(DetectorConstruction* Det)
    : G4UImessenger(), fDetectorConstruction(Det) {
  fDirectory = new G4UIdirectory("/myDet/");

  fAbsorberLenUnits = new G4UIcmdWithABool("/myDet/AbsorberLenUnits", this);
  fAbsorberLenUnits->SetGuidance("Use units or not for absorber length");
  fAbsorberLenUnits->SetParameterName("AbsorberLenUnits", false);
  fAbsorberLenUnits->AvailableForStates(G4State_PreInit, G4State_Idle);

  fAbsorberInRadiationLengths =
      new G4UIcmdWithADouble("/myDet/AbsorberInRadiationLengths", this);
  fAbsorberInRadiationLengths->SetGuidance(
      "Length of the first absorber in radiation lengths");
  fAbsorberInRadiationLengths->SetParameterName("AbsorberLen", false);
  fAbsorberInRadiationLengths->SetRange(
      "AbsorberLen > 0.0 && AbsorberLen < 15.0");
  fAbsorberInRadiationLengths->AvailableForStates(G4State_PreInit,
                                                  G4State_Idle);

  fAbsorberIncm = new G4UIcmdWithADoubleAndUnit("/myDet/AbsorberIncm", this);
  fAbsorberIncm->SetGuidance("Length of the absorber in c");
  fAbsorberIncm->SetParameterName("AbsorberLenCm", false);
  fAbsorberIncm->SetUnitCategory("Length");
  fAbsorberIncm->AvailableForStates(G4State_PreInit, G4State_Idle);
}

DetectorMessenger::~DetectorMessenger() {
  delete fAbsorberLenUnits;
  delete fAbsorberInRadiationLengths;
  delete fAbsorberIncm;
  delete fDirectory;
}

void DetectorMessenger::SetNewValue(G4UIcommand* command, G4String newValue) {
  if (command == fAbsorberLenUnits) {
    fDetectorConstruction->SetAbsUnits(
        fAbsorberLenUnits->GetNewBoolValue(newValue));
  } else if (command == fAbsorberInRadiationLengths) {
    fDetectorConstruction->SetAbsLenghtInRadLen(
        fAbsorberInRadiationLengths->GetNewDoubleValue(newValue));
  } else if (command == fAbsorberIncm) {
    fDetectorConstruction->SetAbsLenghtIncm(
        fAbsorberIncm->GetNewDoubleValue(newValue));
  }

  G4RunManager::GetRunManager()->PhysicsHasBeenModified();
  G4RunManager::GetRunManager()->GeometryHasBeenModified();
}
