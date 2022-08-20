//  header definition
#include "DetectorConstruction.hh"
#include "DetectorMessenger.hh"
#include "G4Types.hh"

// G4 includes
#include <G4Box.hh>
#include <G4LogicalBorderSurface.hh>
#include <G4LogicalSkinSurface.hh>
#include <G4LogicalVolume.hh>
#include <G4MultiFunctionalDetector.hh>
#include <G4NistManager.hh>
#include <G4OpticalPhoton.hh>
#include <G4OpticalSurface.hh>
#include <G4PSEnergyDeposit.hh>
#include <G4PSNofSecondary.hh>
#include <G4PVPlacement.hh>
#include <G4PVReplica.hh>
#include <G4PhysicalConstants.hh>
#include <G4RunManager.hh>
#include <G4SDManager.hh>
#include <G4SDParticleFilter.hh>
#include <G4StateManager.hh>
#include <G4SystemOfUnits.hh>
#include <G4UnitsTable.hh>
#include <G4VPrimitiveScorer.hh>
#include <G4VisAttributes.hh>

DetectorConstruction::DetectorConstruction()
    : G4VUserDetectorConstruction(), fCheckOverlaps(true) {
  solidWorld       = nullptr;
  logicWorld       = nullptr;
  physiWorld       = nullptr;
  solidGap         = nullptr;
  logicLayerBefore = nullptr;
  physiLayerBefore = nullptr;
  solidCalo        = nullptr;
  logicCalo        = nullptr;
  physiCalo        = nullptr;
  logicLastGap     = nullptr;
  physiLastGap     = nullptr;
  solidLayers      = std::vector<G4Box*>(1, nullptr);
  logicLayers      = std::vector<G4LogicalVolume*>(1, nullptr);
  physiLayers      = std::vector<G4PVPlacement*>(1, nullptr);
  solidAbsorbers   = std::vector<G4Box*>(1, nullptr);
  logicAbsorbers   = std::vector<G4LogicalVolume*>(1, nullptr);
  physiAbsorbers   = std::vector<G4PVPlacement*>(1, nullptr);
  logicGaps        = std::vector<G4LogicalVolume*>(1, nullptr);
  physiGaps        = std::vector<G4PVPlacement*>(1, nullptr);

  fAbsorberUseUnits       = false;
  fAbsorberLenghtInRadLen = 1.0;
  fAbsorberLenghtIncm     = 2.0 * CLHEP::cm;

  fnGapsAfterAbs    = 5;
  logicGapsAfterAbs = std::vector<G4LogicalVolume*>(fnGapsAfterAbs, nullptr);
  physiGapsAfterAbs = std::vector<G4PVPlacement*>(fnGapsAfterAbs, nullptr);

  fMessenger = new DetectorMessenger(this);
}

DetectorConstruction::~DetectorConstruction() { delete fMessenger; }

G4VPhysicalVolume* DetectorConstruction::Construct() {
  DefineMaterials();
  // Define volumes and return the physical volume
  return DefineVolumes();
}

void DetectorConstruction::DefineMaterials() {
  auto nistManager = G4NistManager::Instance();
  nistManager->FindOrBuildMaterial("G4_Pb");
  nistManager->FindOrBuildMaterial("G4_Fe");

  G4double a; // mass of a mole;
  G4double z; // z=mean number of protons;
  G4double density;
  // Vacuum
  auto world_mat =
      new G4Material("Galactic", z = 1., a = 1.01 * CLHEP::g / CLHEP::mole,
                     density = CLHEP::universe_mean_density, kStateGas,
                     2.73 * CLHEP::kelvin, 3.e-18 * CLHEP::pascal);

  auto* N = new G4Element("Nitrogen", "N", z = 7,
                          a = 14.00067 * CLHEP::g / CLHEP::mole);
  auto* O =
      new G4Element("Oxygen", "O", z = 8, a = 15.9994 * CLHEP::g / CLHEP::mole);
  auto vac_mat =
      new G4Material("Vac1", density = 1.29e-20 * CLHEP::mg / CLHEP::cm3, 2);
  vac_mat->AddElement(N, 0.7);
  vac_mat->AddElement(O, 0.3);

  (void)world_mat;
  // Print materials
  // G4cout << *(G4Material::GetMaterialTable()) << G4endl;
}

void DetectorConstruction::SetAbsLenghtInRadLen(G4double l) {
  fAbsorberLenghtInRadLen = l;
  if (G4StateManager::GetStateManager()->GetCurrentState() != G4State_PreInit) {
    G4RunManager::GetRunManager()->ReinitializeGeometry();
  }
  G4RunManager::GetRunManager()->GeometryHasBeenModified();
}

