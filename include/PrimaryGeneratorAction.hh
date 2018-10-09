//
//  PrimaryGeneratorAction.hh 2013-09-04  Maxime Chauvin
//

#ifndef PrimaryGeneratorAction_h
#define PrimaryGeneratorAction_h 1

#include "PrimaryGeneratorMessenger.hh"

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4GeneralParticleSource.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4Event.hh"
#include <fstream>

class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
  public:
    PrimaryGeneratorAction();
   ~PrimaryGeneratorAction();

  public:
    void GeneratePrimaries(G4Event*);
    void ReadFile(const G4String name);

  private:
    PrimaryGeneratorMessenger* primaryGeneratorMessenger;
    G4GeneralParticleSource* gps;
    G4ParticleGun* particleGun;
    std::ifstream* asciiFile;

    G4double energy;
    G4ThreeVector position;
    G4ThreeVector momentumDirection;
    G4ThreeVector polarization;
};

#endif
