//
//  DetectorConstruction.hh 2013-09-04  Maxime Chauvin
//  Polarimeter rotation by Maxime Chauvin 2013-12-18
//

#ifndef DetectorConstruction_h
#define DetectorConstruction_h 1

#include "DetectorMessenger.hh"

#include "G4VUserDetectorConstruction.hh"
#include "G4VPhysicalVolume.hh"
#include "G4LogicalVolume.hh"
#include "G4SystemOfUnits.hh"
#include "TrackerSD.hh"

class DetectorConstruction : public G4VUserDetectorConstruction
{
  public:
    DetectorConstruction();
   ~DetectorConstruction();

    G4VPhysicalVolume* Construct();

    void SetFastLength(G4double);
    inline G4double GetFastLength() { return FastLength; };
    void SetFastWidth(G4double);
    inline G4double GetFastWidth() { return FastWidth; };
    void SetCollThickness(G4double);
    inline G4double GetCollThickness() { return CollThickness; };
    void SetPolarimeterRot(G4double);
    inline G4double GetPolarimeterRot() { return PolarimeterRot; };
    void SetElevation(G4double);
    inline G4double GetElevation() { return Elevation; };

  private:
    DetectorMessenger* detectorMessenger;
    G4VPhysicalVolume* p_Pogo;
    G4VPhysicalVolume* p_Polarimeter;

    G4double FastLength;
    G4double FastWidth;
    G4double CollThickness;
    G4double PolarimeterRot;
    G4double Elevation;
    //Some rotation matrices
    G4RotationMatrix *Rot0;
    G4RotationMatrix *xRot;
    G4RotationMatrix *zRot, *zRot60, *zRot120, *zRot180, *zRot240, *zRot300;
    G4bool overlap_check;

    TrackerSD* slowSD;
    TrackerSD* fastSD;
    TrackerSD* BGObottomSD;
    TrackerSD* SASSD;

    void DefineMaterials();
    void DefineRotations()
    {
      PolarimeterRot = 0;
      Elevation = 0;
      Rot0 = new G4RotationMatrix();
      xRot = new G4RotationMatrix();
      zRot = new G4RotationMatrix();
      zRot60 = new G4RotationMatrix();   zRot60->rotateZ(60.*deg);
      zRot120 = new G4RotationMatrix(); zRot120->rotateZ(120.*deg);
      zRot180 = new G4RotationMatrix(); zRot180->rotateZ(180.*deg);
      zRot240 = new G4RotationMatrix(); zRot240->rotateZ(240.*deg);
      zRot300 = new G4RotationMatrix(); zRot300->rotateZ(300.*deg);
    }
};

#endif