void DetectorConstruction::SetAbsLenghtIncm(G4double l) {
  fAbsorberLenghtIncm = l;
  if (G4StateManager::GetStateManager()->GetCurrentState() != G4State_PreInit) {
    G4RunManager::GetRunManager()->ReinitializeGeometry();
  }
  G4RunManager::GetRunManager()->GeometryHasBeenModified();
}

G4VPhysicalVolume* DetectorConstruction::DefineVolumes() {

  // Get materials
  // auto defaultMaterial = G4Material::GetMaterial("Galactic");
  auto defaultMaterial = G4Material::GetMaterial("Vac1");
  // AddOpticalPropertiesVacuum(defaultMaterial);
  auto absorberMaterial = G4Material::GetMaterial("G4_Pb");
  // auto absorberMaterial = G4Material::GetMaterial("G4_Fe");
  // auto gapMaterial      = G4Material::GetMaterial("Galactic");
  auto gapMaterial      = G4Material::GetMaterial("Vac1");
  // AddOpticalPropertiesVacuum(gapMaterial);

  // Geometry parameters
  constexpr G4double gapThickness = 0.5 * CLHEP::mm;
  constexpr G4double gapSizeXY    = 30.0 * CLHEP::cm;
  constexpr G4double calorSizeXY  = 30.0 * CLHEP::cm;

  std::vector<G4double> absoThicknessVec(2);
  if (fAbsorberUseUnits) {
    absoThicknessVec[0] = fAbsorberLenghtIncm;
  } else {
    absoThicknessVec[0] =
        fAbsorberLenghtInRadLen * absorberMaterial->GetRadlen();
  }

  constexpr G4double gapZOffset = 10.0 * CLHEP::cm;
  std::vector<G4double> layerThicknessVec(1);
  layerThicknessVec[0] =
      absoThicknessVec[0] + gapThickness + gapZOffset + gapThickness;

  G4double calorThickness = 0.0;
  for (const auto layThick : layerThicknessVec) {
    calorThickness += layThick;
  }

  // Envelope parameters
  constexpr G4double env_sizeXY = 100 * CLHEP::cm;
  constexpr G4double env_sizeZ  = 100 * CLHEP::cm;
  // World
  constexpr G4double worldSizeXY = 1.2 * env_sizeXY;
  constexpr G4double worldSizeZ  = 1.2 * env_sizeZ;

  if (!defaultMaterial || !absorberMaterial || !gapMaterial) {
    G4ExceptionDescription msg;
    msg << "Cannot retrieve materials already defined.";
    G4Exception("DetectorConstruction::DefineVolumes()", "Code0001",
                FatalException, msg);
  }

  //
  // World
  //
  solidWorld = new G4Box("World", worldSizeXY / 2.0, worldSizeXY / 2.0,
                         worldSizeZ / 2.0);

  logicWorld = new G4LogicalVolume(solidWorld, defaultMaterial, "World");

  physiWorld = new G4PVPlacement(0,               // no rotation
                                 G4ThreeVector(), // at (0,0,0)
                                 logicWorld,      // its logical volume
                                 "World",         // its name
                                 0,               // its mother  volume
                                 false,           // no boolean operation
                                 0,               // copy number
                                 fCheckOverlaps); // checking overlaps

  // Just a gap layer in front of the calo to start timing
  solidGap =
      new G4Box("Gap", gapSizeXY / 2.0, gapSizeXY / 2.0, gapThickness / 2.0);
  logicLayerBefore = new G4LogicalVolume(solidGap, gapMaterial, "preLayerLV");
  physiLayerBefore = new G4PVPlacement(
      nullptr, G4ThreeVector(0., 0., gapThickness / 2.0), logicLayerBefore,
      "preLayer", logicWorld, false, fCheckOverlaps);

  //
  // Calorimeter
  //
  solidCalo = new G4Box("Calorimeter", calorSizeXY / 2.0, calorSizeXY / 2.0,
                        calorThickness / 2.0);

  logicCalo = new G4LogicalVolume(solidCalo, defaultMaterial, "Calorimeter");

  physiCalo = new G4PVPlacement(
      0, G4ThreeVector(0., 0., gapThickness + calorThickness / 2.0), logicCalo,
      "Calorimeter", logicWorld, false, 0, fCheckOverlaps);

  //
  // Layer
  //
  for (G4int i = 0; i < 1; ++i) {
    solidLayers[i] = new G4Box("Layer", calorSizeXY / 2.0, calorSizeXY / 2.0,
                               layerThicknessVec[i] / 2.0);
  }

  G4double temp_layer_posZ = -calorThickness * 0.5;
  for (G4int i = 0; i < 1; ++i) {
    if (i > 1) {
      temp_layer_posZ +=
          0.5 * (layerThicknessVec[i - 1] + layerThicknessVec[i]);
    } else {
      temp_layer_posZ += 0.5 * layerThicknessVec[i];
    }
    std::ostringstream layerName, layerLVName;
    layerName << "Layer" << i;
    layerLVName << "LayerLV" << i;

    logicLayers[i] =
        new G4LogicalVolume(solidLayers[i], defaultMaterial, layerLVName.str());

    physiLayers[i] = new G4PVPlacement(
        0, G4ThreeVector(0., 0., temp_layer_posZ), logicLayers[i],
        layerName.str(), logicCalo, false, 0, fCheckOverlaps);
  }

  //
  // Absorber
  //
  for (G4int i = 0; i < 1; ++i) {
    solidAbsorbers[i] = new G4Box("Abso", calorSizeXY / 2.0, calorSizeXY / 2.0,
                                  absoThicknessVec[i] / 2.0);
  }

  // loop for setting up Logical Volumes and placements of the gaps and
  // absorbers and radiators
  for (G4int i = 0; i < 1; ++i) {
    std::ostringstream gapLVName, absorbersLVName;
    gapLVName << "GapLV" << i;
    absorbersLVName << "AbsoLV" << i;
    logicAbsorbers[i] = new G4LogicalVolume(solidAbsorbers[i], absorberMaterial,
                                            absorbersLVName.str());
    logicGaps[i] = new G4LogicalVolume(solidGap, gapMaterial, gapLVName.str());
    std::ostringstream aName, gName;
    aName << "absorber" << i;
    gName << "gap" << i;
    physiAbsorbers[i] = new G4PVPlacement(
        nullptr,
        G4ThreeVector(0., 0.,
                      -layerThicknessVec[i] * 0.5 + absoThicknessVec[i] * 0.5),
        logicAbsorbers[i], aName.str(), logicLayers[i], 0, i);
    // adding gap at physical volume
    physiGaps[i] = new G4PVPlacement(
        nullptr,
        G4ThreeVector(0., 0.,
                      -layerThicknessVec[i] * 0.5 + absoThicknessVec[i] +
                          gapThickness * 0.5),
        logicGaps[i], gName.str(), logicLayers[i], 0, i);
  }
  logicLastGap = new G4LogicalVolume(solidGap, gapMaterial, "lastGapLV");
  physiLastGap = new G4PVPlacement(
      nullptr,
      G4ThreeVector(0., 0.,
                    -layerThicknessVec[0] * 0.5 + absoThicknessVec[0] +
                        gapThickness + gapZOffset + 0.5 * gapThickness),
      logicLastGap, "lastgap", logicLayers[0], 0, 0);

  const G4double gapSpacing = (physiLastGap->GetTranslation().z() -
                               physiGaps[0]->GetTranslation().z()) /
                              fnGapsAfterAbs;
  for (G4int i = 0; i < fnGapsAfterAbs; ++i) {
    const G4double tempZ = -layerThicknessVec[0] * 0.5 + absoThicknessVec[0] +
                           gapThickness + 0.5 * gapThickness + i * gapSpacing;
    std::ostringstream gapLVName, gName;
    gapLVName << "aGapLV" << i;
    gName << "agap" << i;
    logicGapsAfterAbs[i] =
        new G4LogicalVolume(solidGap, gapMaterial, gapLVName.str());
    physiGapsAfterAbs[i] = new G4PVPlacement(
        nullptr, G4ThreeVector(0., 0., tempZ), logicGapsAfterAbs[i],
        gName.str(), logicLayers[0], 0, i);
  }

  //
  // print parameters
  //
  G4cout << G4endl
         << "------------------------------------------------------------"
         << G4endl << "---> The calorimeter is " << 1 << G4endl;
  G4cout << " RadLen of PB "
         << G4BestUnit(absorberMaterial->GetRadlen(), "Length") << G4endl;
  for (G4int i = 0; i < 1; ++i) {
    G4cout << " layers of: [ " << G4BestUnit(absoThicknessVec[i], "Length")
           << " of " << absorberMaterial->GetName() << " + "
           << G4BestUnit(gapThickness, "Length") << "of "
           << gapMaterial->GetName() << " ] " << G4endl
           << "------------------------------------------------------------"
           << G4endl;
  }

  //
  // Visualization attributes
  //
  logicWorld->SetVisAttributes(G4VisAttributes::GetInvisible());

  // auto simpleBoxVisAtt = new G4VisAttributes(G4Colour(1.0, 1.0, 1.0));
  // simpleBoxVisAtt->SetVisibility(false);
  // logicCalo->SetVisAttributes(simpleBoxVisAtt);

  // Setting red for absorber
  auto red = new G4VisAttributes(G4Colour::Red());
  red->SetVisibility(true);
  // absorberLV->SetVisAttributes(red);

  // Setting yellow for scintillator
  auto yellow = new G4VisAttributes(G4Colour::Yellow());
  yellow->SetForceSolid(true);
  // gapLV->SetVisAttributes(yellow);
  auto cyan = new G4VisAttributes(G4Colour::Cyan());
  cyan->SetVisibility(true);

  auto magenta = new G4VisAttributes(G4Colour::Magenta());
  magenta->SetVisibility(true);

  red->SetForceSolid(true);
  yellow->SetVisibility(true);
  cyan->SetForceSolid(true);

  logicLayerBefore->SetVisAttributes(yellow);
  for (G4int i = 0; i < 1; ++i) {
    logicAbsorbers[i]->SetVisAttributes(red);
    logicGaps[i]->SetVisAttributes(yellow);
    // logicLayers[i]->SetVisAttributes(red);
  }

  for (G4int i = 0; i < fnGapsAfterAbs; ++i) {
    logicGapsAfterAbs[i]->SetVisAttributes(magenta);
  }
  logicLastGap->SetVisAttributes(cyan);
  //
  // Always return the physical World
  //
  return physiWorld;
}

