#ifndef ACTIONINITIALIZATION_H
#define ACTIONINITIALIZATION_H 1

#include "DetectorConstruction.hh"

#include <G4VUserActionInitialization.hh>
#include <globals.hh>

// Action initialization class.
class ActionInitialization : public G4VUserActionInitialization {
public:
  ActionInitialization(DetectorConstruction* detConstr);
  virtual ~ActionInitialization();

  virtual void Build() const;
  virtual void BuildForMaster() const;

private:
  DetectorConstruction* fDetectorConstruction;
};

#endif
