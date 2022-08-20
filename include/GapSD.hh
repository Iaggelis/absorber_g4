#ifndef GAPSD_H_
#define GAPSD_H_ 1

#include "GapElectronHit.hh"

#include <G4VSensitiveDetector.hh> // Template class for SD

class G4Step;
class G4HCofThisEvent;
class G4TouchableHistory;

class GapSD : public G4VSensitiveDetector {
public:
  GapSD(G4String name);
  virtual ~GapSD();

  virtual void Initialize(G4HCofThisEvent* HCE);
  virtual G4bool ProcessHits(G4Step* aStep, G4TouchableHistory* ROhist);
  virtual void EndOfEvent(G4HCofThisEvent* HCE);
  virtual void clear();
  virtual void DrawAll();
  virtual void PrintAll();

private:
  GapElectronHitsCollection* fGapElectronHitCollection;
};

#endif // MYGAPSD_H_