void DetectorConstruction::ConstructSDandField() {
  G4SDManager::GetSDMpointer()->SetVerboseLevel(1);

  // Set pre gap
  if (!fgapSD.Get()) {
    GapSD* gapSD = new GapSD("/det/gap");
    fgapSD.Put(gapSD);
  }
  G4SDManager::GetSDMpointer()->AddNewDetector(fgapSD.Get());
  SetSensitiveDetector(logicLayerBefore, fgapSD.Get());

  // Set gaps
  for (G4int i = 0; i < 1; ++i) {
    G4SDManager::GetSDMpointer()->AddNewDetector(fgapSD.Get());
    SetSensitiveDetector(logicGaps[i], fgapSD.Get());
  }

  // Set middle gaps
  for (G4int i = 0; i < fnGapsAfterAbs; ++i) {
    G4SDManager::GetSDMpointer()->AddNewDetector(fgapSD.Get());
    SetSensitiveDetector(logicGapsAfterAbs[i], fgapSD.Get());
  }

  // Set last gap
  G4SDManager::GetSDMpointer()->AddNewDetector(fgapSD.Get());
  SetSensitiveDetector(logicLastGap, fgapSD.Get());

  // Get the Energy left at first absorber
  G4VPrimitiveScorer* primitive;
  primitive = new G4PSEnergyDeposit("Edep");
  if (!fabsorberMFDet.Get()) {
    auto absDetector = new G4MultiFunctionalDetector("Absorber");
    absDetector->RegisterPrimitive(primitive);
    fabsorberMFDet.Put(absDetector);
  }
  G4SDManager::GetSDMpointer()->AddNewDetector(fabsorberMFDet.Get());
  SetSensitiveDetector(logicAbsorbers[0], fabsorberMFDet.Get());

  // primitive = new G4PSTrackLength("TrackLength");
  // auto charged = new G4SDChargedFilter("chargedFilter");
  // primitive ->SetFilter(charged);
  // absDetector->RegisterPrimitive(primitive);

  // primitive = new G4PSTrackLength("TrackLength");
  // primitive ->SetFilter(charged);
  // gapDetector->RegisterPrimitive(primitive);
}

