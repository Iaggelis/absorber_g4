#ifndef DETECTORCONSTRUCTION_H
#define DETECTORCONSTRUCTION_H 1

#include "GapSD.hh"

#include <G4Cache.hh>
#include <G4Material.hh>
#include <G4OpticalSurface.hh>
#include <G4Types.hh>
#include <G4VUserDetectorConstruction.hh>
#include <globals.hh>

class G4Box;
class G4LogicalVolume;
class G4PVPlacement;
class G4VPhysicalVolume;
class DetectorMessenger;
class G4MultiFunctionalDetector;

/// Detector construction class to define materials and geometry.
class DetectorConstruction : public G4VUserDetectorConstruction {
public:
  DetectorConstruction();
  virtual ~DetectorConstruction();

  virtual G4VPhysicalVolume* Construct();
  virtual void ConstructSDandField(); // construct Sensitive Detector

  void AddGasBox(G4bool flag);

  void SetAbsUnits(G4bool flag) { fAbsorberUseUnits = flag; }
  auto GetAbsUnits() const -> G4bool { return fAbsorberUseUnits; };

  void SetAbsLenghtInRadLen(G4double);
  auto GetAbsLenghtInRadLen() const -> G4double {
    return fAbsorberLenghtInRadLen;
  };

  void SetAbsLenghtIncm(G4double);
  auto GetAbsLenghtIncm() const -> G4double { return fAbsorberLenghtIncm; };

  // void AddOpticalPropertiesVacuum(G4Material*);

  void UpdateGeometry();

private:
  // methods
  void DefineMaterials();
  G4VPhysicalVolume* DefineVolumes();

  G4bool fCheckOverlaps;
  G4bool fAbsorberUseUnits;
  G4double fAbsorberLenghtInRadLen;
  G4double fAbsorberLenghtIncm;
  DetectorMessenger* fMessenger;

  G4int fnGapsAfterAbs;
  std::vector<G4LogicalVolume*> logicGapsAfterAbs;
  std::vector<G4PVPlacement*> physiGapsAfterAbs;

  // World
  G4Box* solidWorld;
  G4LogicalVolume* logicWorld;
  G4PVPlacement* physiWorld;

  G4Box* solidGap;
  // Gap before calorimeter
  G4LogicalVolume* logicLayerBefore;
  G4PVPlacement* physiLayerBefore;
  // Last Gap
  G4LogicalVolume* logicLastGap;
  G4PVPlacement* physiLastGap;

  // Calorimeter
  G4Box* solidCalo;
  G4LogicalVolume* logicCalo;
  G4PVPlacement* physiCalo;

  // Layers of the calorimeter
  std::vector<G4Box*> solidLayers;
  std::vector<G4LogicalVolume*> logicLayers;
  std::vector<G4PVPlacement*> physiLayers;

  // Absorbers
  std::vector<G4Box*> solidAbsorbers;
  std::vector<G4LogicalVolume*> logicAbsorbers;
  std::vector<G4PVPlacement*> physiAbsorbers;

  std::vector<G4LogicalVolume*> logicGaps;
  std::vector<G4PVPlacement*> physiGaps;

  // Sensitive Detectors
  G4Cache<GapSD*> fgapSD;
  G4Cache<G4MultiFunctionalDetector*> fabsorberMFDet;
  G4Cache<G4MultiFunctionalDetector*> fgapMFDet;
};

#endif
