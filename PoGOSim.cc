/*
 * ==========================================================================================================
 *                                       PoGOSim - PoGO+ Simulation
 * ==========================================================================================================
 * 
 * Versions -
 *      2015-11 - Creation Maxime Chauvin
 * 
 * Command line -
 *      Program name - PoGOSim
 *      Arg[1] - Macro file name
 *      Arg[2] - Output file name with no extension
 *
 *      ./PoGOSim run.mac output
 *
 * Output -
 * 
 *     output.root
 *       G4data (Tree)
 *         Inst_roll             (Leaf)
 *         primary   (Branch)
 *           int Id              (Leaf)
 *           float Energy        (Leaf)
 *           float Px            (Leaf)
 *           float Py            (Leaf)
 *           float Pz            (Leaf)
 *           float Dx            (Leaf)
 *           float Dy            (Leaf)
 *           float Dz            (Leaf)
 *           float PolX          (Leaf)
 *           float PolY          (Leaf)
 *           float PolZ          (Leaf)
 *         slow      (Branch)
 *           int nhits           (Leaf)
 *           float EdepTot       (Leaf)
 *           float Edep[nhits]   (Leaf)
 *           float Px[nhits]     (Leaf)
 *           float Py[nhits]     (Leaf)
 *           float Pz[nhits]     (Leaf)
 *           float UnitId[nhits] (Leaf)
 *           int nUnits          (Leaf)
 *           float EdepTot[61]   (Leaf)
 *         fast      (Branch)
 *           ...
 *         bottomBGO (Branch)
 *            ...
 *         SAS       (Branch)
 *             ...
 *
 * ==========================================================================================================
 */

#include "G4RunManager.hh"
#include "G4UImanager.hh"

#include "DetectorConstruction.hh"
#include "PhysicsList.hh"
#include "PrimaryGeneratorAction.hh"
#include "RunAction.hh"
#include "EventAction.hh"

#ifdef G4VIS_USE
 #include "G4VisExecutive.hh"
 #include "G4UIExecutive.hh"
#endif

