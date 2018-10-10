//
//  DetectorConstruction.cc 2013-09-04  Maxime Chauvin
//  SAS included by Merlin Kole 2013-10-22
//  Polarimeter rotation by Maxime Chauvin 2013-12-18
//

#include "DetectorConstruction.hh"
#include "StructureConstruction.hh"

#include "G4Material.hh"
#include "G4NistManager.hh"

#include "G4ThreeVector.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Trap.hh"
#include "G4Polyhedra.hh"
#include "G4Polycone.hh"
#include "G4UnionSolid.hh"
#include "G4SubtractionSolid.hh"
#include "G4IntersectionSolid.hh"
#include "G4PVPlacement.hh"

#include "G4GeometryManager.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4SolidStore.hh"

#include "G4VisAttributes.hh"
#include "G4VVisManager.hh"
#include "G4SDManager.hh"
#include "G4RunManager.hh"

DetectorConstruction::DetectorConstruction()
{
  // create commands for interactive definition of the detector
  detectorMessenger = new DetectorMessenger(this);

  // define the geometry parameters
  FastLength = 12.0*cm;
  FastWidth = 1.3875*cm;
  CollThickness = 0.5*mm;

  // define materials
  DefineMaterials();

  // define some rotation matrices
  DefineRotations();

  // geometry overlap check
  overlap_check = true;
}

DetectorConstruction::~DetectorConstruction()
{
  delete detectorMessenger;
}

void DetectorConstruction::DefineMaterials()
{
  G4NistManager * nist_manager = G4NistManager::Instance();
  // elements
  G4Element *elementH  = nist_manager->FindOrBuildElement("H");
  G4Element *elementC  = nist_manager->FindOrBuildElement("C");
  G4Element *elementSi = nist_manager->FindOrBuildElement("Si");
  G4Element *elementFe = nist_manager->FindOrBuildElement("Fe");
  G4Element *elementCu = nist_manager->FindOrBuildElement("Cu");
  G4Element *elementMn = nist_manager->FindOrBuildElement("Mn");
  G4Element *elementMg = nist_manager->FindOrBuildElement("Mg");
  G4Element *elementCr = nist_manager->FindOrBuildElement("Cr");
  G4Element *elementZn = nist_manager->FindOrBuildElement("Zn");
  G4Element *elementTi = nist_manager->FindOrBuildElement("Ti");
  G4Element *elementAl = nist_manager->FindOrBuildElement("Al");
  // materials
  nist_manager->FindOrBuildMaterial("G4_AIR");
  nist_manager->BuildMaterialWithNewDensity("Air_3hPa","G4_AIR", 0.0041*kg/m3);
  //nist_manager->BuildMaterialWithNewDensity("Atmosphere","G4_AIR", 5.655*g/cm3); // Crab average over 13th and 14th
  nist_manager->BuildMaterialWithNewDensity("Atmosphere","G4_AIR", 4.89*g/cm3); // Crab 2013-07-13 12h00
  //nist_manager->BuildMaterialWithNewDensity("Atmosphere","G4_AIR", 6.4*g/cm3); // Cygnus X-1 average over 12th and 13th
  nist_manager->FindOrBuildMaterial("G4_Cu");
  nist_manager->FindOrBuildMaterial("G4_Sn");
  nist_manager->FindOrBuildMaterial("G4_Pb");
  nist_manager->FindOrBuildMaterial("G4_BGO");
  nist_manager->FindOrBuildMaterial("G4_POLYETHYLENE");
  nist_manager->FindOrBuildMaterial("G4_PLASTIC_SC_VINYLTOLUENE");
  nist_manager->FindOrBuildMaterial("G4_GLASS_PLATE");
  nist_manager->FindOrBuildMaterial("G4_POLYVINYL_CHLORIDE");
  nist_manager->FindOrBuildMaterial("G4_MYLAR");					// also use for PEEK wich is similar
  nist_manager->FindOrBuildMaterial("G4_POLYVINYLIDENE_FLUORIDE");	// PVDF close to PVF (Tedlar)
  // Aluminium alloy 6061
  G4Material* Al_6061 = new G4Material("Al_6061", 2.70*g/cm3, 9);
  Al_6061->AddElement(elementSi,  0.6*perCent);  //0.6
  Al_6061->AddElement(elementFe,  0.5*perCent);  //1.1
  Al_6061->AddElement(elementCu,  0.3*perCent);  //1.4
  Al_6061->AddElement(elementMn,  0.1*perCent);  //1.5
  Al_6061->AddElement(elementMg,  1.0*perCent);  //2.5
  Al_6061->AddElement(elementCr,  0.2*perCent);  //2.7
  Al_6061->AddElement(elementZn,  0.2*perCent);  //2.9
  Al_6061->AddElement(elementTi,  0.1*perCent);  //3.0
  Al_6061->AddElement(elementAl, 97.0*perCent);  //100
  // epoxy (for FR4)
  G4Material* epoxy = new G4Material("Epoxy", 1.2*g/cm3, 2);
  epoxy->AddElement(elementH, 2);
  epoxy->AddElement(elementC, 2);
  // FR4 (glass + epoxy)
  G4Material * SiO2 = nist_manager->FindOrBuildMaterial("G4_SILICON_DIOXIDE");
  G4Material* FR4 = new G4Material("FR4", 1.86*g/cm3, 2);
  FR4->AddMaterial(SiO2, 0.528);
  FR4->AddMaterial(epoxy, 0.472);
}

