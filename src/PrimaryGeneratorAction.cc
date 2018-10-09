//
//  PrimaryGeneratorAction.cc 2013-09-04  Maxime Chauvin
//

#include "PrimaryGeneratorAction.hh"
#include "G4SystemOfUnits.hh"
#include <stdexcept>

PrimaryGeneratorAction::PrimaryGeneratorAction()
{
  // create commands for interactive definition of the primary generator
  primaryGeneratorMessenger = new PrimaryGeneratorMessenger(this);

  // definition of the general particle source (to use with macro commands)
  gps = new G4GeneralParticleSource();

  // definition of the particle gun (to use with an input source file)
  particleGun = new G4ParticleGun(1);
  G4ParticleDefinition* particle = G4ParticleTable::GetParticleTable()->FindParticle("gamma");
  particleGun->SetParticleDefinition(particle);
}

PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
  delete gps;
  delete particleGun;
  delete asciiFile;
}

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
  if(asciiFile) {
    // particules generation from input ascii file
    (*asciiFile) >> energy >> position >> momentumDirection >> polarization;
    if (asciiFile->eof()) {
      asciiFile->clear(); asciiFile->seekg(0);  // clear the failed eof state and rewind the stream to the beginning
      (*asciiFile) >> energy >> position >> momentumDirection >> polarization;
    }
    // Z position of top of Mylar = 819.01*mm
    position.setZ(850.0*mm); // Z position of the scattered source output from the rotation + shutter holder 20160626
    //position.setZ(854.0*mm); // Z position of the scattered source output from the rotation + shutter holder 20160317
    //position.setZ(1014.0*mm); // Z position of the 2016 source output from the rotation + shutter holder 20160317
    //position += G4ThreeVector(0.0*mm,56.0*mm,0.0*mm); // for ring 2 centering
    //position += G4ThreeVector(0.0*mm,-90.0*mm,0.0*mm); // for unit 28 centering
    //G4cout << energy << position << momentumDirection << polarization << G4endl;
    particleGun->SetParticleEnergy(energy*keV);
    particleGun->SetParticlePosition(position);
    particleGun->SetParticleMomentumDirection(momentumDirection);
    particleGun->SetParticlePolarization(polarization);
    particleGun->GeneratePrimaryVertex(anEvent);
  } else {
    // particules generation via GPS
    gps->GeneratePrimaryVertex(anEvent);
  }
}

void PrimaryGeneratorAction::ReadFile(const G4String name)
{
  delete asciiFile;
  asciiFile = new std::ifstream(name);
  if (!(asciiFile->is_open())) throw std::runtime_error("unable to open events source file");
}
