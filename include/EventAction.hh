//
//  EventAction.hh 2013-09-04  Maxime Chauvin
//

#ifndef EventAction_h
#define EventAction_h 1

#include "TrackerHit.hh"
#include "DetectorConstruction.hh"

#include "G4UserEventAction.hh"
#include "G4Event.hh"
#include "G4ThreeVector.hh"
#include "TFile.h"
#include "TTree.h"
#include "TLeaf.h"

class EventAction : public G4UserEventAction
{
  public:
    EventAction(DetectorConstruction*);
   ~EventAction();

    void BeginOfEventAction(const G4Event*);
    void EndOfEventAction(const G4Event*);
    void CreateROOTFile(const G4String name);

  private:
    DetectorConstruction* detector;
    struct Prim_event{
      int Id;
      float E;
      float Px, Py, Pz;
      float Dx, Dy, Dz;
      float PolX, PolY, PolZ;
    };
    struct Det_event{
      int nhits; //Number of hits determines the size of arrays when writing the TTree
      float Edep[1000000];
      float Px[1000000];
      float Py[1000000];
      float Pz[1000000];
      int UnitId[1000000];
      int nUnits;
      float EdepTot[61];
      inline void SetAddress(TBranch* branch){
        TObjArray* leaf_array = branch->GetListOfLeaves();
        ((TLeaf*)leaf_array->At(0))->SetAddress(&nhits);
        ((TLeaf*)leaf_array->At(1))->SetAddress(Edep);
        ((TLeaf*)leaf_array->At(2))->SetAddress(Px);
        ((TLeaf*)leaf_array->At(3))->SetAddress(Py);
        ((TLeaf*)leaf_array->At(4))->SetAddress(Pz);
        ((TLeaf*)leaf_array->At(5))->SetAddress(UnitId);
        ((TLeaf*)leaf_array->At(6))->SetAddress(&nUnits);
        ((TLeaf*)leaf_array->At(7))->SetAddress(EdepTot);
      }
    };
    void WriteToROOT(const G4Event*);
    void readHitsCollection(const TrackerHitsCollection*, Det_event*);

    G4int fastCollID, slowCollID, bottomBGOCollID, SASCollID;
    G4int printModulo;

    float Inst_roll;
    Prim_event primEvent;
    Det_event slowEvent, fastEvent, bottomBGOEvent, SASEvent;

    TFile *rootFile;
    TTree *G4dataTree;
};

#endif
