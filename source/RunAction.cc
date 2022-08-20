#include "RunAction.hh"
#include "Analysis.hh"
#include "DetectorConstruction.hh"
#include "PrimaryGeneratorAction.hh"

#include <G4Run.hh>
#include <G4RunManager.hh>
#include <G4SystemOfUnits.hh>
#include <G4UnitsTable.hh>

RunAction::RunAction(EventAction* eventAction,
                     DetectorConstruction* detConstruction,
                     PrimaryGeneratorAction* primaryGenAction)
    : G4UserRunAction(), fEventAction(eventAction),
      fDetConstruction(detConstruction),
      fPrimaryGeneratorAction(primaryGenAction) {
  // print event number after each event
  G4RunManager::GetRunManager()->SetPrintProgress(1);

  // create the analysis manager
  // the analysis method is choosed from myAnalysis.hh
  auto analysisManager = G4AnalysisManager::Instance();

  if (analysisManager->GetType() == "Root") {
    analysisManager->SetVerboseLevel(0);
    analysisManager->SetNtupleMerging(true);
    analysisManager->SetFileName("output_file");
  }

  // create histograms
  // analysisManager->CreateH1("Eabs", "Edep in absorber", 100, 0., 5.*GeV);

  // create ntuples
  // electron from gaps
  analysisManager->CreateNtuple("Gaps", "Electron Ntuple");
  analysisManager->CreateNtupleIColumn("GapID",
                                       fEventAction->fParticles.det_id);
  analysisManager->CreateNtupleIColumn("ParentID",
                                       fEventAction->fParticles.parent_id);
  analysisManager->CreateNtupleIColumn("TRID", fEventAction->fParticles.trid);
  analysisManager->CreateNtupleIColumn("PDG", fEventAction->fParticles.pdgid);
  analysisManager->CreateNtupleDColumn("Energy",
                                       fEventAction->fParticles.energy);
  analysisManager->CreateNtupleDColumn("PosX", fEventAction->fParticles.posX);
  analysisManager->CreateNtupleDColumn("PosY", fEventAction->fParticles.posY);
  analysisManager->CreateNtupleDColumn("PosZ", fEventAction->fParticles.posZ);
  analysisManager->CreateNtupleDColumn("DirX", fEventAction->fParticles.dirX);
  analysisManager->CreateNtupleDColumn("DirY", fEventAction->fParticles.dirY);
  analysisManager->CreateNtupleDColumn("DirZ", fEventAction->fParticles.dirZ);
  analysisManager->CreateNtupleDColumn("Theta", fEventAction->fParticles.theta);
  analysisManager->CreateNtupleDColumn("Phi", fEventAction->fParticles.phi);
  analysisManager->CreateNtupleDColumn("Time", fEventAction->fParticles.times);
  // analysisManager->CreateNtupleDColumn("TrLen",
  // fEventAction->fParticles.trel);
  // analysisManager->CreateNtupleDColumn("Eabs");
  analysisManager->FinishNtuple();
}

RunAction::~RunAction() { delete G4AnalysisManager::Instance(); }

void RunAction::BeginOfRunAction(const G4Run*) {
  // // inform the runManager to save random number seed
  // G4RunManager::GetRunManager()->SetRandomNumberStore(false);
  auto analysisManager = G4AnalysisManager::Instance();

  analysisManager->OpenFile();
}

void RunAction::EndOfRunAction(const G4Run* aRun) {
  auto analysisManager = G4AnalysisManager::Instance();

  // if (analysisManager->GetH1(1)) {
  //   G4cout << G4endl << "print histo statistic ";
  //   if (isMaster) {
  //     G4cout << "for the entire run " << G4endl << G4endl;
  //   }
  // } else {
  //   G4cout << "for the local thread " << G4endl << G4endl;
  // }
  G4int n_run = aRun->GetRunID();
  G4cout << "INFO: run : " << n_run << G4endl;

  // TODO: print here the stats

  // save and close the file
  analysisManager->Write();
  analysisManager->CloseFile();
}