G4VPhysicalVolume* DetectorConstruction::Construct()
{
  // Materials
  G4Material * air_1atm = G4Material::GetMaterial("G4_AIR");
  G4Material * air_3hPa = G4Material::GetMaterial("Air_3hPa");
  G4Material * atmos = G4Material::GetMaterial("Atmosphere");
  G4Material * Al_6061 = G4Material::GetMaterial("Al_6061");
  G4Material * Cu = G4Material::GetMaterial("G4_Cu");
  G4Material * Sn = G4Material::GetMaterial("G4_Sn");
  G4Material * Pb = G4Material::GetMaterial("G4_Pb");
  G4Material * BGO = G4Material::GetMaterial("G4_BGO");
  G4Material * EJ204 = G4Material::GetMaterial("G4_PLASTIC_SC_VINYLTOLUENE");

  //-------------------- Create the World Volume ----------------------------------
  const G4double WorldSize  = 600.0*cm;
  G4Box* s_World = new G4Box("World", WorldSize/2, WorldSize/2, WorldSize/2);
  G4LogicalVolume* l_World = new G4LogicalVolume(s_World, air_3hPa, "World");
  G4VPhysicalVolume* p_World = new G4PVPlacement(0,	G4ThreeVector(), l_World, "World", NULL, false, 0, overlap_check);

  //-------------------- Include atmosphere ---------------------------------------
  const G4String Atmosname = "Atmosphere";
  G4Tubs* s_Atmos = new G4Tubs(Atmosname, 0.0*mm, 200.0*cm, 1./2*cm, 0.*deg, 360.*deg);
  G4LogicalVolume* l_Atmos = new G4LogicalVolume(s_Atmos, atmos, Atmosname);
  // new G4PVPlacement(0, G4ThreeVector(0.0*mm,0.0*mm,200.0*cm), l_Atmos, Atmosname, l_World, false, 0, overlap_check);

  //-------------------- Create POGO mother volume --------------------------------
  G4Box* s_Pogo = new G4Box("Pogo", 95*cm,95*cm,95*cm);
  G4LogicalVolume* l_Pogo = new G4LogicalVolume(s_Pogo, air_3hPa, "Pogo");
  p_Pogo = new G4PVPlacement(0, G4ThreeVector(0.0*cm,0.0*cm,0.0*cm), l_Pogo, "Pogo", l_World, false, 0, overlap_check);

  //-------------------- Create Polarimeter mother volume -------------------------
  G4Tubs* s_Polarimeter = new G4Tubs("Polarimeter", 0.0*cm, 435.0/2.0*mm, 88.45*cm, 0.*deg, 360.*deg);
  G4LogicalVolume* l_Polarimeter = new G4LogicalVolume(s_Polarimeter, air_1atm, "Polarimeter");
  p_Polarimeter = new G4PVPlacement(0, G4ThreeVector(0.0*cm,0.0*cm,0.0*cm), l_Polarimeter, "Polarimeter", l_Pogo, false, 0, overlap_check);

  //-------------------- Now include PVA, RFA, STM, STR and AMU -------------------
  new StructureConstruction(l_Pogo, l_Polarimeter, overlap_check);

  //-------------------- Now construct the polarimeter ----------------------------
  // PDC mother volume
  const G4String PDCName = "PDCUnit";
  const G4double PDCZ[2] = {-22.0*cm,80.0*cm};
  const G4double PDCInner[2] = {0.0*cm,0.0*cm};
  const G4double PDCOuter[2] = {1.499*cm,1.499*cm};
  G4Polyhedra* s_PDC = new G4Polyhedra(PDCName, 0.*deg, 360.*deg, 6, 2, PDCZ, PDCInner, PDCOuter);
  G4LogicalVolume* l_PDC = new G4LogicalVolume(s_PDC, air_1atm, PDCName);

      // passive collimator
      const G4String CollName = "PassiveCollimator";
      const G4double CollZ[2] = {0.0*cm,80.0*cm-FastLength};
      const G4double CollInner[2] = {FastWidth-CollThickness,FastWidth-CollThickness};
      const G4double CollOuter[2] = {FastWidth,FastWidth};
      G4Polyhedra* s_Coll = new G4Polyhedra(CollName, 0.*deg, 360.*deg, 6, 2, CollZ, CollInner, CollOuter);
      G4LogicalVolume* l_Coll = new G4LogicalVolume(s_Coll, Cu, CollName);
      new G4PVPlacement(0, G4ThreeVector(0.0*cm,0.0*cm,FastLength), l_Coll, CollName, l_PDC, false, 0, overlap_check);

      // passive collimator Sn + Pb foil
      const G4String SnCollName = "SnFoil";
      const G4double SnCollZ[2] = {0.0*cm,80.0*cm-FastLength};
      const G4double SnCollInner[2] = {FastWidth,FastWidth};
      const G4double SnCollOuter[2] = {FastWidth+100*um,FastWidth+100*um};
      G4Polyhedra* s_SnColl = new G4Polyhedra(SnCollName, 0.*deg, 360.*deg, 6, 2, SnCollZ, SnCollInner, SnCollOuter);
      G4LogicalVolume* l_SnColl = new G4LogicalVolume(s_SnColl, Sn, SnCollName);
      new G4PVPlacement(0, G4ThreeVector(0.0*cm,0.0*cm,FastLength), l_SnColl, SnCollName, l_PDC, false, 0, overlap_check);
      const G4String PbCollName = "PbFoil";
      const G4double PbCollZ[2] = {0.0*cm,80.0*cm-FastLength};
      const G4double PbCollInner[2] = {FastWidth+100*um,FastWidth+100*um};
      const G4double PbCollOuter[2] = {FastWidth+200*um,FastWidth+200*um};
      G4Polyhedra* s_PbColl = new G4Polyhedra(PbCollName, 0.*deg, 360.*deg, 6, 2, PbCollZ, PbCollInner, PbCollOuter);
      G4LogicalVolume* l_PbColl = new G4LogicalVolume(s_PbColl, Pb, PbCollName);
      new G4PVPlacement(0, G4ThreeVector(0.0*cm,0.0*cm,FastLength), l_PbColl, PbCollName, l_PDC, false, 0, overlap_check);

      // fast scintillator
      const G4String FastName = "FastScintillator";
      const G4double FastZ[2] = {0.0*cm,FastLength};
      const G4double FastInner[2] = {0.0*cm,0.0*cm};
      const G4double FastOuter[2] = {FastWidth,FastWidth};
      G4Polyhedra* s_Fast = new G4Polyhedra(FastName, 0.*deg, 360.*deg, 6, 2, FastZ, FastInner, FastOuter);
      G4LogicalVolume* l_Fast = new G4LogicalVolume(s_Fast, EJ204, FastName);
      new G4PVPlacement(0, G4ThreeVector(0.0*cm,0.0*cm,0.0*cm), l_Fast, FastName, l_PDC, false, 0, overlap_check);

      // bottom BGO
      const G4String BGObottomName = "BGObottom";
      const G4double BGOZ[2] = {0.0*cm,4.0*cm};
      const G4double BGOInner[2] = {0.0*cm,0.0*cm};
      const G4double BGOOuter[2] = {1.425*cm,1.425*cm};
      G4Polyhedra* s1_BGO = new G4Polyhedra("s1_BGO", 0.*deg, 360.*deg, 6, 2, BGOZ, BGOInner, BGOOuter);
      const G4double BGOCylinderZ[4] = {0.0*cm,1.2*cm,2.0*cm,4.0*cm};
      const G4double BGOCylinderInner[4] = {0.0*cm,0.0*cm,0.0*cm,0.0*cm};
      const G4double BGOCylinderOuter[4] = {1.15*cm,1.15*cm,1.612*cm,1.612*cm};
      G4Polycone* s2_BGO = new G4Polycone("s2_BGO", 0.*deg, 360.*deg, 4, BGOCylinderZ, BGOCylinderInner, BGOCylinderOuter);
      G4IntersectionSolid* s_BGObottom = new G4IntersectionSolid(BGObottomName, s1_BGO, s2_BGO);
      G4LogicalVolume* l_BGObottom = new G4LogicalVolume(s_BGObottom, BGO, BGObottomName);
      new G4PVPlacement(0, G4ThreeVector(0.0*cm,0.0*cm,-4.0*cm), l_BGObottom, BGObottomName, l_PDC, false, 0, overlap_check);

      // PM Tube
      const G4String PMName = "PMTube";
      G4Tubs* s_PM = new G4Tubs(PMName, 0.*cm, 1.15*cm, 9.0*cm, 0.*deg, 360.*deg);
      G4LogicalVolume* l_PM = new G4LogicalVolume(s_PM, Al_6061, PMName);
      new G4PVPlacement(0, G4ThreeVector(0.0*cm,0.0*cm,-13.0*cm), l_PM, PMName, l_PDC, false, 0, overlap_check);


  // SAS edge mother volume
  const G4String SASedgeUnitName = "SASedgeUnit";
  G4Box* s1_SASedgeUnit = new G4Box("SASedgeUnit_box", 2.0*cm, 1.465*cm, 39.6*cm);
  G4Tubs* s2_SASedgeUnit = new G4Tubs("SASedgeUnit_cyl", 0.0*cm, 1.691*cm, 39.6*cm, -60.0*deg, 120.0*deg);
  G4UnionSolid* s_SASedgeUnit = new G4UnionSolid(SASedgeUnitName, s1_SASedgeUnit, s2_SASedgeUnit, 0, G4ThreeVector(-2.845*cm,0.0*cm,0.0*cm));
  G4LogicalVolume* l_SASedgeUnit = new G4LogicalVolume(s_SASedgeUnit, air_1atm, SASedgeUnitName);

      // SAS edge back bone
      const G4String SASedgeBoneName = "SASedgeBone";
      G4Box* s_SASedgeBone = new G4Box(SASedgeBoneName, 0.49*cm, 1.465*cm, 30.0*cm);
      G4LogicalVolume* l_SASedgeBone = new G4LogicalVolume(s_SASedgeBone, Al_6061, SASedgeBoneName);
      new G4PVPlacement(0, G4ThreeVector(1.5*cm,0.0*cm,9.6*cm), l_SASedgeBone, SASedgeBoneName, l_SASedgeUnit, false, 0, overlap_check);

      // SAS edge
      const G4String SASedgeName = "SASedge";
      G4Box* s1_SASedge = new G4Box("SASedge_box", 1.5*cm, 1.465*cm, 30.0*cm);
      G4Tubs* s2_SASedge = new G4Tubs("SASedge_cyl", 0.0*cm, 1.691*cm, 30.0*cm, -60.0*deg, 120.0*deg);
      G4UnionSolid* s_SASedge = new G4UnionSolid(SASedgeName, s1_SASedge, s2_SASedge, 0, G4ThreeVector(-2.345*cm,0.0*cm,0.0*cm));
      G4LogicalVolume* l_SASedge = new G4LogicalVolume(s_SASedge, BGO, SASedgeName);
      new G4PVPlacement(0, G4ThreeVector(-0.5*cm,0.0*cm,9.6*cm), l_SASedge, SASedgeName, l_SASedgeUnit, false, 0, overlap_check);

      // SAS bottom edge
      const G4String SASbottomName = "SASbottom";
      G4Tubs* s_SASbottom = new G4Tubs(SASbottomName, 0.0*cm, 1.15*cm, 0.6*cm, 0.0*deg, 360.*deg);
      G4LogicalVolume* l_SASbottom = new G4LogicalVolume(s_SASbottom, BGO, SASbottomName);
      new G4PVPlacement(0, G4ThreeVector(-0.5*cm,0.0*cm,-21.0*cm), l_SASbottom, SASbottomName, l_SASedgeUnit, false, 0, overlap_check);

      // SAS PMT
      new G4PVPlacement(0, G4ThreeVector(-0.5*cm,0.0*cm,-30.6*cm), l_PM, PMName, l_SASedgeUnit, false, 0, overlap_check);

  // SAS corner mother volume
  const G4String SAScornerUnitName = "SAScornerUnit";
  G4Trap* s1_SAScornerUnit = new G4Trap("SAScornerUnit_trap", 39.6*cm, 0.0*cm, 0.0*cm, 1.73*cm, 0.93*cm, 2.93*cm, 0.0*cm, 1.73*cm, 0.93*cm, 2.93*cm, 0.0*cm);
  G4Tubs* s2_SAScornerUnit = new G4Tubs("SAScornerUnit_cyl", 0.0*cm, 3.38*cm, 39.6*cm, 210.0*deg, 120.0*deg);
  G4UnionSolid* s_SAScornerUnit = new G4UnionSolid(SAScornerUnitName, s1_SAScornerUnit, s2_SAScornerUnit, 0, G4ThreeVector(0.0*cm,3.42*cm,0.0*cm));
  G4LogicalVolume* l_SAScornerUnit = new G4LogicalVolume(s_SAScornerUnit, air_1atm, SAScornerUnitName);

      // SAS corner back bone
      const G4String SAScornerBoneName = "SAScornerBone";
      G4Box* s1_SAScornerBone = new G4Box(SAScornerBoneName, 0.49*cm, 1.69*cm, 30.0*cm);
      G4UnionSolid* s_SAScornerBone = new G4UnionSolid(SAScornerBoneName, s1_SAScornerBone, s1_SAScornerBone, zRot60, G4ThreeVector(-1.21858*cm,-2.11065*cm,0.0*cm));
      G4LogicalVolume* l_SAScornerBone = new G4LogicalVolume(s_SAScornerBone, Al_6061, SAScornerBoneName);
      new G4PVPlacement(zRot240, G4ThreeVector(-1.218*cm,2.148*cm,9.6*cm), l_SAScornerBone, SAScornerBoneName, l_SAScornerUnit, false, 0, overlap_check);

      // SAS corner
      const G4String SAScornerName = "SAScorner";
      G4Trap* s1_SAScorner = new G4Trap("SAScorner_trap", 30.0*cm, 0.0*cm, 0.0*cm, 1.3*cm, 0.93*cm, 2.43*cm, 0.0*cm, 1.3*cm, 0.93*cm, 2.43 *cm, 0.0*cm);
      G4Tubs* s2_SAScorner = new G4Tubs("SAScorner_cyl", 0.0*cm, 2.807*cm, 30.0*cm, 210.0*deg, 120.0*deg);
      G4UnionSolid* s_SAScorner = new G4UnionSolid(SAScornerName, s1_SAScorner, s2_SAScorner, 0, G4ThreeVector(0.0*cm,2.705*cm,0.0*cm));
      G4LogicalVolume* l_SAScorner = new G4LogicalVolume(s_SAScorner, BGO, SAScornerName);
      new G4PVPlacement(0, G4ThreeVector(0.0*cm,-0.429*cm,9.6*cm), l_SAScorner, SAScornerName, l_SAScornerUnit, false, 0, overlap_check);

      // SAS bottom corner
      new G4PVPlacement(0, G4ThreeVector(0.0*cm,0.20*cm,-21.0*cm), l_SASbottom, SASbottomName, l_SAScornerUnit, false, 0, overlap_check);

      // PMT corner
      new G4PVPlacement(0, G4ThreeVector(0.0*cm,0.20*cm,-30.6*cm), l_PM, PMName, l_SAScornerUnit, false, 0, overlap_check);

  // place 61 PDC units
  new G4PVPlacement(      0, G4ThreeVector(   0.0000*mm,    0.0*mm,  0.0*mm), l_PDC, PDCName, l_Polarimeter, false,  0, overlap_check);
  new G4PVPlacement( zRot60, G4ThreeVector(   0.0000*mm,   30.0*mm,  0.0*mm), l_PDC, PDCName, l_Polarimeter, false,  1, overlap_check);
  new G4PVPlacement(zRot120, G4ThreeVector(  25.9808*mm,   15.0*mm,  0.0*mm), l_PDC, PDCName, l_Polarimeter, false,  2, overlap_check);
  new G4PVPlacement(zRot180, G4ThreeVector(  25.9808*mm,  -15.0*mm,  0.0*mm), l_PDC, PDCName, l_Polarimeter, false,  3, overlap_check);
  new G4PVPlacement(zRot240, G4ThreeVector(   0.0000*mm,  -30.0*mm,  0.0*mm), l_PDC, PDCName, l_Polarimeter, false,  4, overlap_check);
  new G4PVPlacement(zRot300, G4ThreeVector( -25.9808*mm,  -15.0*mm,  0.0*mm), l_PDC, PDCName, l_Polarimeter, false,  5, overlap_check);
  new G4PVPlacement(      0, G4ThreeVector( -25.9808*mm,   15.0*mm,  0.0*mm), l_PDC, PDCName, l_Polarimeter, false,  6, overlap_check);
  new G4PVPlacement( zRot60, G4ThreeVector(   0.0000*mm,   60.0*mm,  0.0*mm), l_PDC, PDCName, l_Polarimeter, false,  7, overlap_check);
  new G4PVPlacement(zRot120, G4ThreeVector(  25.9808*mm,   45.0*mm,  0.0*mm), l_PDC, PDCName, l_Polarimeter, false,  8, overlap_check);
  new G4PVPlacement(zRot180, G4ThreeVector(  51.9616*mm,   30.0*mm,  0.0*mm), l_PDC, PDCName, l_Polarimeter, false,  9, overlap_check);
  new G4PVPlacement(zRot240, G4ThreeVector(  51.9616*mm,    0.0*mm,  0.0*mm), l_PDC, PDCName, l_Polarimeter, false, 10, overlap_check);
  new G4PVPlacement(zRot300, G4ThreeVector(  51.9616*mm,  -30.0*mm,  0.0*mm), l_PDC, PDCName, l_Polarimeter, false, 11, overlap_check);
  new G4PVPlacement(      0, G4ThreeVector(  25.9808*mm,  -45.0*mm,  0.0*mm), l_PDC, PDCName, l_Polarimeter, false, 12, overlap_check);
  new G4PVPlacement( zRot60, G4ThreeVector(   0.0000*mm,  -60.0*mm,  0.0*mm), l_PDC, PDCName, l_Polarimeter, false, 13, overlap_check);
  new G4PVPlacement(zRot120, G4ThreeVector( -25.9808*mm,  -45.0*mm,  0.0*mm), l_PDC, PDCName, l_Polarimeter, false, 14, overlap_check);
  new G4PVPlacement(zRot180, G4ThreeVector( -51.9616*mm,  -30.0*mm,  0.0*mm), l_PDC, PDCName, l_Polarimeter, false, 15, overlap_check);
  new G4PVPlacement(zRot240, G4ThreeVector( -51.9616*mm,    0.0*mm,  0.0*mm), l_PDC, PDCName, l_Polarimeter, false, 16, overlap_check);
  new G4PVPlacement(zRot300, G4ThreeVector( -51.9616*mm,   30.0*mm,  0.0*mm), l_PDC, PDCName, l_Polarimeter, false, 17, overlap_check);
  new G4PVPlacement(      0, G4ThreeVector( -25.9808*mm,   45.0*mm,  0.0*mm), l_PDC, PDCName, l_Polarimeter, false, 18, overlap_check);
  new G4PVPlacement( zRot60, G4ThreeVector(   0.0000*mm,   90.0*mm,  0.0*mm), l_PDC, PDCName, l_Polarimeter, false, 19, overlap_check);
  new G4PVPlacement(zRot120, G4ThreeVector(  25.9808*mm,   75.0*mm,  0.0*mm), l_PDC, PDCName, l_Polarimeter, false, 20, overlap_check);
  new G4PVPlacement(zRot180, G4ThreeVector(  51.9616*mm,   60.0*mm,  0.0*mm), l_PDC, PDCName, l_Polarimeter, false, 21, overlap_check);
  new G4PVPlacement(zRot240, G4ThreeVector(  77.9423*mm,   45.0*mm,  0.0*mm), l_PDC, PDCName, l_Polarimeter, false, 22, overlap_check);
  new G4PVPlacement(zRot300, G4ThreeVector(  77.9423*mm,   15.0*mm,  0.0*mm), l_PDC, PDCName, l_Polarimeter, false, 23, overlap_check);
  new G4PVPlacement(      0, G4ThreeVector(  77.9423*mm,  -15.0*mm,  0.0*mm), l_PDC, PDCName, l_Polarimeter, false, 24, overlap_check);
  new G4PVPlacement( zRot60, G4ThreeVector(  77.9423*mm,  -45.0*mm,  0.0*mm), l_PDC, PDCName, l_Polarimeter, false, 25, overlap_check);
  new G4PVPlacement(zRot120, G4ThreeVector(  51.9616*mm,  -60.0*mm,  0.0*mm), l_PDC, PDCName, l_Polarimeter, false, 26, overlap_check);
  new G4PVPlacement(zRot180, G4ThreeVector(  25.9808*mm,  -75.0*mm,  0.0*mm), l_PDC, PDCName, l_Polarimeter, false, 27, overlap_check);
  new G4PVPlacement(zRot240, G4ThreeVector(   0.0000*mm,  -90.0*mm,  0.0*mm), l_PDC, PDCName, l_Polarimeter, false, 28, overlap_check);
  new G4PVPlacement(zRot300, G4ThreeVector( -25.9808*mm,  -75.0*mm,  0.0*mm), l_PDC, PDCName, l_Polarimeter, false, 29, overlap_check);
  new G4PVPlacement(      0, G4ThreeVector( -51.9616*mm,  -60.0*mm,  0.0*mm), l_PDC, PDCName, l_Polarimeter, false, 30, overlap_check);
  new G4PVPlacement( zRot60, G4ThreeVector( -77.9423*mm,  -45.0*mm,  0.0*mm), l_PDC, PDCName, l_Polarimeter, false, 31, overlap_check);
  new G4PVPlacement(zRot120, G4ThreeVector( -77.9423*mm,  -15.0*mm,  0.0*mm), l_PDC, PDCName, l_Polarimeter, false, 32, overlap_check);
  new G4PVPlacement(zRot180, G4ThreeVector( -77.9423*mm,   15.0*mm,  0.0*mm), l_PDC, PDCName, l_Polarimeter, false, 33, overlap_check);
  new G4PVPlacement(zRot240, G4ThreeVector( -77.9423*mm,   45.0*mm,  0.0*mm), l_PDC, PDCName, l_Polarimeter, false, 34, overlap_check);
  new G4PVPlacement(zRot300, G4ThreeVector( -51.9616*mm,   60.0*mm,  0.0*mm), l_PDC, PDCName, l_Polarimeter, false, 35, overlap_check);
  new G4PVPlacement(      0, G4ThreeVector( -25.9808*mm,   75.0*mm,  0.0*mm), l_PDC, PDCName, l_Polarimeter, false, 36, overlap_check);
  new G4PVPlacement( zRot60, G4ThreeVector(   0.0000*mm,  120.0*mm,  0.0*mm), l_PDC, PDCName, l_Polarimeter, false, 37, overlap_check);
  new G4PVPlacement(zRot120, G4ThreeVector(  25.9808*mm,  105.0*mm,  0.0*mm), l_PDC, PDCName, l_Polarimeter, false, 38, overlap_check);
  new G4PVPlacement(zRot180, G4ThreeVector(  51.9616*mm,   90.0*mm,  0.0*mm), l_PDC, PDCName, l_Polarimeter, false, 39, overlap_check);
  new G4PVPlacement(zRot240, G4ThreeVector(  77.9423*mm,   75.0*mm,  0.0*mm), l_PDC, PDCName, l_Polarimeter, false, 40, overlap_check);
  new G4PVPlacement(zRot300, G4ThreeVector( 103.9231*mm,   60.0*mm,  0.0*mm), l_PDC, PDCName, l_Polarimeter, false, 41, overlap_check);
  new G4PVPlacement(      0, G4ThreeVector( 103.9231*mm,   30.0*mm,  0.0*mm), l_PDC, PDCName, l_Polarimeter, false, 42, overlap_check);
  new G4PVPlacement( zRot60, G4ThreeVector( 103.9231*mm,    0.0*mm,  0.0*mm), l_PDC, PDCName, l_Polarimeter, false, 43, overlap_check);
  new G4PVPlacement(zRot120, G4ThreeVector( 103.9231*mm,  -30.0*mm,  0.0*mm), l_PDC, PDCName, l_Polarimeter, false, 44, overlap_check);
  new G4PVPlacement(zRot180, G4ThreeVector( 103.9231*mm,  -60.0*mm,  0.0*mm), l_PDC, PDCName, l_Polarimeter, false, 45, overlap_check);
  new G4PVPlacement(zRot240, G4ThreeVector(  77.9423*mm,  -75.0*mm,  0.0*mm), l_PDC, PDCName, l_Polarimeter, false, 46, overlap_check);
  new G4PVPlacement(zRot300, G4ThreeVector(  51.9616*mm,  -90.0*mm,  0.0*mm), l_PDC, PDCName, l_Polarimeter, false, 47, overlap_check);
  new G4PVPlacement(      0, G4ThreeVector(  25.9808*mm, -105.0*mm,  0.0*mm), l_PDC, PDCName, l_Polarimeter, false, 48, overlap_check);
  new G4PVPlacement( zRot60, G4ThreeVector(   0.0000*mm, -120.0*mm,  0.0*mm), l_PDC, PDCName, l_Polarimeter, false, 49, overlap_check);
  new G4PVPlacement(zRot120, G4ThreeVector( -25.9808*mm, -105.0*mm,  0.0*mm), l_PDC, PDCName, l_Polarimeter, false, 50, overlap_check);
  new G4PVPlacement(zRot180, G4ThreeVector( -51.9616*mm,  -90.0*mm,  0.0*mm), l_PDC, PDCName, l_Polarimeter, false, 51, overlap_check);
  new G4PVPlacement(zRot240, G4ThreeVector( -77.9423*mm,  -75.0*mm,  0.0*mm), l_PDC, PDCName, l_Polarimeter, false, 52, overlap_check);
  new G4PVPlacement(zRot300, G4ThreeVector(-103.9231*mm,  -60.0*mm,  0.0*mm), l_PDC, PDCName, l_Polarimeter, false, 53, overlap_check);
  new G4PVPlacement(      0, G4ThreeVector(-103.9231*mm,  -30.0*mm,  0.0*mm), l_PDC, PDCName, l_Polarimeter, false, 54, overlap_check);
  new G4PVPlacement( zRot60, G4ThreeVector(-103.9231*mm,    0.0*mm,  0.0*mm), l_PDC, PDCName, l_Polarimeter, false, 55, overlap_check);
  new G4PVPlacement(zRot120, G4ThreeVector(-103.9231*mm,   30.0*mm,  0.0*mm), l_PDC, PDCName, l_Polarimeter, false, 56, overlap_check);
  new G4PVPlacement(zRot180, G4ThreeVector(-103.9231*mm,   60.0*mm,  0.0*mm), l_PDC, PDCName, l_Polarimeter, false, 57, overlap_check);
  new G4PVPlacement(zRot240, G4ThreeVector( -77.9423*mm,   75.0*mm,  0.0*mm), l_PDC, PDCName, l_Polarimeter, false, 58, overlap_check);
  new G4PVPlacement(zRot300, G4ThreeVector( -51.9616*mm,   90.0*mm,  0.0*mm), l_PDC, PDCName, l_Polarimeter, false, 59, overlap_check);
  new G4PVPlacement(      0, G4ThreeVector( -25.9808*mm,  105.0*mm,  0.0*mm), l_PDC, PDCName, l_Polarimeter, false, 60, overlap_check);
  // place 30 SAS units
  new G4PVPlacement(      0, G4ThreeVector(       0*mm,  154.33*mm,159.5*mm), l_SAScornerUnit, SAScornerUnitName, l_Polarimeter, false,  0, overlap_check);
  new G4PVPlacement(zRot300, G4ThreeVector( 32.7054*mm, 146.647*mm,159.5*mm),   l_SASedgeUnit,   SASedgeUnitName, l_Polarimeter, false,  1, overlap_check);
  new G4PVPlacement(zRot300, G4ThreeVector( 58.6861*mm, 131.647*mm,159.5*mm),   l_SASedgeUnit,   SASedgeUnitName, l_Polarimeter, false,  2, overlap_check);
  new G4PVPlacement(zRot300, G4ThreeVector( 84.6669*mm, 116.647*mm,159.5*mm),   l_SASedgeUnit,   SASedgeUnitName, l_Polarimeter, false,  3, overlap_check);
  new G4PVPlacement(zRot300, G4ThreeVector( 110.648*mm, 101.647*mm,159.5*mm),   l_SASedgeUnit,   SASedgeUnitName, l_Polarimeter, false,  4, overlap_check);
  new G4PVPlacement( zRot60, G4ThreeVector( 133.654*mm,  77.165*mm,159.5*mm), l_SAScornerUnit, SAScornerUnitName, l_Polarimeter, false,  5, overlap_check);
  new G4PVPlacement(      0, G4ThreeVector( 143.353*mm,    45.0*mm,159.5*mm),   l_SASedgeUnit,   SASedgeUnitName, l_Polarimeter, false,  6, overlap_check);
  new G4PVPlacement(      0, G4ThreeVector( 143.353*mm,    15.0*mm,159.5*mm),   l_SASedgeUnit,   SASedgeUnitName, l_Polarimeter, false,  7, overlap_check);
  new G4PVPlacement(      0, G4ThreeVector( 143.353*mm,   -15.0*mm,159.5*mm),   l_SASedgeUnit,   SASedgeUnitName, l_Polarimeter, false,  8, overlap_check);
  new G4PVPlacement(      0, G4ThreeVector( 143.353*mm,   -45.0*mm,159.5*mm),   l_SASedgeUnit,   SASedgeUnitName, l_Polarimeter, false,  9, overlap_check);
  new G4PVPlacement(zRot120, G4ThreeVector( 133.654*mm, -77.165*mm,159.5*mm), l_SAScornerUnit, SAScornerUnitName, l_Polarimeter, false, 10, overlap_check);
  new G4PVPlacement( zRot60, G4ThreeVector( 110.648*mm,-101.647*mm,159.5*mm),   l_SASedgeUnit,   SASedgeUnitName, l_Polarimeter, false, 11, overlap_check);
  new G4PVPlacement( zRot60, G4ThreeVector( 84.6669*mm,-116.647*mm,159.5*mm),   l_SASedgeUnit,   SASedgeUnitName, l_Polarimeter, false, 12, overlap_check);
  new G4PVPlacement( zRot60, G4ThreeVector( 58.6861*mm,-131.647*mm,159.5*mm),   l_SASedgeUnit,   SASedgeUnitName, l_Polarimeter, false, 13, overlap_check);
  new G4PVPlacement( zRot60, G4ThreeVector( 32.7054*mm,-146.647*mm,159.5*mm),   l_SASedgeUnit,   SASedgeUnitName, l_Polarimeter, false, 14, overlap_check);
  new G4PVPlacement(zRot180, G4ThreeVector(       0*mm, -154.33*mm,159.5*mm), l_SAScornerUnit, SAScornerUnitName, l_Polarimeter, false, 15, overlap_check);
  new G4PVPlacement(zRot120, G4ThreeVector(-32.7054*mm,-146.647*mm,159.5*mm),   l_SASedgeUnit,   SASedgeUnitName, l_Polarimeter, false, 16, overlap_check);
  new G4PVPlacement(zRot120, G4ThreeVector(-58.6861*mm,-131.647*mm,159.5*mm),   l_SASedgeUnit,   SASedgeUnitName, l_Polarimeter, false, 17, overlap_check);
  new G4PVPlacement(zRot120, G4ThreeVector(-84.6669*mm,-116.647*mm,159.5*mm),   l_SASedgeUnit,   SASedgeUnitName, l_Polarimeter, false, 18, overlap_check);
  new G4PVPlacement(zRot120, G4ThreeVector(-110.648*mm,-101.647*mm,159.5*mm),   l_SASedgeUnit,   SASedgeUnitName, l_Polarimeter, false, 19, overlap_check);
  new G4PVPlacement(zRot240, G4ThreeVector(-133.654*mm, -77.165*mm,159.5*mm), l_SAScornerUnit, SAScornerUnitName, l_Polarimeter, false, 20, overlap_check);
  new G4PVPlacement(zRot180, G4ThreeVector(-143.353*mm,   -45.0*mm,159.5*mm),   l_SASedgeUnit,   SASedgeUnitName, l_Polarimeter, false, 21, overlap_check);
  new G4PVPlacement(zRot180, G4ThreeVector(-143.353*mm,   -15.0*mm,159.5*mm),   l_SASedgeUnit,   SASedgeUnitName, l_Polarimeter, false, 22, overlap_check);
  new G4PVPlacement(zRot180, G4ThreeVector(-143.353*mm,    15.0*mm,159.5*mm),   l_SASedgeUnit,   SASedgeUnitName, l_Polarimeter, false, 23, overlap_check);
  new G4PVPlacement(zRot180, G4ThreeVector(-143.353*mm,    45.0*mm,159.5*mm),   l_SASedgeUnit,   SASedgeUnitName, l_Polarimeter, false, 24, overlap_check);
  new G4PVPlacement(zRot300, G4ThreeVector(-133.654*mm,  77.165*mm,159.5*mm), l_SAScornerUnit, SAScornerUnitName, l_Polarimeter, false, 25, overlap_check);
  new G4PVPlacement(zRot240, G4ThreeVector(-110.648*mm, 101.647*mm,159.5*mm),   l_SASedgeUnit,   SASedgeUnitName, l_Polarimeter, false, 26, overlap_check);
  new G4PVPlacement(zRot240, G4ThreeVector(-84.6669*mm, 116.647*mm,159.5*mm),   l_SASedgeUnit,   SASedgeUnitName, l_Polarimeter, false, 27, overlap_check);
  new G4PVPlacement(zRot240, G4ThreeVector(-58.6861*mm, 131.647*mm,159.5*mm),   l_SASedgeUnit,   SASedgeUnitName, l_Polarimeter, false, 28, overlap_check);
  new G4PVPlacement(zRot240, G4ThreeVector(-32.7054*mm, 146.647*mm,159.5*mm),   l_SASedgeUnit,   SASedgeUnitName, l_Polarimeter, false, 29, overlap_check);


  //-------------------- Now define the sensitive detectors ----------------------------
  G4SDManager* SDman = G4SDManager::GetSDMpointer();

    slowSD = (TrackerSD*)SDman->FindSensitiveDetector("slowSD", false);
    if (slowSD) delete slowSD;
      slowSD = new TrackerSD("slowSD");
      SDman->AddNewDetector(slowSD);
      //l_Slow->SetSensitiveDetector(slowSD);

    fastSD = (TrackerSD*)SDman->FindSensitiveDetector("fastSD", false);
    if (fastSD) delete fastSD;
      fastSD = new TrackerSD("fastSD");
      SDman->AddNewDetector(fastSD);
      l_Fast->SetSensitiveDetector(fastSD);

    BGObottomSD = (TrackerSD*)SDman->FindSensitiveDetector("bottomBGOSD", false);
    if (BGObottomSD) delete BGObottomSD;
      BGObottomSD = new TrackerSD("bottomBGOSD");
      SDman->AddNewDetector(BGObottomSD);
      l_BGObottom->SetSensitiveDetector(BGObottomSD);

    SASSD = (TrackerSD*)SDman->FindSensitiveDetector("SASSD", false);
    if (SASSD) delete SASSD;
      SASSD = new TrackerSD("SASSD");
      SDman->AddNewDetector(SASSD);
      l_SASedge->SetSensitiveDetector(SASSD);
      l_SAScorner->SetSensitiveDetector(SASSD);
      l_SASbottom->SetSensitiveDetector(SASSD);

  //-------------------- Now define the visualization ----------------------------
  G4VisAttributes* colourWhite = new G4VisAttributes(G4Colour(1.0,1.0,1.0));
  G4VisAttributes* colourLightBlue = new G4VisAttributes(G4Colour(0.6,0.6,1.0));
  G4VisAttributes* colourGrey = new G4VisAttributes(G4Colour(0.8,0.8,0.8));
  G4VisAttributes* colourRed = new G4VisAttributes(G4Colour(1.0,0.4,0.4));
  G4VisAttributes* colourCopper = new G4VisAttributes(G4Colour(0.85, 0.54, 0.4));
  colourWhite->SetForceSolid(true);
  colourLightBlue->SetForceSolid(true);
  colourGrey->SetForceSolid(true);
  colourRed->SetForceSolid(true);
  colourCopper->SetForceSolid(true);

  l_Coll->SetVisAttributes(colourCopper);
  l_SnColl->SetVisAttributes(colourWhite);
  l_PbColl->SetVisAttributes(colourGrey);
  l_Fast->SetVisAttributes(colourLightBlue);
  l_BGObottom->SetVisAttributes(colourRed);
  l_PM->SetVisAttributes(colourWhite);
  l_SASedge->SetVisAttributes(colourRed);
  l_SASedgeBone->SetVisAttributes(colourGrey);
  l_SAScorner->SetVisAttributes(colourRed);
  l_SAScornerBone->SetVisAttributes(colourGrey);
  l_SASbottom->SetVisAttributes(colourRed);

  l_World->SetVisAttributes(G4VisAttributes::Invisible);
  l_Pogo->SetVisAttributes(G4VisAttributes::Invisible);
  l_Polarimeter->SetVisAttributes(G4VisAttributes::Invisible);
  l_PDC->SetVisAttributes(G4VisAttributes::Invisible);
  l_SASedgeUnit->SetVisAttributes(G4VisAttributes::Invisible);
  l_SAScornerUnit->SetVisAttributes(G4VisAttributes::Invisible);

  return p_World;
}

