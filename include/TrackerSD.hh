//
//  TrackerSD.hh 2013-09-04  Maxime Chauvin
//

#ifndef TrackerSD_h
#define TrackerSD_h 1

#include "TrackerHit.hh"

#include "G4VSensitiveDetector.hh"
#include "G4HCofThisEvent.hh"
#include "G4Step.hh"

class TrackerSD : public G4VSensitiveDetector
{
  public:
    TrackerSD(G4String);
   ~TrackerSD();

    void Initialize(G4HCofThisEvent*);
    G4bool ProcessHits(G4Step*, G4TouchableHistory*);
    void EndOfEvent(G4HCofThisEvent*);

  private:
    TrackerHitsCollection* m_HitsCollection;
    int TrackID;
    int CollID[61];
};

#endif
