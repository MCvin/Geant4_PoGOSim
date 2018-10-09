//
//  StructureConstruction.hh 2014-12-09  Maxime Chauvin
//

#ifndef StructureConstruction_h
#define StructureConstruction_h 1

#include "G4LogicalVolume.hh"
#include "G4SystemOfUnits.hh"

class StructureConstruction
{
  public:
    StructureConstruction(G4LogicalVolume*, G4LogicalVolume*, G4bool);
   ~StructureConstruction();

  private:
    //Some rotation matrices
    G4RotationMatrix *xRot1, *xRot10, *xRot15, *xRot20, *xRot30, *xRot40, *xRot50, *xRot45, *xRot60, *xRot75, *xRot80, *xRotflat;
    G4RotationMatrix *zRot45, *zRot90, *zRot135, *zRot180, *zRot225, *zRot270, *zRot315;
    G4bool overlap_check;

    void DefineRotations()
    {
      xRot1 = new G4RotationMatrix();       xRot1->rotateX( -1.*deg);      
      xRot10 = new G4RotationMatrix();     xRot10->rotateX(-10.*deg);
      xRot15 = new G4RotationMatrix();     xRot15->rotateX(-15.*deg);
      xRot20 = new G4RotationMatrix();     xRot20->rotateX(-21.*deg);
      xRot30 = new G4RotationMatrix();     xRot30->rotateX(-30.*deg);
      xRot40 = new G4RotationMatrix();     xRot40->rotateX(-40.*deg);
      xRot45 = new G4RotationMatrix();     xRot45->rotateX(-45.*deg);
      xRot50 = new G4RotationMatrix();     xRot50->rotateX(-50.*deg);
      xRot60 = new G4RotationMatrix();     xRot60->rotateX(-60.*deg);
      xRot75 = new G4RotationMatrix();     xRot75->rotateX(-75.*deg);
      xRot80 = new G4RotationMatrix();     xRot80->rotateX(-80.*deg);
      xRotflat = new G4RotationMatrix(); xRotflat->rotateX(-90.*deg);
      zRot45 = new G4RotationMatrix();     zRot45->rotateZ( 45.*deg);
      zRot90 = new G4RotationMatrix();     zRot90->rotateZ( 90.*deg);
      zRot135 = new G4RotationMatrix();   zRot135->rotateZ(135.*deg);
      zRot180 = new G4RotationMatrix();   zRot180->rotateZ(180.*deg);
      zRot225 = new G4RotationMatrix();   zRot225->rotateZ(225.*deg);
      zRot270 = new G4RotationMatrix();   zRot270->rotateZ(270.*deg);
      zRot315 = new G4RotationMatrix();   zRot315->rotateZ(315.*deg);
    }
};

#endif