void DetectorConstruction::SetFastLength(G4double length)
{
  if (length <= 80.0*cm) {
    FastLength = length;
  } else {
    G4cout << "  Length of the fast scintillator too long: " << length/cm << " cm" << ", please chose a length < or = 80 cm" << G4endl;
    return ;
  }
  // clean-up previous geometry
  G4GeometryManager::GetInstance()->OpenGeometry();
  G4PhysicalVolumeStore::GetInstance()->Clean();
  G4LogicalVolumeStore::GetInstance()->Clean();
  G4SolidStore::GetInstance()->Clean();
  // define new one
  G4RunManager::GetRunManager()->DefineWorldVolume(Construct());
  G4cout << "  Length of the fast scintillator set to " << length/cm << " cm" << G4endl;
}

void DetectorConstruction::SetFastWidth(G4double width)
{
  if (width <= 1.425*cm) {
    FastWidth = width;
  } else {
    G4cout << "  Width of the fast scintillator too large: " << width/cm << " cm" << ", please chose a width < or = 1.425 cm" << G4endl;
    return ;
  }
  // clean-up previous geometry
  G4GeometryManager::GetInstance()->OpenGeometry();
  G4PhysicalVolumeStore::GetInstance()->Clean();
  G4LogicalVolumeStore::GetInstance()->Clean();
  G4SolidStore::GetInstance()->Clean();
  // define new one
  G4RunManager::GetRunManager()->DefineWorldVolume(Construct());
  G4cout << "  Width of the fast scintillator set to " << width/cm << " cm" << G4endl;
}

