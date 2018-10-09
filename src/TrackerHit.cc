//
//  TrackerHit.cc 2013-09-04  Maxime Chauvin
//

#include "TrackerHit.hh"

G4Allocator<TrackerHit> TrackerHitAllocator;

TrackerHit::TrackerHit(){}

TrackerHit::~TrackerHit(){}

TrackerHit::TrackerHit(const TrackerHit &right) : G4VHit()
{
  m_Edep = right.m_Edep;
  m_position = right.m_position;
}

const TrackerHit& TrackerHit::operator=(const TrackerHit &right)
{
  m_Edep = right.m_Edep;
  m_position = right.m_position;
  return *this;
}

int TrackerHit::operator==(const TrackerHit &right) const
{
  if (this==&right){
    return 1;
  } else {
    return 0;
  }
}
