//
//  DetectorMessenger.hh 2014-11-04  Maxime Chauvin
//

#ifndef PrimaryGeneratorMessenger_h
#define PrimaryGeneratorMessenger_h 1

#include "G4UImessenger.hh"
#include "G4UIcmdWithAString.hh"

class PrimaryGeneratorAction;

class PrimaryGeneratorMessenger: public G4UImessenger
{
  public:
    PrimaryGeneratorMessenger(PrimaryGeneratorAction* );
   ~PrimaryGeneratorMessenger();
    
    void SetNewValue(G4UIcommand*, G4String);
    
  private:
    PrimaryGeneratorAction* action;

    G4UIcmdWithAString* genCmd;
};

#endif

