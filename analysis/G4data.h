#ifndef G4data_h
#define G4data_h
#include <TTree.h>
#include <TLeaf.h>

struct Prim_event{
  int Id;
  float E;
  float Px, Py, Pz;
  float Dx, Dy, Dz;
  float PolX, PolY, PolZ;
  inline void SetAddress(TBranch* branch){
    branch->SetAddress(this);
  }
};
struct Det_event{
  int nhits;
  float Edep[1000000];
  float Px[1000000];
  float Py[1000000];
  float Pz[1000000];
  int UnitId[1000000];
  int nUnits;
  float EdepTot[61];
  inline void SetAddress(TBranch* branch){
    branch->SetAddress(this);
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
struct G4data{
  float Inst_roll;
  Prim_event* primary;
  Det_event* fast;
  Det_event* bottomBGO;
  Det_event* SAS;
  G4data(){
  primary = new Prim_event;
  fast = new Det_event;
  bottomBGO = new Det_event;
  SAS = new Det_event;
  }
  inline void SetAddress(TTree* tree){
    tree->GetBranch("Inst_roll")->SetAddress(&Inst_roll);
    primary->SetAddress(tree->GetBranch("primary"));
    fast->SetAddress(tree->GetBranch("fast"));
    bottomBGO->SetAddress(tree->GetBranch("bottomBGO"));
    SAS->SetAddress(tree->GetBranch("SAS"));
  }
};

struct Processed_event{
  int nUnits;
  float EdepTot[91];
  int UnitId[91];
  inline void Init(){
    nUnits = 0;
    for (int x = 0; x < 91; x++) {
      EdepTot[x] = 0;
      UnitId[x] = -1;
    }
  }
  inline void Update(){
    nUnits = 0;
    for (int x = 0; x < 91; x++) {
      if (EdepTot[x] > 0) {
	      UnitId[nUnits] = x;
	      nUnits++;
      }
    }
  }
};
struct Processed_data{
  Processed_event *fast;
  Processed_event *bottomBGO;
  Processed_event *SAS;
  Processed_event *PMT_fast;
  Processed_event *PMT_slow;
  Processed_data(){
    fast = new Processed_event;
    bottomBGO = new Processed_event;
    SAS = new Processed_event;
    PMT_fast = new Processed_event;
    PMT_slow = new Processed_event;
  }
  inline void Init(){
    fast->Init();
    bottomBGO->Init();
    SAS->Init();
    PMT_fast->Init();
    PMT_slow->Init();
  }
};

#endif
