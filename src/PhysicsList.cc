//
//  PhysicsList.cc 2013-09-10  Maxime Chauvin
//

#include "PhysicsList.hh"

#include "G4EmLivermorePolarizedPhysics.hh"
#include "G4SystemOfUnits.hh"

PhysicsList::PhysicsList() : G4VModularPhysicsList()
{
  // EM Livermore Polarized physics
  RegisterPhysics(new G4EmLivermorePolarizedPhysics());

  defaultCutValue = 0.1*mm;
  cutForGamma     = defaultCutValue;
  cutForElectron  = defaultCutValue;
  cutForPositron  = defaultCutValue;

  SetVerboseLevel(1);
}

PhysicsList::~PhysicsList(){}

void PhysicsList::SetCuts()
{
  G4VUserPhysicsList::SetCuts();
  SetCutValue(cutForGamma, "gamma");
  SetCutValue(cutForElectron, "e-");
  SetCutValue(cutForPositron, "e+");  

  // fix lower limit for cut (default = 990 eV)
  G4ProductionCutsTable::GetProductionCutsTable()->SetEnergyRange(250*eV, 1*GeV);

  // print the table of elements with the cuts
  DumpCutValuesTable();
}  
