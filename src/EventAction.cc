//
//  EventAction.cc 2013-09-04  Maxime Chauvin
//

#include "EventAction.hh"

#include "G4SDManager.hh"

EventAction::EventAction(DetectorConstruction* det): detector(det), fastCollID(-1), slowCollID(-1), bottomBGOCollID(-1), SASCollID(-1)
{
  printModulo = 10000;
}

EventAction::~EventAction()
{
  G4dataTree->Write();
  rootFile->Close();
}

void EventAction::BeginOfEventAction(const G4Event* evt)
{
  if (evt->GetEventID()%printModulo == 0) G4cout << "    ---> Begin of event: " << evt->GetEventID() << "\n" << G4endl;

  G4SDManager * SDman = G4SDManager::GetSDMpointer();
  if (fastCollID==-1) fastCollID = SDman->GetCollectionID("fastSD");
  if (slowCollID==-1) slowCollID = SDman->GetCollectionID("slowSD");
  if (bottomBGOCollID==-1) bottomBGOCollID = SDman->GetCollectionID("bottomBGOSD");
  if (SASCollID==-1) SASCollID = SDman->GetCollectionID("SASSD");
}

void EventAction::EndOfEventAction(const G4Event* evt)
{
  // write to ROOT file
  WriteToROOT(evt);
}

void EventAction::CreateROOTFile(const G4String name)
{
  rootFile = new TFile(name,"RECREATE");
  G4dataTree = new TTree( "G4data" , "PoGOSIM data" );
  G4dataTree->Branch("Inst_roll", &Inst_roll, "Inst_roll/F");
  G4dataTree->Branch("primary", &primEvent, "Id/I:Energy/F:Px/F:Py/F:Pz/F:Dx/F:Dy/F:Dz/F:PolX/F:PolY/F:PolZ/F");
  TBranch* slow_branch = G4dataTree->Branch("slow", &slowEvent, "nhits/I:Edep[nhits]/F:Px[nhits]/F:Py[nhits]/F:Pz[nhits]/F:UnitId[nhits]/I:nUnits/I:EdepTot[61]/F");
  slowEvent.SetAddress(slow_branch); //Set the address for each leaf explicitly (IMPORTANT!)
  TBranch* fast_branch = G4dataTree->Branch("fast", &fastEvent, "nhits/I:Edep[nhits]/F:Px[nhits]/F:Py[nhits]/F:Pz[nhits]/F:UnitId[nhits]/I:nUnits/I:EdepTot[61]/F");
  fastEvent.SetAddress(fast_branch); //Set the address for each leaf explicitly (IMPORTANT!)
  TBranch* bottomBGO_branch = G4dataTree->Branch("bottomBGO", &bottomBGOEvent, "nhits/I:Edep[nhits]/F:Px[nhits]/F:Py[nhits]/F:Pz[nhits]/F:UnitId[nhits]/I:nUnits/I:EdepTot[61]/F");
  bottomBGOEvent.SetAddress(bottomBGO_branch); //Set the address for each leaf explicitly (IMPORTANT!)
  TBranch* SAS_branch = G4dataTree->Branch("SAS", &SASEvent, "nhits/I:Edep[nhits]/F:Px[nhits]/F:Py[nhits]/F:Pz[nhits]/F:UnitId[nhits]/I:nUnits/I:EdepTot[61]/F");
  SASEvent.SetAddress(SAS_branch); //Set the address for each leaf explicitly (IMPORTANT!)
  G4dataTree->SetAutoSave(1000000000); // to avoid getting many trees in the root file
}

