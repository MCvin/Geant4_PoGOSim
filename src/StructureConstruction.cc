//
//  StructureConstruction.cc 2014-12-09  Maxime Chauvin
//
//  Includes the PVA, RFA, Neutron shield, STM, STR and AMU
//  based on Merlin Kole 2014-7 updated by Maxime Chauvin 2014-12-09
//  Modification from PoGOLite to PoGO+, Maxime Chauvin 2016-03-16
//

#include "StructureConstruction.hh"

#include "G4Material.hh"
#include "G4NistManager.hh"

#include "G4ThreeVector.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4PVPlacement.hh"
#include "G4VisAttributes.hh"

StructureConstruction::StructureConstruction(G4LogicalVolume* l_Pogo, G4LogicalVolume* l_Polarimeter, G4bool overlap_check)
{
  // define some rotation matrices
  DefineRotations();

  // materials
  G4Material * air_3hPa = G4Material::GetMaterial("Air_3hPa");
  G4Material * Al_6061 = G4Material::GetMaterial("Al_6061");
  G4Material * PE = G4Material::GetMaterial("G4_POLYETHYLENE");
  G4Material * Glass = G4Material::GetMaterial("G4_GLASS_PLATE");
  G4Material * PVC = G4Material::GetMaterial("G4_POLYVINYL_CHLORIDE");
  G4Material * Mylar = G4Material::GetMaterial("G4_MYLAR");
  G4Material * PVDF = G4Material::GetMaterial("G4_POLYVINYLIDENE_FLUORIDE");
  G4Material * FR4 = G4Material::GetMaterial("FR4");

  ////////////////////////////////// Top pressure vessel //////////////////////////////////////////

  //// Top Pressure Vessel
  const G4String TopPVname = "Top_PV";
  G4Tubs* s_Top_PV = new G4Tubs(TopPVname, 427.0/2.0*mm, 433.0/2.0*mm, 868/2.0*mm, 0.*deg, 360.*deg);
  G4LogicalVolume* l_Top_PV = new G4LogicalVolume(s_Top_PV, Al_6061, TopPVname);
  new G4PVPlacement(0, G4ThreeVector(0.0*cm,0.0*cm,384.5*mm), l_Top_PV, TopPVname, l_Polarimeter, false, 0, overlap_check);

  //// Top Plate
  const G4String TopPlatename = "Top_Plate";
  G4Tubs* s_Top_plate = new G4Tubs(TopPlatename, 280.0/2.0*mm, 427.0/2.0*mm, 25.0/2.0*mm, 0.*deg, 360.*deg);
  G4LogicalVolume* l_Top_plate = new G4LogicalVolume(s_Top_plate, Al_6061, TopPlatename);
  new G4PVPlacement(0, G4ThreeVector(0.0*cm,0.0*cm,806.0*mm), l_Top_plate, TopPlatename, l_Polarimeter, false, 0, overlap_check);

  //// Mylar + Tedlar + PEEK window
  G4Tubs* s_Mylar = new G4Tubs("Mylar_Window", 0.0*mm, 320.0/2.0*mm, 0.2/2.0*mm, 0.*deg, 360.*deg);
  G4Tubs* s_Tedlar = new G4Tubs("Tedlar_Window", 0.0*mm, 320.0/2.0*mm, 0.12/2.0*mm, 0.*deg, 360.*deg);
  G4Tubs* s_PEEK = new G4Tubs("PEEK_Window", 0.0*mm, 320.0/2.0*mm, 0.19/2.0*mm, 0.*deg, 360.*deg);
  G4LogicalVolume* l_Mylar = new G4LogicalVolume(s_Mylar, Mylar, "Mylar_Window");
  G4LogicalVolume* l_Tedlar = new G4LogicalVolume(s_Tedlar, PVDF, "Tedlar_Window");
  G4LogicalVolume* l_PEEK = new G4LogicalVolume(s_PEEK, Mylar, "PEEK_Window");
  new G4PVPlacement(0, G4ThreeVector(0.0*cm,0.0*cm,818.910*mm),  l_Mylar,  "Mylar_Window", l_Polarimeter, false, 0, overlap_check);
  new G4PVPlacement(0, G4ThreeVector(0.0*cm,0.0*cm,818.750*mm), l_Tedlar, "Tedlar_Window", l_Polarimeter, false, 0, overlap_check);
  new G4PVPlacement(0, G4ThreeVector(0.0*cm,0.0*cm,818.595*mm),   l_PEEK,   "PEEK_Window", l_Polarimeter, false, 0, overlap_check);

  //// Base Plate
  const G4String BasePlatename = "Base_Plate";
  G4Tubs* s_Base_plate = new G4Tubs(BasePlatename, 201.0*mm, 427.0/2.0*mm, 20.0/2.0*mm, 0.*deg, 360.*deg);
  G4LogicalVolume* l_Base_plate = new G4LogicalVolume(s_Base_plate, Al_6061, BasePlatename);
  new G4PVPlacement(0, G4ThreeVector(0.0*cm,0.0*cm,-39.5*mm), l_Base_plate, BasePlatename, l_Polarimeter, false, 0, overlap_check);

  ////////////////////////////////// PMT pressure vessel //////////////////////////////////////////

  //// PMT PV
  const G4String PMT_PV_name = "PMT_PV";
  G4Tubs* s_PMT_PV = new G4Tubs(PMT_PV_name, 191*mm, 201.0*mm, 400.0/2*mm, 0.*deg, 360.*deg);
  G4LogicalVolume* l_PMT_PV = new G4LogicalVolume(s_PMT_PV, Al_6061, PMT_PV_name);
  new G4PVPlacement(0, G4ThreeVector(0.0*cm,0.0*cm,-249.5*mm), l_PMT_PV, PMT_PV_name, l_Polarimeter, false, 0, overlap_check);

  //// Cold plate
  const G4String Cold_plate_name = "Cold_plate";
  G4Tubs* s_Cold_plate = new G4Tubs(Cold_plate_name, 0.0*mm, 190.0*mm, 10.0/2*mm, 0.*deg, 360.*deg);
  G4LogicalVolume* l_Cold_plate = new G4LogicalVolume(s_Cold_plate, Al_6061, Cold_plate_name);
  new G4PVPlacement(0, G4ThreeVector(0.0*cm,0.0*cm,-320.0*mm), l_Cold_plate, Cold_plate_name, l_Polarimeter, false, 0, overlap_check);

  //// Bottom Poly shield
  const G4String n_Bottom_poly = "Bottom poly block";
  G4Tubs* s_Bottom_poly = new G4Tubs(n_Bottom_poly, 0.0*mm, 190.0*mm, 75.0*mm, 0.*deg, 360.*deg);
  G4LogicalVolume* l_Bottom_poly = new G4LogicalVolume(s_Bottom_poly, PE, n_Bottom_poly);
  new G4PVPlacement(0, G4ThreeVector(0.0*cm,0.0*cm,-450.0*mm), l_Bottom_poly, n_Bottom_poly, l_Polarimeter, false, 0, overlap_check);

  ////////////////////////////////// Electronics pressure vessel //////////////////////////////////////////

  //// El_PV
  const G4String ElPVname = "El_PV";
  G4Tubs* s_El_PV = new G4Tubs(ElPVname, 191.0*mm, 201.0*mm, 400.0/2.0*mm, 0.*deg, 360.*deg);
  G4LogicalVolume* l_El_PV = new G4LogicalVolume(s_El_PV, Al_6061, ElPVname);
  new G4PVPlacement(0, G4ThreeVector(0.*cm,0.*cm,-649.5*mm), l_El_PV, ElPVname, l_Polarimeter, false, 0, overlap_check);

  //// El_bottom
  const G4String Elbottomname = "El_Bottom";
  G4Tubs* s_El_bottom = new G4Tubs(Elbottomname, 0.0*mm, 435.0/2.0*mm, 17.5*mm, 0.*deg, 360.*deg);
  G4LogicalVolume* l_El_bottom = new G4LogicalVolume(s_El_bottom, Al_6061, Elbottomname);
  new G4PVPlacement(0, G4ThreeVector(0.*cm,0.*cm,-867.0*mm), l_El_bottom, Elbottomname, l_Polarimeter, false, 0, overlap_check);

  //// FADC boards
  const G4String FADCname = "FADC boards";
  G4Box* s_FADC = new G4Box("FADCname", 130./2.0*mm, 15./2.0*mm, 170./2.0*mm);
  G4LogicalVolume* l_FADC = new G4LogicalVolume(s_FADC, FR4, FADCname);
  // place 11 copies of FADC board (6 FADC + DIO + IUB + 1 SpWrouter + 1 minion + 1 powerbrd)
  new G4PVPlacement(     0, G4ThreeVector(-20.0*mm,  10.0*mm,-700.0*mm), l_FADC, FADCname, l_Polarimeter, false,  0, overlap_check);
  new G4PVPlacement(     0, G4ThreeVector(-20.0*mm,  30.0*mm,-700.0*mm), l_FADC, FADCname, l_Polarimeter, false,  1, overlap_check);
  new G4PVPlacement(     0, G4ThreeVector(-20.0*mm,  50.0*mm,-700.0*mm), l_FADC, FADCname, l_Polarimeter, false,  2, overlap_check);
  new G4PVPlacement(     0, G4ThreeVector(-20.0*mm,  70.0*mm,-700.0*mm), l_FADC, FADCname, l_Polarimeter, false,  3, overlap_check);
  new G4PVPlacement(     0, G4ThreeVector(-20.0*mm,  90.0*mm,-700.0*mm), l_FADC, FADCname, l_Polarimeter, false,  4, overlap_check);
  new G4PVPlacement(     0, G4ThreeVector(-20.0*mm, 110.0*mm,-700.0*mm), l_FADC, FADCname, l_Polarimeter, false,  5, overlap_check);
  new G4PVPlacement(     0, G4ThreeVector(-20.0*mm, -10.0*mm,-700.0*mm), l_FADC, FADCname, l_Polarimeter, false,  6, overlap_check);
  new G4PVPlacement(     0, G4ThreeVector(-20.0*mm, -30.0*mm,-700.0*mm), l_FADC, FADCname, l_Polarimeter, false,  7, overlap_check);
  new G4PVPlacement(     0, G4ThreeVector(-20.0*mm, -50.0*mm,-700.0*mm), l_FADC, FADCname, l_Polarimeter, false,  8, overlap_check);
  new G4PVPlacement(     0, G4ThreeVector(-20.0*mm, -70.0*mm,-700.0*mm), l_FADC, FADCname, l_Polarimeter, false,  9, overlap_check);
  new G4PVPlacement(     0, G4ThreeVector(-20.0*mm, -90.0*mm,-700.0*mm), l_FADC, FADCname, l_Polarimeter, false, 10, overlap_check);

  //// FIXME: Add other stuff here, the electronic is closely packed stuff
  //// Add crate made of Copper and Aluminum

  ////////////////////////////////////////// RFA and neutron shield /////////////////////////////////

  //// RFA mother volume
  const G4String RFAmom = "RFA_mother";
  G4Tubs* s_RFA_mom = new G4Tubs(RFAmom, 233.5*mm, 400.0*mm, 1262.0/2.0*mm, 0.*deg, 360.*deg);
  G4LogicalVolume* l_RFA_mom = new G4LogicalVolume(s_RFA_mom, air_3hPa, RFAmom);
  new G4PVPlacement(0, G4ThreeVector(0.0*cm,0.0*cm,20.0*cm), l_RFA_mom, RFAmom, l_Pogo, false, 0, overlap_check);

  //// RFA
  const G4String RFAname = "RFA";
  G4Tubs* s_RFA = new G4Tubs(RFAname, 233.5*mm, 238.5*mm, 1262.0/2.0*mm, 0.*deg, 360.*deg);
  G4LogicalVolume* l_RFA = new G4LogicalVolume(s_RFA, Al_6061, RFAname);
  new G4PVPlacement(0, G4ThreeVector(0.0*cm,0.0*cm,0.0*cm), l_RFA, RFAname, l_RFA_mom, false, 0, overlap_check);

  //// Thick Flange
  const G4String Flange_name1 = "Flange_thick";
  G4Tubs* s_Flange1 = new G4Tubs(Flange_name1, 238.5*mm, 338.5*mm, 7.5*mm, 0.*deg, 360.*deg);
  G4LogicalVolume* l_Flange1 = new G4LogicalVolume(s_Flange1, Al_6061, Flange_name1);
  new G4PVPlacement(0, G4ThreeVector(0.0*cm,0.0*cm, 623.5*mm), l_Flange1, Flange_name1, l_RFA_mom, false, 0, overlap_check);
  new G4PVPlacement(0, G4ThreeVector(0.0*cm,0.0*cm,-623.5*mm), l_Flange1, Flange_name1, l_RFA_mom, false, 1, overlap_check);

  //// Thin Flanges
  const G4String Flange_name2 = "Flange_thin";
  G4Tubs* s_Flange2 = new G4Tubs(Flange_name2, 238.5*mm, 338.5*mm, 2.5*mm, 0.*deg, 360.*deg);
  G4LogicalVolume* l_Flange2 = new G4LogicalVolume(s_Flange2, Al_6061, Flange_name2);
  new G4PVPlacement(0, G4ThreeVector(0.0*cm,0.0*cm, 318.5*mm), l_Flange2, Flange_name2, l_RFA_mom, false, 0, overlap_check);
  new G4PVPlacement(0, G4ThreeVector(0.0*cm,0.0*cm,  18.5*mm), l_Flange2, Flange_name2, l_RFA_mom, false, 1, overlap_check);
  new G4PVPlacement(0, G4ThreeVector(0.0*cm,0.0*cm,-136.5*mm), l_Flange2, Flange_name2, l_RFA_mom, false, 2, overlap_check);
  new G4PVPlacement(0, G4ThreeVector(0.0*cm,0.0*cm,-377.5*mm), l_Flange2, Flange_name2, l_RFA_mom, false, 3, overlap_check);

  //// Poly blocks
  const G4String Poly10_name0 = "Poly_block10_0";
  G4Tubs* s_Poly10_0 = new G4Tubs(Poly10_name0, 238.5*mm, 288.5*mm, 100.0*mm, 0.5*deg, 44.0*deg);
  //G4Tubs* s_Poly10_0 = new G4Tubs(Poly10_name0, 238.5*mm, 338.5*mm, 147.5*mm, 0.5*deg, 44.0*deg);
  //G4Tubs* s_Poly10_0 = new G4Tubs(Poly10_name0, 238.5*mm, 388.5*mm, 147.5*mm, 0.5*deg, 44.0*deg);
  G4LogicalVolume* l_Poly10_0 = new G4LogicalVolume(s_Poly10_0, PE, Poly10_name0);
  new G4PVPlacement(      0, G4ThreeVector(0.0*cm,0.0*cm,421.0*mm), l_Poly10_0, Poly10_name0, l_RFA_mom, false, 0, overlap_check);
  new G4PVPlacement( zRot45, G4ThreeVector(0.0*cm,0.0*cm,421.0*mm), l_Poly10_0, Poly10_name0, l_RFA_mom, false, 1, overlap_check);
  new G4PVPlacement( zRot90, G4ThreeVector(0.0*cm,0.0*cm,421.0*mm), l_Poly10_0, Poly10_name0, l_RFA_mom, false, 2, overlap_check);
  new G4PVPlacement(zRot135, G4ThreeVector(0.0*cm,0.0*cm,421.0*mm), l_Poly10_0, Poly10_name0, l_RFA_mom, false, 3, overlap_check);
  new G4PVPlacement(zRot180, G4ThreeVector(0.0*cm,0.0*cm,421.0*mm), l_Poly10_0, Poly10_name0, l_RFA_mom, false, 4, overlap_check);
  new G4PVPlacement(zRot225, G4ThreeVector(0.0*cm,0.0*cm,421.0*mm), l_Poly10_0, Poly10_name0, l_RFA_mom, false, 5, overlap_check);
  new G4PVPlacement(zRot270, G4ThreeVector(0.0*cm,0.0*cm,421.0*mm), l_Poly10_0, Poly10_name0, l_RFA_mom, false, 6, overlap_check);
  new G4PVPlacement(zRot315, G4ThreeVector(0.0*cm,0.0*cm,421.0*mm), l_Poly10_0, Poly10_name0, l_RFA_mom, false, 7, overlap_check);

  const G4String Poly10_name1 = "Poly_block10_1";
  G4Tubs* s_Poly10_1 = new G4Tubs(Poly10_name1, 238.5*mm, 338.5*mm, 117.5*mm, 0.5*deg, 44.0*deg);
  //G4Tubs* s_Poly10_1 = new G4Tubs(Poly10_name1, 238.5*mm, 388.5*mm, 117.5*mm, 0.5*deg, 44.0*deg);
  G4LogicalVolume* l_Poly10_1 = new G4LogicalVolume(s_Poly10_1, PE, Poly10_name1);
  new G4PVPlacement(      0, G4ThreeVector(0.0*cm,0.0*cm,198.5*mm), l_Poly10_1, Poly10_name1, l_RFA_mom, false, 0, overlap_check);
  new G4PVPlacement( zRot45, G4ThreeVector(0.0*cm,0.0*cm,198.5*mm), l_Poly10_1, Poly10_name1, l_RFA_mom, false, 1, overlap_check);
  new G4PVPlacement( zRot90, G4ThreeVector(0.0*cm,0.0*cm,198.5*mm), l_Poly10_1, Poly10_name1, l_RFA_mom, false, 2, overlap_check);
  new G4PVPlacement(zRot135, G4ThreeVector(0.0*cm,0.0*cm,198.5*mm), l_Poly10_1, Poly10_name1, l_RFA_mom, false, 3, overlap_check);
  new G4PVPlacement(zRot180, G4ThreeVector(0.0*cm,0.0*cm,198.5*mm), l_Poly10_1, Poly10_name1, l_RFA_mom, false, 4, overlap_check);
  new G4PVPlacement(zRot225, G4ThreeVector(0.0*cm,0.0*cm,198.5*mm), l_Poly10_1, Poly10_name1, l_RFA_mom, false, 5, overlap_check);
  new G4PVPlacement(zRot270, G4ThreeVector(0.0*cm,0.0*cm,198.5*mm), l_Poly10_1, Poly10_name1, l_RFA_mom, false, 6, overlap_check);
  new G4PVPlacement(zRot315, G4ThreeVector(0.0*cm,0.0*cm,198.5*mm), l_Poly10_1, Poly10_name1, l_RFA_mom, false, 7, overlap_check);

  const G4String Poly15_name1 = "Poly_block15_1";
  G4Tubs* s_Poly15_1 = new G4Tubs(Poly15_name1, 238.5*mm, 388.5*mm, 30.0*mm, 0.5*deg, 44.0*deg);
  G4LogicalVolume* l_Poly15_1 = new G4LogicalVolume(s_Poly15_1, PE, Poly15_name1);
  new G4PVPlacement(      0, G4ThreeVector(0.0*cm,0.0*cm,51.0*mm), l_Poly15_1, Poly15_name1, l_RFA_mom, false, 0, overlap_check);
  new G4PVPlacement( zRot45, G4ThreeVector(0.0*cm,0.0*cm,51.0*mm), l_Poly15_1, Poly15_name1, l_RFA_mom, false, 1, overlap_check);
  new G4PVPlacement( zRot90, G4ThreeVector(0.0*cm,0.0*cm,51.0*mm), l_Poly15_1, Poly15_name1, l_RFA_mom, false, 2, overlap_check);
  new G4PVPlacement(zRot135, G4ThreeVector(0.0*cm,0.0*cm,51.0*mm), l_Poly15_1, Poly15_name1, l_RFA_mom, false, 3, overlap_check);
  new G4PVPlacement(zRot180, G4ThreeVector(0.0*cm,0.0*cm,51.0*mm), l_Poly15_1, Poly15_name1, l_RFA_mom, false, 4, overlap_check);
  new G4PVPlacement(zRot225, G4ThreeVector(0.0*cm,0.0*cm,51.0*mm), l_Poly15_1, Poly15_name1, l_RFA_mom, false, 5, overlap_check);
  new G4PVPlacement(zRot270, G4ThreeVector(0.0*cm,0.0*cm,51.0*mm), l_Poly15_1, Poly15_name1, l_RFA_mom, false, 6, overlap_check);
  new G4PVPlacement(zRot315, G4ThreeVector(0.0*cm,0.0*cm,51.0*mm), l_Poly15_1, Poly15_name1, l_RFA_mom, false, 7, overlap_check);

  const G4String Poly15_name2 = "Poly_block15_2";
  G4Tubs* s_Poly15_2 = new G4Tubs(Poly15_name2, 238.5*mm, 388.5*mm, 75.0*mm, 0.5*deg, 44.0*deg);
  G4LogicalVolume* l_Poly15_2 = new G4LogicalVolume(s_Poly15_2, PE, Poly15_name2);
  new G4PVPlacement(      0, G4ThreeVector(0.0*cm,0.0*cm,-59.0*mm), l_Poly15_2, Poly15_name2, l_RFA_mom, false, 0, overlap_check);
  new G4PVPlacement( zRot45, G4ThreeVector(0.0*cm,0.0*cm,-59.0*mm), l_Poly15_2, Poly15_name2, l_RFA_mom, false, 1, overlap_check);
  new G4PVPlacement( zRot90, G4ThreeVector(0.0*cm,0.0*cm,-59.0*mm), l_Poly15_2, Poly15_name2, l_RFA_mom, false, 2, overlap_check);
  new G4PVPlacement(zRot135, G4ThreeVector(0.0*cm,0.0*cm,-59.0*mm), l_Poly15_2, Poly15_name2, l_RFA_mom, false, 3, overlap_check);
  new G4PVPlacement(zRot180, G4ThreeVector(0.0*cm,0.0*cm,-59.0*mm), l_Poly15_2, Poly15_name2, l_RFA_mom, false, 4, overlap_check);
  new G4PVPlacement(zRot225, G4ThreeVector(0.0*cm,0.0*cm,-59.0*mm), l_Poly15_2, Poly15_name2, l_RFA_mom, false, 5, overlap_check);
  new G4PVPlacement(zRot270, G4ThreeVector(0.0*cm,0.0*cm,-59.0*mm), l_Poly15_2, Poly15_name2, l_RFA_mom, false, 6, overlap_check);
  new G4PVPlacement(zRot315, G4ThreeVector(0.0*cm,0.0*cm,-59.0*mm), l_Poly15_2, Poly15_name2, l_RFA_mom, false, 7, overlap_check);

  const G4String Poly15_name3 = "Poly_block15_3";
  G4Tubs* s_Poly15_3 = new G4Tubs(Poly15_name3, 238.5*mm, 388.5*mm, 70.0*mm, 0.5*deg, 44.0*deg);
  G4LogicalVolume* l_Poly15_3 = new G4LogicalVolume(s_Poly15_3, PE, Poly15_name3);
  new G4PVPlacement(      0, G4ThreeVector(0.0*cm,0.0*cm,-209.0*mm), l_Poly15_3, Poly15_name3, l_RFA_mom, false, 0, overlap_check);
  new G4PVPlacement( zRot45, G4ThreeVector(0.0*cm,0.0*cm,-209.0*mm), l_Poly15_3, Poly15_name3, l_RFA_mom, false, 1, overlap_check);
  new G4PVPlacement( zRot90, G4ThreeVector(0.0*cm,0.0*cm,-209.0*mm), l_Poly15_3, Poly15_name3, l_RFA_mom, false, 2, overlap_check);
  new G4PVPlacement(zRot135, G4ThreeVector(0.0*cm,0.0*cm,-209.0*mm), l_Poly15_3, Poly15_name3, l_RFA_mom, false, 3, overlap_check);
  new G4PVPlacement(zRot180, G4ThreeVector(0.0*cm,0.0*cm,-209.0*mm), l_Poly15_3, Poly15_name3, l_RFA_mom, false, 4, overlap_check);
  new G4PVPlacement(zRot225, G4ThreeVector(0.0*cm,0.0*cm,-209.0*mm), l_Poly15_3, Poly15_name3, l_RFA_mom, false, 5, overlap_check);
  new G4PVPlacement(zRot270, G4ThreeVector(0.0*cm,0.0*cm,-209.0*mm), l_Poly15_3, Poly15_name3, l_RFA_mom, false, 6, overlap_check);
  new G4PVPlacement(zRot315, G4ThreeVector(0.0*cm,0.0*cm,-209.0*mm), l_Poly15_3, Poly15_name3, l_RFA_mom, false, 7, overlap_check);

  const G4String Poly10_name2 = "Poly_block10_2";
  G4Tubs* s_Poly10_2 = new G4Tubs(Poly10_name2, 238.5*mm, 338.5*mm, 47.5*mm, 0.5*deg, 44.0*deg);
  //G4Tubs* s_Poly10_2 = new G4Tubs(Poly10_name2, 238.5*mm, 388.5*mm, 47.5*mm, 0.5*deg, 44.0*deg);
  G4LogicalVolume* l_Poly10_2 = new G4LogicalVolume(s_Poly10_2, PE, Poly10_name2);
  new G4PVPlacement(      0, G4ThreeVector(0.0*cm,0.0*cm,-327.5*mm), l_Poly10_2, Poly10_name2, l_RFA_mom, false, 0, overlap_check);
  new G4PVPlacement( zRot45, G4ThreeVector(0.0*cm,0.0*cm,-327.5*mm), l_Poly10_2, Poly10_name2, l_RFA_mom, false, 1, overlap_check);
  new G4PVPlacement( zRot90, G4ThreeVector(0.0*cm,0.0*cm,-327.5*mm), l_Poly10_2, Poly10_name2, l_RFA_mom, false, 2, overlap_check);
  new G4PVPlacement(zRot135, G4ThreeVector(0.0*cm,0.0*cm,-327.5*mm), l_Poly10_2, Poly10_name2, l_RFA_mom, false, 3, overlap_check);
  new G4PVPlacement(zRot180, G4ThreeVector(0.0*cm,0.0*cm,-327.5*mm), l_Poly10_2, Poly10_name2, l_RFA_mom, false, 4, overlap_check);
  new G4PVPlacement(zRot225, G4ThreeVector(0.0*cm,0.0*cm,-327.5*mm), l_Poly10_2, Poly10_name2, l_RFA_mom, false, 5, overlap_check);
  new G4PVPlacement(zRot270, G4ThreeVector(0.0*cm,0.0*cm,-327.5*mm), l_Poly10_2, Poly10_name2, l_RFA_mom, false, 6, overlap_check);
  new G4PVPlacement(zRot315, G4ThreeVector(0.0*cm,0.0*cm,-327.5*mm), l_Poly10_2, Poly10_name2, l_RFA_mom, false, 7, overlap_check);

  const G4String Poly10_name3 = "Poly_block10_3";
  G4Tubs* s_Poly10_3 = new G4Tubs(Poly10_name3, 238.5*mm, 338.5*mm, 67.5*mm, 0.5*deg, 44.0*deg);
  //G4Tubs* s_Poly10_3 = new G4Tubs(Poly10_name3, 238.5*mm, 388.5*mm, 67.5*mm, 0.5*deg, 44.0*deg);
  G4LogicalVolume* l_Poly10_3 = new G4LogicalVolume(s_Poly10_3, PE, Poly10_name3);
  new G4PVPlacement(      0, G4ThreeVector(0.0*mm,0.0*mm,-447.5*mm), l_Poly10_3, Poly10_name3, l_RFA_mom, false, 0, overlap_check);
  new G4PVPlacement( zRot45, G4ThreeVector(0.0*mm,0.0*mm,-447.5*mm), l_Poly10_3, Poly10_name3, l_RFA_mom, false, 1, overlap_check);
  new G4PVPlacement( zRot90, G4ThreeVector(0.0*mm,0.0*mm,-447.5*mm), l_Poly10_3, Poly10_name3, l_RFA_mom, false, 2, overlap_check);
  new G4PVPlacement(zRot135, G4ThreeVector(0.0*mm,0.0*mm,-447.5*mm), l_Poly10_3, Poly10_name3, l_RFA_mom, false, 3, overlap_check);
  new G4PVPlacement(zRot180, G4ThreeVector(0.0*mm,0.0*mm,-447.5*mm), l_Poly10_3, Poly10_name3, l_RFA_mom, false, 4, overlap_check);
  new G4PVPlacement(zRot225, G4ThreeVector(0.0*mm,0.0*mm,-447.5*mm), l_Poly10_3, Poly10_name3, l_RFA_mom, false, 5, overlap_check);
  new G4PVPlacement(zRot270, G4ThreeVector(0.0*mm,0.0*mm,-447.5*mm), l_Poly10_3, Poly10_name3, l_RFA_mom, false, 6, overlap_check);
  new G4PVPlacement(zRot315, G4ThreeVector(0.0*mm,0.0*mm,-447.5*mm), l_Poly10_3, Poly10_name3, l_RFA_mom, false, 7, overlap_check);

  const G4String Poly10_name4 = "Poly_block10_4";
  G4Tubs* s_Poly10_4 = new G4Tubs(Poly10_name4, 238.5*mm, 338.5*mm, 50.5*mm, 0.5*deg, 44.0*deg);
  //G4Tubs* s_Poly10_4 = new G4Tubs(Poly10_name4, 238.5*mm, 388.5*mm, 50.5*mm, 0.5*deg, 44.0*deg);
  G4LogicalVolume* l_Poly10_4 = new G4LogicalVolume(s_Poly10_4, PE, Poly10_name4);
  new G4PVPlacement(      0, G4ThreeVector(0.0*mm,0.0*mm,-565.5*mm), l_Poly10_4, Poly10_name4, l_RFA_mom, false, 0, overlap_check);
  new G4PVPlacement( zRot45, G4ThreeVector(0.0*mm,0.0*mm,-565.5*mm), l_Poly10_4, Poly10_name4, l_RFA_mom, false, 1, overlap_check);
  new G4PVPlacement( zRot90, G4ThreeVector(0.0*mm,0.0*mm,-565.5*mm), l_Poly10_4, Poly10_name4, l_RFA_mom, false, 2, overlap_check);
  new G4PVPlacement(zRot135, G4ThreeVector(0.0*mm,0.0*mm,-565.5*mm), l_Poly10_4, Poly10_name4, l_RFA_mom, false, 3, overlap_check);
  new G4PVPlacement(zRot180, G4ThreeVector(0.0*mm,0.0*mm,-565.5*mm), l_Poly10_4, Poly10_name4, l_RFA_mom, false, 4, overlap_check);
  new G4PVPlacement(zRot225, G4ThreeVector(0.0*mm,0.0*mm,-565.5*mm), l_Poly10_4, Poly10_name4, l_RFA_mom, false, 5, overlap_check);
  new G4PVPlacement(zRot270, G4ThreeVector(0.0*mm,0.0*mm,-565.5*mm), l_Poly10_4, Poly10_name4, l_RFA_mom, false, 6, overlap_check);
  new G4PVPlacement(zRot315, G4ThreeVector(0.0*mm,0.0*mm,-565.5*mm), l_Poly10_4, Poly10_name4, l_RFA_mom, false, 7, overlap_check);

  ///////////////////////////////////// STR /////////////////////////////////////////////////////////////

  //// STR_mother
  const G4String STRname = "STR_mother";
  G4Tubs* s_STR_mom = new G4Tubs(STRname, 0.0*mm, 140.0/2.0*mm, 920/2.0*mm, 0.*deg, 360.*deg);
  G4LogicalVolume* l_STR_mom = new G4LogicalVolume(s_STR_mom, air_3hPa, STRname);
  new G4PVPlacement(0, G4ThreeVector(0.0*mm,-489*mm,380.0*mm), l_STR_mom, STRname, l_Pogo, false, 0, overlap_check);

  const G4String STR_baffle_name = "STR_baffle";
  G4Tubs* s_STR_baffle = new G4Tubs(STR_baffle_name, 135.0/2.0*mm, 140.0/2.0*mm, 510/2.0*mm, 0.*deg, 360.*deg);
  G4LogicalVolume* l_STR_baffle = new G4LogicalVolume(s_STR_baffle, PVC, STR_baffle_name);
  new G4PVPlacement(0, G4ThreeVector(0.0*mm, 0.0*mm,205.0*mm), l_STR_baffle, STR_baffle_name, l_STR_mom, false, 0, overlap_check);

  const G4String STR_plate_name = "STR_baffle_plate";
  G4Tubs* s_STR_baffle_plate = new G4Tubs(STR_plate_name, 105.0/2.0*mm, 135.0/2.0*mm, 4.8/2.0*mm, 0.*deg, 360.*deg);
  G4LogicalVolume* l_STR_baffle_plate = new G4LogicalVolume(s_STR_baffle_plate, Al_6061, STR_plate_name);
  new G4PVPlacement(0, G4ThreeVector(0.0*mm,0.0*mm,455.0*mm), l_STR_baffle_plate, STR_plate_name, l_STR_mom, false, 0, overlap_check);
  new G4PVPlacement(0, G4ThreeVector(0.0*mm,0.0*mm,395.0*mm), l_STR_baffle_plate, STR_plate_name, l_STR_mom, false, 1, overlap_check);
  new G4PVPlacement(0, G4ThreeVector(0.0*mm,0.0*mm,345.0*mm), l_STR_baffle_plate, STR_plate_name, l_STR_mom, false, 2, overlap_check);
  new G4PVPlacement(0, G4ThreeVector(0.0*mm,0.0*mm,295.0*mm), l_STR_baffle_plate, STR_plate_name, l_STR_mom, false, 3, overlap_check);
  new G4PVPlacement(0, G4ThreeVector(0.0*mm,0.0*mm,245.0*mm), l_STR_baffle_plate, STR_plate_name, l_STR_mom, false, 4, overlap_check);
  new G4PVPlacement(0, G4ThreeVector(0.0*mm,0.0*mm,195.0*mm), l_STR_baffle_plate, STR_plate_name, l_STR_mom, false, 5, overlap_check);
  new G4PVPlacement(0, G4ThreeVector(0.0*mm,0.0*mm,145.0*mm), l_STR_baffle_plate, STR_plate_name, l_STR_mom, false, 6, overlap_check);
  new G4PVPlacement(0, G4ThreeVector(0.0*mm,0.0*mm, 95.0*mm), l_STR_baffle_plate, STR_plate_name, l_STR_mom, false, 7, overlap_check);
  new G4PVPlacement(0, G4ThreeVector(0.0*mm,0.0*mm, 45.0*mm), l_STR_baffle_plate, STR_plate_name, l_STR_mom, false, 8, overlap_check);
  new G4PVPlacement(0, G4ThreeVector(0.0*mm,0.0*mm, -5.0*mm), l_STR_baffle_plate, STR_plate_name, l_STR_mom, false, 9, overlap_check);

  const G4String STR_name = "STR";
  G4Tubs* s_STR = new G4Tubs(STR_name, 92.0/2.0*mm, 97.0/2.0*mm, 390/2.0*mm, 0.*deg, 360.*deg);
  G4LogicalVolume* l_STR = new G4LogicalVolume(s_STR, Al_6061, STR_name);
  new G4PVPlacement(0, G4ThreeVector(0.0*mm, 0.0*mm,-255.0*mm), l_STR, STR_name, l_STR_mom, false, 0, overlap_check);

  const G4String STR_bottom_name = "STR_bottom";
  G4Tubs* s_STR_bottom = new G4Tubs(STR_bottom_name, 0.0*mm, 97.0/2.0*mm, 10/2.0*mm, 0.*deg, 360.*deg);
  G4LogicalVolume* l_STR_bottom = new G4LogicalVolume(s_STR_bottom, Al_6061, STR_bottom_name);
  new G4PVPlacement(0, G4ThreeVector(0.0*mm, 0.0*mm,-455.0*mm), l_STR_bottom, STR_bottom_name, l_STR_mom, false, 0, overlap_check);

  const G4String STR_top_name = "STR_top";
  G4Tubs* s_STR_top = new G4Tubs(STR_top_name, 97.0/2*mm, 140.0/2.0*mm, 10/2.0*mm, 0.*deg, 360.*deg);
  G4LogicalVolume* l_STR_top = new G4LogicalVolume(s_STR_top, Al_6061, STR_top_name);
  new G4PVPlacement(0, G4ThreeVector(0.0*mm, 0.0*mm,-55.0*mm), l_STR_top, STR_top_name, l_STR_mom, false, 0, overlap_check);

  const G4String STR_window_name = "STR_window";
  G4Tubs* s_STR_window = new G4Tubs(STR_top_name, 0.0*mm, 97.0/2.0*mm, 7.5/2.0*mm, 0.*deg, 360.*deg);
  G4LogicalVolume* l_STR_window = new G4LogicalVolume(s_STR_window, Glass, STR_window_name);
  new G4PVPlacement(0, G4ThreeVector(0.0*mm, 0.0*mm,-55.0*mm), l_STR_window, STR_window_name, l_STR_mom, false, 0, overlap_check);

  const G4String STR_centre_name = "STR_centre";
  G4Tubs* s_STR_centre = new G4Tubs(STR_centre_name, 0.0*mm, 75.0/2.0*mm, 380/2.0*mm, 0.*deg, 360.*deg);
  G4LogicalVolume* l_STR_centre = new G4LogicalVolume(s_STR_centre, FR4, STR_centre_name);
  new G4PVPlacement(0, G4ThreeVector(0.0*mm, 0.0*mm,-255.0*mm), l_STR_centre, STR_centre_name, l_STR_mom, false, 0, overlap_check);


  ///////////////////////////////////// Visualization ///////////////////////////////////////////////////
  G4VisAttributes* colourWhite = new G4VisAttributes(G4Colour(1.,1.,1.));
  G4VisAttributes* colourDirtyWhite = new G4VisAttributes(G4Colour(1.,1.,0.8));
  G4VisAttributes* colourGreen = new G4VisAttributes(G4Colour(0.,1.,0.));
  G4VisAttributes* colourRed = new G4VisAttributes(G4Colour(1.0,0.4,0.4));
  G4VisAttributes* colourGrey = new G4VisAttributes(G4Colour(0.8,0.8,0.8));
  G4VisAttributes* colourDark = new G4VisAttributes(G4Colour(0.2,0.2,0.2));
  G4VisAttributes* colourDirtyYellow = new G4VisAttributes(G4Colour(1.0, 1.0, 0.6));

  colourWhite->SetForceSolid(true);
  colourGrey->SetForceSolid(true);
  colourRed->SetForceSolid(true);
  colourDark->SetForceSolid(true);
  colourGreen->SetForceSolid(true);
  colourDirtyYellow->SetForceSolid(true);
  colourDirtyWhite->SetForceSolid(true);

  l_RFA->SetVisAttributes(colourGrey);
  l_Flange1->SetVisAttributes(colourWhite);
  l_Flange2->SetVisAttributes(colourWhite);
  l_Poly10_0->SetVisAttributes(colourDirtyYellow);
  l_Poly10_1->SetVisAttributes(colourDirtyYellow);
  l_Poly10_2->SetVisAttributes(colourDirtyYellow);
  l_Poly10_3->SetVisAttributes(colourDirtyYellow);
  l_Poly10_4->SetVisAttributes(colourDirtyYellow);
  l_Poly15_1->SetVisAttributes(colourDirtyYellow);
  l_Poly15_2->SetVisAttributes(colourDirtyYellow);
  l_Poly15_3->SetVisAttributes(colourDirtyYellow);

  l_Bottom_poly->SetVisAttributes(colourDirtyYellow);

  l_Top_PV->SetVisAttributes(colourWhite);
  l_Top_plate->SetVisAttributes(colourGrey);
  l_Base_plate->SetVisAttributes(colourGrey);
  l_PMT_PV->SetVisAttributes(colourWhite);
  l_Cold_plate->SetVisAttributes(colourGrey);

  l_El_PV->SetVisAttributes(colourWhite);
  l_El_bottom->SetVisAttributes(colourGrey);
  l_FADC->SetVisAttributes(colourGreen);

  l_STR_baffle->SetVisAttributes(colourWhite);
  l_STR->SetVisAttributes(colourWhite);
  l_STR_bottom->SetVisAttributes(colourGrey);
  l_STR_top->SetVisAttributes(colourGrey);
  l_STR_window->SetVisAttributes(colourRed);
  l_STR_baffle_plate->SetVisAttributes(colourDark);
  l_STR_centre->SetVisAttributes(colourGreen);

  l_RFA_mom->SetVisAttributes(G4VisAttributes::Invisible);
  l_STR_mom->SetVisAttributes(G4VisAttributes::Invisible);
}

StructureConstruction::~StructureConstruction(){}
