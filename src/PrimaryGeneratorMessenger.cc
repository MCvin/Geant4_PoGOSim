//
//  PrimaryGeneratorMessenger.cc 2014-11-04  Maxime Chauvin
//

#include "PrimaryGeneratorMessenger.hh"
#include "PrimaryGeneratorAction.hh"

PrimaryGeneratorMessenger::PrimaryGeneratorMessenger(PrimaryGeneratorAction* pga): action(pga)
{
  genCmd = new G4UIcmdWithAString("/PrimaryGenerator/file",this);
  genCmd->SetGuidance("Input source events from ascii file.");
  genCmd->SetParameterName("File",true);
  genCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
}

PrimaryGeneratorMessenger::~PrimaryGeneratorMessenger()
{
  delete genCmd;
}

void PrimaryGeneratorMessenger::SetNewValue(G4UIcommand* command, G4String newValue)
{
  if( command == genCmd )
   { action->ReadFile(newValue);}
}