void EventAction::WriteToROOT(const G4Event* evt)
{
  G4HCofThisEvent* HCE = evt->GetHCofThisEvent();
  const G4PrimaryVertex* primaryVertex = evt->GetPrimaryVertex();
  G4PrimaryParticle* primary = evt->GetPrimaryVertex()->GetPrimary(0);

  // get everything we want to output
    Inst_roll = detector->GetPolarimeterRot()/deg;
    // primary particule
    primEvent.Id = evt->GetEventID();
    primEvent.E  = primary->GetKineticEnergy()/keV;
    primEvent.Px = primaryVertex->GetX0()/mm;
    primEvent.Py = primaryVertex->GetY0()/mm;
    primEvent.Pz = primaryVertex->GetZ0()/mm;
    primEvent.Dx = primary->GetPx();
    primEvent.Dy = primary->GetPy();
    primEvent.Dz = primary->GetPz();
    primEvent.PolX = primary->GetPolX();
    primEvent.PolY = primary->GetPolY();
    primEvent.PolZ = primary->GetPolZ();
    // slow scintillator hits
    TrackerHitsCollection* slowHC = 0;
    if (HCE) slowHC = (TrackerHitsCollection*)HCE->GetHC(slowCollID);
    if (slowHC) readHitsCollection(slowHC, &slowEvent);
    // fast scintillator hits
    TrackerHitsCollection* fastHC = 0;
    if (HCE) fastHC = (TrackerHitsCollection*)HCE->GetHC(fastCollID);
    if (fastHC) readHitsCollection(fastHC, &fastEvent);
    // bottom BGO hits
    TrackerHitsCollection* bottomBGOHC = 0;
    if (HCE) bottomBGOHC = (TrackerHitsCollection*)HCE->GetHC(bottomBGOCollID);
    if (bottomBGOHC) readHitsCollection(bottomBGOHC, &bottomBGOEvent);
    // SAS hits
    TrackerHitsCollection* SASHC = 0;
    if (HCE) SASHC = (TrackerHitsCollection*)HCE->GetHC(SASCollID);
    if (SASHC) readHitsCollection(SASHC, &SASEvent);

  // fill the Tree only if something happened in the PDC volumes
  if ((slowEvent.nhits > 0)||(fastEvent.nhits > 0)||(bottomBGOEvent.nhits > 0)) G4dataTree->Fill();
}

void EventAction::readHitsCollection(const TrackerHitsCollection* HC, Det_event* detEvt)
{
  G4double Edep;
  G4int UnitId;
  G4int nhits = 0;

  detEvt->nUnits = 0;
  for (G4int i = 0; i < 61; i++) detEvt->EdepTot[i] = 0.0;

  for (G4int i = 0; i < HC->entries(); i++){
    Edep = (*HC)[i]->GetEdep()/keV;
    UnitId = (*HC)[i]->GetId();

    //Quench particle's energy based on data by Verbinski et al.(1968). (except for O16  
    //proton
    if ((*HC)[i]->GetParticleName()=="proton" || (*HC)[i]->GetParticleName()=="deuteron"){
	  if (Edep < 1000.0)
	    Edep = 0.0564 * Edep + 0.000119 * Edep * Edep;
	  else
	    Edep = 0.1754 * Edep;
    }
    //alpha 
    else if ((*HC)[i]->GetParticleName()=="alpha"){
	  if (Edep < 4000.0)
	    Edep = 0.0145 * Edep + 0.00000878 * Edep * Edep;
	  else
	    Edep = 0.0545 * Edep;
    }
    else if ((*HC)[i]->GetParticleName()=="C12[0.0]"){
	  Edep = 0.0062 * Edep;
    }
    else if ((*HC)[i]->GetParticleName()=="O16[0.0]"){ 
	  Edep = 0.004 * Edep; //THIS IS A FROM AN EXTRAPOLATION BY MK
    }
    else if ((*HC)[i]->GetParticleName()=="gamma" || (*HC)[i]->GetParticleName()=="e-"|| (*HC)[i]->GetParticleName()=="neutron" || (*HC)[i]->GetParticleName()=="e+"){
	  Edep = Edep;
    }
    else {
	  Edep = 0;
    }

    detEvt->Edep[nhits] = Edep;
    detEvt->Px[nhits] = (*HC)[i]->GetPostPosition().x()/mm;
    detEvt->Py[nhits] = (*HC)[i]->GetPostPosition().y()/mm;
    detEvt->Pz[nhits] = (*HC)[i]->GetPostPosition().z()/mm;
    detEvt->UnitId[nhits] = UnitId;
    nhits++;

    if (detEvt->EdepTot[UnitId] == 0) detEvt->nUnits += 1;
    detEvt->EdepTot[UnitId] += Edep;
  }
  detEvt->nhits = nhits;

  return ;
}
