//
//  RunAction.cc 2013-09-04  Maxime Chauvin
//

#include "RunAction.hh"

RunAction::RunAction(){}

RunAction::~RunAction(){}

void RunAction::BeginOfRunAction(const G4Run* run)
{    
  //Initialization
  G4cout << "  Start of run " << "\n"
         << "  Number of events requested "
         << run->GetNumberOfEventToBeProcessed() << "\n"
         << G4endl;
}

void RunAction::EndOfRunAction(const G4Run* run)
{
  G4cout << "  End of run " << "\n"
         << "  Number of events processed "
         << run->GetNumberOfEvent() << "\n"
         << G4endl;
}
