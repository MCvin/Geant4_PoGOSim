//
//  TrackerSD.cc 2013-09-04  Maxime Chauvin
//

#include "TrackerSD.hh"

#include "G4VProcess.hh"
#include "G4Track.hh"
#include "G4SDManager.hh"

TrackerSD::TrackerSD(G4String name) : G4VSensitiveDetector(name)
{
  collectionName.insert(name);
}

TrackerSD::~TrackerSD(){}

void TrackerSD::Initialize(G4HCofThisEvent* )
{
  // create a new "hits collection"
  m_HitsCollection = new TrackerHitsCollection(SensitiveDetectorName,collectionName[0]); 
  // initialize TrackID and CollID to identify a new track or a new detector
  TrackID = -1;
  for (G4int i = 0; i < 61; i++) CollID[i] = -1;
}

G4bool TrackerSD::ProcessHits(G4Step* aStep, G4TouchableHistory*)
{
  // get track, process name, scintillator id, particle name, energy deposit
  G4Track* aTrack = aStep->GetTrack();
  G4String processName = aStep->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName();
  G4int id = aStep->GetPreStepPoint()->GetTouchableHandle()->GetCopyNumber(1);	//CopyNumber of the detector mother volume
  G4String particleName = aTrack->GetDefinition()->GetParticleName();
  G4double Edep = aStep->GetTotalEnergyDeposit();

  // if the process is tranportation, do nothing
  if ((processName == "Transportation") || (Edep == 0)) return true;

  // check if detector or particule is new to create a new "hit"
  if ((CollID[id] == -1) || (TrackID != aTrack->GetTrackID())) {
    TrackerHit* newHit = new TrackerHit();
    newHit->SetCharge( aTrack->GetDefinition()->GetPDGCharge());
    newHit->SetId( id );
    newHit->SetEdep( Edep );
    newHit->SetStepLength( aStep->GetStepLength() );
    newHit->SetPosition( aStep->GetPreStepPoint()->GetPosition() );
    newHit->SetPostPosition( aStep->GetPostStepPoint()->GetPosition() );
    newHit->SetMomentum( aStep->GetPreStepPoint()->GetMomentumDirection() );
    newHit->SetPostMomentum( aStep->GetPostStepPoint()->GetMomentumDirection() );
    newHit->SetParticleName( particleName );
    newHit->SetKineticEnergy( aStep->GetPreStepPoint()->GetKineticEnergy() );
    newHit->SetPostKineticEnergy( aStep->GetPostStepPoint()->GetKineticEnergy() );
    newHit->SetProcessName( aStep->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName());
    newHit->SetPolarization( aStep->GetPreStepPoint()->GetPolarization() );
    newHit->SetPostPolarization( aStep->GetPostStepPoint()->GetPolarization() );

    // register the new Hit to the hits collection
    CollID[id] = m_HitsCollection->insert(newHit);
    TrackID = aTrack->GetTrackID();
  } else {
    // if the detector has already been hit and the track is the same, add the energy to the hits collection
    (*m_HitsCollection)[CollID[id]-1]->AddEdep( Edep );
  }

  return true;
}

void TrackerSD::EndOfEvent(G4HCofThisEvent* HCE)
{
  G4int HCID = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);

  // register "hits collection" to HCE
  HCE->AddHitsCollection(HCID, m_HitsCollection); 
}
