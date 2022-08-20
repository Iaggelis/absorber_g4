#ifndef GAPELECTRONHIT_H
#define GAPELECTRONHIT_H

#include <G4Allocator.hh>
#include <G4THitsCollection.hh>
#include <G4ThreeVector.hh>
#include <G4Types.hh>
#include <G4VHit.hh>

class G4AttDef;

class GapElectronHit : public G4VHit {
public:
  GapElectronHit();

  virtual ~GapElectronHit();

  GapElectronHit(const GapElectronHit& rhs);

  // operators
  const GapElectronHit& operator=(const GapElectronHit& rhs);
  G4bool operator==(const GapElectronHit& rhs) const;
  inline void* operator new(size_t);
  inline void operator delete(void* aHit);

  virtual const std::map<G4String, G4AttDef>* GetAttDefs() const;
  virtual std::vector<G4AttValue>* CreateAttValues() const;
  virtual void Print();

  // Setters and Getters
  inline void SetE(G4double en) { fEnergy = en; }
  inline auto GetE() -> G4double { return fEnergy; }

  inline void SetPos(G4ThreeVector vec3) { fPos = vec3; }
  inline auto GetPos() -> G4ThreeVector { return fPos; }

  inline void SetMomentumDir(G4ThreeVector vec3) { fMomentumDir = vec3; }
  inline auto GetMomentumDir() -> G4ThreeVector { return fMomentumDir; }

  inline void SetTime(G4double ti) { fTime = ti; }
  inline auto GetTime() -> G4double { return fTime; }

  inline void SetParentId(G4int parentID) { fParentID = parentID; }
  inline auto GetParentId() -> G4int { return fParentID; }

  inline void SetTrId(G4double tid) { fTrID = tid; }
  inline auto GetTrId() -> G4double { return fTrID; }

  inline void SetTrLen(G4double tl) { fTrLen = tl; }
  inline auto GetTrLen() -> G4double { return fTrLen; }

  inline void SetLocalTime(G4double ti) { fLocalTime = ti; }
  inline auto GetLocalTime() -> G4double { return fLocalTime; }

  inline void SetParticleName(G4String par_name) { fParName = par_name; }
  inline auto GetParticleName() -> G4String { return fParName; }

  inline void SetParticlePDG(G4int par_pdg) { fParPDG = par_pdg; }
  inline auto GetParticlePDG() -> G4int { return fParPDG; }

  inline void SetGapName(G4String gap_name) { fGapName = gap_name; }
  inline auto GetGapName() -> G4String { return fGapName; }

private:
  G4double fEnergy;           // Particle energy
  G4ThreeVector fPos;         // Position Vec3
  G4ThreeVector fMomentumDir; // Momentum Direction Vec3
  G4double fTime;             // time
  G4int fParentID;            // Particle ID
  G4int fTrID;                // Track ID
  G4double fTrLen;            // Track Length
  G4double fLocalTime;        // LocalTIme
  G4String fParName;          // Particle Name
  G4int fParPDG;              // Particle PDG Encoding
  G4String fGapName;          // Gap Name

  static std::map<G4String, G4AttDef> fAttDefs;
};

using GapElectronHitsCollection = G4THitsCollection<GapElectronHit>;

extern G4ThreadLocal G4Allocator<GapElectronHit>* GapElectronHitAllocator;

inline void* GapElectronHit::operator new(size_t) {
  if (!GapElectronHitAllocator)
    GapElectronHitAllocator = new G4Allocator<GapElectronHit>;
  return (void*)GapElectronHitAllocator->MallocSingle();
}

inline void GapElectronHit::operator delete(void* aHit) {
  GapElectronHitAllocator->FreeSingle((GapElectronHit*)aHit);
}

#endif // GapElectronHit.hh
