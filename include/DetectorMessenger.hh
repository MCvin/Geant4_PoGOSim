//
//  DetectorMessenger.hh 2013-12-18  Maxime Chauvin
//

#ifndef DetectorMessenger_h
#define DetectorMessenger_h 1

#include "G4UImessenger.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"

class DetectorConstruction;

class DetectorMessenger: public G4UImessenger
{
  public:
    DetectorMessenger(DetectorConstruction* );
   ~DetectorMessenger();

    void SetNewValue(G4UIcommand*, G4String);
    
  private:
    DetectorConstruction* detector;

    G4UIcmdWithADoubleAndUnit* FastLengthCmd;
    G4UIcmdWithADoubleAndUnit* FastWidthCmd;
    G4UIcmdWithADoubleAndUnit* CollThicknessCmd;
    G4UIcmdWithADoubleAndUnit* RotationCmd;
    G4UIcmdWithADoubleAndUnit* ElevationCmd;
};

#endif