// void DetectorConstruction::AddOpticalPropertiesVacuum(G4Material* matVacuum) {
//   G4double photonWavelength[] = {0.24 * micrometer, 0.1 * micrometer};
//   const G4int nEntries        = sizeof(photonWavelength) / sizeof(G4double);

//   G4double photonEnergy[nEntries];
//   for (int i = nEntries; i--;)
//     photonEnergy[i] = h_Planck * c_light / photonWavelength[i];

//   G4double refractiveIndex[nEntries];
//   for (int i = nEntries; i--;)
//     refractiveIndex[i] = 1.;

//   //  material property table
//   G4MaterialPropertiesTable* MPT = new G4MaterialPropertiesTable();
//   MPT->AddProperty("RINDEX", photonEnergy, refractiveIndex, nEntries)
//       ->SetSpline(true);
//   // MPT->AddProperty("ABSLENGTH", photonEnergy, abslength, nEntries)
//   //->SetSpline(true);
//   matVacuum->SetMaterialPropertiesTable(MPT);
// }

void DetectorConstruction::UpdateGeometry() {

  G4RunManager::GetRunManager()->DefineWorldVolume(Construct());
  G4RunManager::GetRunManager()->PhysicsHasBeenModified();
  // G4RegionStore::GetInstance()->UpdateMaterialList(physiWorld);

  G4RunManager::GetRunManager()->ReinitializeGeometry();
}
