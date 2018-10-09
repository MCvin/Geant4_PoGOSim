//
//  DetectorMessenger.cc 2013-12-18  Maxime Chauvin
//

#include "DetectorMessenger.hh"
#include "DetectorConstruction.hh"

DetectorMessenger::DetectorMessenger(DetectorConstruction* det): detector(det)
{
  FastLengthCmd = new G4UIcmdWithADoubleAndUnit("/PoGO+/SetFastLength",this);
  FastLengthCmd->SetGuidance("Set length of the fast scintillator");
  FastLengthCmd->SetParameterName("Fast length",false);
  FastLengthCmd->SetUnitCategory("Length");
  FastLengthCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  FastWidthCmd = new G4UIcmdWithADoubleAndUnit("/PoGO+/SetFastWidth",this);
  FastWidthCmd->SetGuidance("Set width of the fast scintillator");
  FastWidthCmd->SetParameterName("Fast width",false);
  FastWidthCmd->SetUnitCategory("Length");
  FastWidthCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  CollThicknessCmd = new G4UIcmdWithADoubleAndUnit("/PoGO+/SetCollThickness",this);
  CollThicknessCmd->SetGuidance("Set thickness of the passive collimator");
  CollThicknessCmd->SetParameterName("Collimator thickness",false);
  CollThicknessCmd->SetUnitCategory("Length");
  CollThicknessCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  RotationCmd = new G4UIcmdWithADoubleAndUnit("/PoGO+/SetPolarimeterRot",this);
  RotationCmd->SetGuidance("Set rotation of the polarimeter");
  RotationCmd->SetParameterName("Polarimeter rotation",false);
  RotationCmd->SetUnitCategory("Angle");
  RotationCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  ElevationCmd = new G4UIcmdWithADoubleAndUnit("/PoGO+/SetElevation",this);
  ElevationCmd->SetGuidance("Set elevation of the polarimeter");
  ElevationCmd->SetParameterName("Polarimeter elevation",false);
  ElevationCmd->SetUnitCategory("Angle");
  ElevationCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
}

DetectorMessenger::~DetectorMessenger()
{
  delete FastLengthCmd;
  delete FastWidthCmd;
  delete CollThicknessCmd;
  delete RotationCmd;
  delete ElevationCmd;
}

void DetectorMessenger::SetNewValue(G4UIcommand* command, G4String newValue)
{
  if( command == FastLengthCmd )
   { detector->SetFastLength(FastLengthCmd->GetNewDoubleValue(newValue));}
  if( command == FastWidthCmd )
   { detector->SetFastWidth(FastWidthCmd->GetNewDoubleValue(newValue));}
  if( command == CollThicknessCmd )
   { detector->SetCollThickness(CollThicknessCmd->GetNewDoubleValue(newValue));}
  if( command == RotationCmd )
   { detector->SetPolarimeterRot(RotationCmd->GetNewDoubleValue(newValue));}
  if( command == ElevationCmd )
   { detector->SetElevation(ElevationCmd->GetNewDoubleValue(newValue));}
}
