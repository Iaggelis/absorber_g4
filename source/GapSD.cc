#include "GapSD.hh"
#include "Analysis.hh"
#include "GapElectronHit.hh"

#include <G4HCofThisEvent.hh>
#include <G4SDManager.hh>
#include <G4Track.hh>
#include <G4VProcess.hh>

GapSD::GapSD(G4String name)
    : G4VSensitiveDetector(name), fGapElectronHitCollection(nullptr) {
  G4String HCname;
  collectionName.insert(HCname = "gapElectronCollection");
}

GapSD::~GapSD() {}

void GapSD::Initialize(G4HCofThisEvent* HCE) {
  fGapElectronHitCollection =
      new GapElectronHitsCollection(SensitiveDetectorName, collectionName[0]);
  static G4int HCID = -1;
  if (HCID < 0) {
    HCID = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);
  }
  HCE->AddHitsCollection(HCID, fGapElectronHitCollection);
}

G4bool GapSD::ProcessHits(G4Step* aStep, G4TouchableHistory*) {
  G4Track* theTrack = aStep->GetTrack();
  auto particleName = theTrack->GetParticleDefinition()->GetParticleName();

  // Get only electron when they step into the gap volume
  // if (particleName == "e-" && aStep->IsFirstStepInVolume()) {
  if (aStep->IsFirstStepInVolume()) {
    G4int ParentID       = theTrack->GetParentID();
    G4int TrackID        = theTrack->GetTrackID();
    G4double TrackLength = theTrack->GetTrackLength();
    auto newHit          = new GapElectronHit;
    newHit->SetGapName(theTrack->GetVolume()->GetName());
    newHit->SetParticlePDG(theTrack->GetParticleDefinition()->GetPDGEncoding());
    newHit->SetParentId(ParentID);
    newHit->SetTrId(TrackID);
    newHit->SetE(theTrack->GetTotalEnergy());
    newHit->SetMomentumDir(theTrack->GetMomentumDirection());
    newHit->SetPos(aStep->GetPostStepPoint()->GetPosition());
    // Global time start from the start of the run
    newHit->SetTime(aStep->GetPostStepPoint()->GetGlobalTime());
    // Local time start from particle creation
    // newHit->SetTime(aStep->GetPostStepPoint()->GetLocalTime());
    newHit->SetTrLen(TrackLength + aStep->GetStepLength());
    newHit->SetLocalTime(aStep->GetPostStepPoint()->GetGlobalTime());

    fGapElectronHitCollection->insert(newHit);

    return true;
  }
  return false;
}

void GapSD::EndOfEvent(G4HCofThisEvent*) {}

void GapSD::clear() {}

void GapSD::DrawAll() {}

void GapSD::PrintAll() {}