int main(int argc, char** argv) {

  G4cout << "...................................................................................................." << G4endl;
  G4cout << "...............................................:tKKKD..............................................." << G4endl;
  G4cout << "..............................................WEf:ii:jWK............................................" << G4endl;
  G4cout << "............................................:#tE..:::,Kt............................................" << G4endl;
  G4cout << "...........................jjjjLjjjjf;;;;;;;EEE:::WD::KKK:;;;;;;jDDDEEDDDEt........................." << G4endl;
  G4cout << "..........................i....K....G.......EEDfW,KKG:DKKE......:.........f........................." << G4endl;
  G4cout << "..........................G....,....D......:EEjfKijGD.,KKK.......G.........D........................" << G4endl;
  G4cout << ".........................f.........::......EEEK.tfGftfLDKKi...........f....:........................" << G4endl;
  G4cout << ".........................:....:....i.....:EEEfj:KjLfKE:LKKKL.....i:....:...:,......................." << G4endl;
  G4cout << "........................:;;;;;;;;;;E......EEEjD:.iffKG.,KKKK....::LLLLLGLLLLK......................." << G4endl;
  G4cout << "........................f....:.....f.....LEEEWi..:,.Kj..DKKK.....t.....E....f......................." << G4endl;
  G4cout << ".......................::....L....::....:EEEEE......,..;jKKK,:...::....,:..........................." << G4endl;
  G4cout << ".......................i.....G....E.....:DDDGiGGGGGGGGGGfKKKt:,..:E.....G...:i:....................." << G4endl;
  G4cout << ".......................jjjjjjjjjjjjjjjjjjjjjijKKKKKKKKKKfGGGGGGGGGGGGGGGGGGGGGL:...................." << G4endl;
  G4cout << ".........................................jtGDjKKKKKKKKKKjK;j;:......................................" << G4endl;
  G4cout << "...........................................j;G;;;;;;;;;;Df;........................................." << G4endl;
  G4cout << "...................................................................................................." << G4endl;
  G4cout << ".......MNNMNO..........................8NNNNND$. ...........NNNNNNM................................." << G4endl;
  G4cout << ".......NNNNNNNNN....................INNNNM77NNNNNI.......NNNDDMMMMNDNN.............................." << G4endl;
  G4cout << ".......NN.... INN..... ... ...... .NNN.. .......NNN....NDND?........NNND............................" << G4endl;
  G4cout << ".......NN......MND... .. .. ......NNM....   ..  .... .MNN...OO...ZZ...NNM...........NN.............." << G4endl;
  G4cout << ".......NN......NN8...............NNN... .. . .  .  ...NN  ...Z...Z~....NN...........NN.............." << G4endl;
  G4cout << ".......NN.....NDN...MNNNNNNN.  ..NN. .. .. .... . ...NNN...,..O.O......NNN..........NN.............." << G4endl;
  G4cout << ".......NNNNNNNNN...NNM,...NNNZ...NN.. . .. .NNNNNNNN.NNM???+???O???????MNN....... ..NN.............." << G4endl;
  G4cout << ".......NN$$7,.....NN8.......NN...NN.......  NNNNNNNN.NNN......Z.O......NNN..NNNNNNNNNNNNNNNNNN......" << G4endl;
  G4cout << ".......NN..... ..,NN. ......NNN..NNN.............NNN.,NN....,Z...OI ...NN...NNNNNNNNNNNNNNNNNN......" << G4endl;
  G4cout << ".......NN.........NN........NNN...NNN. .... ....7NN...NNN...ZZ...OO...NNN...........NN.............." << G4endl;
  G4cout << ".......NN.........NNN .....DDN.....NNN. .......NNN. ...MNNN$........NDNN............NN.............." << G4endl;
  G4cout << ".......NN..........NNNND$DNNN.......$NNNNMZODNNND........MNNNNN8MNNNNN..............NN.............." << G4endl;
  G4cout << ".......NN............MNNNNN...... .....MNNNNNN,....... ... ,NNNNNNN.................NN.............." << G4endl;
  G4cout << "...................................................................................................." << G4endl;

  //Construct the default run manager
  G4RunManager* runManager = new G4RunManager;

  //set mandatory initialization classes
  DetectorConstruction* detector = new DetectorConstruction();
  runManager->SetUserInitialization(detector);

  PhysicsList* physics = new PhysicsList();
  runManager->SetUserInitialization(physics);

  //set user action classes
  PrimaryGeneratorAction* gen_action = new PrimaryGeneratorAction();
  runManager->SetUserAction(gen_action);

  RunAction* run_action = new RunAction();
  runManager->SetUserAction(run_action);

  EventAction* event_action = new EventAction(detector);
  runManager->SetUserAction(event_action);

  //Initialize Geant4 kernel
  runManager->Initialize();

  //get the pointer to the User Interface manager 
  G4UImanager* UIm = G4UImanager::GetUIpointer();  
  
  if (argc==1) {// interactive mode
  #ifdef G4VIS_USE
    event_action->CreateROOTFile("vis.root");
    G4VisManager* visManager = new G4VisExecutive;
    visManager->Initialize();
    G4UIExecutive* UIi = new G4UIExecutive(argc, argv);
    UIm->ApplyCommand("/control/execute ../scripts/vis0.mac");
    UIi->SessionStart();
    delete UIi;
    delete visManager;
  #endif
  }
  else if (argc==3) {// batch mode
    G4String ext_root = ".root";
    event_action->CreateROOTFile(argv[2] + ext_root);
    G4String fileName = argv[1];
  	G4String command = "/control/execute ";
    UIm->ApplyCommand(command + fileName);
  }
  else {
    G4cout << "Not the right number of arguments. Usage:" << G4endl;
    G4cout << " -interactive mode ./PoGOSim" << G4endl;
    G4cout << " -batch mode ./PoGOSim macro.mac output" << G4endl;
    exit(1);
  }

  //job termination
  delete runManager;
  return 0;
}
