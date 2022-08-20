#include "GapElectronHit.hh"

#include <G4AttCheck.hh>
#include <G4AttDef.hh>
#include <G4AttValue.hh>
#include <G4Circle.hh>
#include <G4Colour.hh>
#include <G4UnitsTable.hh>
#include <G4VVisManager.hh>
#include <G4VisAttributes.hh>

G4ThreadLocal G4Allocator<GapElectronHit>* GapElectronHitAllocator = nullptr;

std::map<G4String, G4AttDef> GapElectronHit::fAttDefs;

GapElectronHit::GapElectronHit()
    : G4VHit(), fPos(0.0), fMomentumDir(0.0), fTime(), fParentID(), fTrID(),
      fTrLen(), fLocalTime() {}

GapElectronHit::~GapElectronHit() {}

GapElectronHit::GapElectronHit(const GapElectronHit& rhs) : G4VHit() {
  fPos         = rhs.fPos;
  fMomentumDir = rhs.fMomentumDir;
  fTime        = rhs.fTime;
  fParentID    = rhs.fParentID;
  fTrID        = rhs.fTrID;
  fTrLen       = rhs.fTrLen;
  fLocalTime   = rhs.fLocalTime;
  fParName     = rhs.fParName;
  fGapName     = rhs.fGapName;
}

const GapElectronHit& GapElectronHit::operator=(const GapElectronHit& rhs) {
  fPos         = rhs.fPos;
  fMomentumDir = rhs.fMomentumDir;
  fTime        = rhs.fTime;
  fParentID    = rhs.fParentID;
  fTrID        = rhs.fTrID;
  fTrLen       = rhs.fTrLen;
  fLocalTime   = rhs.fLocalTime;
  fParName     = rhs.fParName;
  fGapName     = rhs.fGapName;
  return *this;
}

G4bool GapElectronHit::operator==(const GapElectronHit& rhs) const {
  return (this == &rhs) ? true : false;
}

const std::map<G4String, G4AttDef>* GapElectronHit::GetAttDefs() const {
  if (fAttDefs.empty()) {
    fAttDefs["HitType"] =
        G4AttDef("HitType", "Type of hit", "Physics", "", "G4String");
  }
  return &fAttDefs;
}

std::vector<G4AttValue>* GapElectronHit::CreateAttValues() const {
  std::vector<G4AttValue>* attValues = new std::vector<G4AttValue>;
  attValues->emplace_back("HistType", "GapElectronHit", "");
  return attValues;
}

void GapElectronHit::Print() {}
