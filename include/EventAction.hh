#ifndef EVENTACTION_H
#define EVENTACTION_H 1

#include <G4SystemOfUnits.hh>
#include <G4THitsMap.hh>
#include <G4UserEventAction.hh>
#include <globals.hh>

#include "GapElectronHit.hh"

struct Event {
  std::vector<G4int> det_id;
  std::vector<G4int> parent_id;
  std::vector<G4int> trid;
  std::vector<G4int> pdgid;
  std::vector<G4double> times;
  std::vector<G4double> energy;
  std::vector<G4double> posX;
  std::vector<G4double> posY;
  std::vector<G4double> posZ;
  std::vector<G4double> dirX;
  std::vector<G4double> dirY;
  std::vector<G4double> dirZ;
  std::vector<G4double> theta;
  std::vector<G4double> phi;
  std::vector<G4double> trlen;
  std::vector<G4int> n_secondaries;

  void Populate(const GapElectronHitsCollection* GapHC) {
    const std::size_t nHits = GapHC->entries();
    Reserve(nHits);
    const std::map<std::string, G4int> detIds = {
        {"preLayer", -1}, {"gap0", 0},    {"agap0", 1}, {"agap1", 2},
        {"agap2", 3},     {"agap3", 4},   {"agap4", 5}, {"agap5", 6},
        {"agap6", 7},     {"agap7", 8},   {"agap8", 9}, {"agap9", 10},
        {"agap10", 11},   {"lastgap", 99}};
    for (std::size_t i = 0; i < nHits; ++i) {
      det_id[i]    = detIds.at((*GapHC)[i]->GetGapName());
      parent_id[i] = ((*GapHC)[i]->GetParentId());
      trid[i]      = ((*GapHC)[i]->GetTrId());
      pdgid[i]     = ((*GapHC)[i]->GetParticlePDG());
      times[i]     = ((*GapHC)[i]->GetTime() / CLHEP::ns);
      energy[i]    = ((*GapHC)[i]->GetE() / CLHEP::MeV);
      posX[i]      = ((*GapHC)[i]->GetPos().x() / CLHEP::mm);
      posY[i]      = ((*GapHC)[i]->GetPos().y() / CLHEP::mm);
      posZ[i]      = ((*GapHC)[i]->GetPos().z() / CLHEP::mm);
      dirX[i]      = ((*GapHC)[i]->GetMomentumDir().x());
      dirY[i]      = ((*GapHC)[i]->GetMomentumDir().y());
      dirZ[i]      = ((*GapHC)[i]->GetMomentumDir().z());
      theta[i]     = ((*GapHC)[i]->GetPos().theta());
      phi[i]       = ((*GapHC)[i]->GetPos().phi());
      trlen[i]     = ((*GapHC)[i]->GetTrLen());
    }
  }

  void Reserve(const std::size_t nparticles) {
    det_id.resize(nparticles);
    parent_id.resize(nparticles);
    trid.resize(nparticles);
    pdgid.resize(nparticles);
    times.resize(nparticles);
    energy.resize(nparticles);
    posX.resize(nparticles);
    posY.resize(nparticles);
    posZ.resize(nparticles);
    dirX.resize(nparticles);
    dirY.resize(nparticles);
    dirZ.resize(nparticles);
    theta.resize(nparticles);
    phi.resize(nparticles);
    trlen.resize(nparticles);
    n_secondaries.resize(nparticles);
  }

  void ClearVecs() {
    det_id.clear();
    parent_id.clear();
    trid.clear();
    times.clear();
    energy.clear();
    posX.clear();
    posY.clear();
    posZ.clear();
    dirX.clear();
    dirY.clear();
    dirZ.clear();
    theta.clear();
    phi.clear();
    trlen.clear();
    n_secondaries.clear();
  }
};

// Event action class
class EventAction : public G4UserEventAction {
public:
  EventAction();
  virtual ~EventAction();

  virtual void BeginOfEventAction(const G4Event* event);
  virtual void EndOfEventAction(const G4Event* event);

  Event fParticles;

private:
  auto GetHitsCollection(G4int hcID, const G4Event* event) const
      -> G4THitsMap<G4double>*;
  auto GetSum(G4THitsMap<G4double>* hitsMap) const -> G4double;
  void PrintEventStatistics(G4double absoEdep, G4double gapEdep) const;

  G4int fAbsoEdepHCID;
  G4int fGapElectronCollID;

  G4int fNofGaps;
};

#endif