void DetectorConstruction::SetCollThickness(G4double thickness)
{
  if (thickness <= 1.425*cm) {
    CollThickness = thickness;
  } else {
    G4cout << "  Thickness of the passive collimator too large: " << thickness/cm << " cm" << ", please chose a thickness < or = 1.425 cm" << G4endl;
    return ;
  }
  // clean-up previous geometry
  G4GeometryManager::GetInstance()->OpenGeometry();
  G4PhysicalVolumeStore::GetInstance()->Clean();
  G4LogicalVolumeStore::GetInstance()->Clean();
  G4SolidStore::GetInstance()->Clean();
  // define new one
  G4RunManager::GetRunManager()->DefineWorldVolume(Construct());
  G4cout << "  Thickness of the passive collimator set to " << thickness/cm << " cm" << G4endl;
}

void DetectorConstruction::SetPolarimeterRot(G4double phi)
{
  PolarimeterRot = phi;
  *zRot=*Rot0;
  zRot->rotateZ(PolarimeterRot);
  // rotate the mother volume Polarimeter to rotate all the detectors
  p_Polarimeter->SetRotation(zRot);
  // update the geometry
  G4RunManager::GetRunManager()->GeometryHasBeenModified();
  G4cout << "  Rotation of the polarimeter set to " << phi/deg << " deg" << G4endl;
  // Notify the VisManager as well
  G4VVisManager* pVVisManager = G4VVisManager::GetConcreteInstance();
  if(pVVisManager) pVVisManager->GeometryHasChanged();
}

void DetectorConstruction::SetElevation(G4double delta)
{
  if (delta < 70*deg) {
    Elevation = delta;
  } else {
    G4cout << "  Elevation too high: " << delta/deg << " degrees" << ", please chose elevation < 70 degrees" << G4endl;
    return ;
  }
  *xRot=*Rot0;
  xRot->rotateX(Elevation);
  // rotate the mother volume Pogo to rotate all the instrument
  p_Pogo->SetRotation(xRot);
  // update the geometry
  G4RunManager::GetRunManager()->GeometryHasBeenModified();
  G4cout << "  Elevation set to " << delta/deg << " deg" << G4endl;
  // Notify the VisManager as well
  G4VVisManager* pVVisManager = G4VVisManager::GetConcreteInstance();
  if(pVVisManager) pVVisManager->GeometryHasChanged();
}
