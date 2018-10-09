//
//  TrackerHit.hh 2013-09-04  Maxime Chauvin
//

#ifndef TrackerHit_h
#define TrackerHit_h 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"
#include "G4DynamicParticle.hh"
#include "G4SystemOfUnits.hh"

class TrackerHit : public G4VHit
{
  public:
    TrackerHit();
   ~TrackerHit();
    TrackerHit(const TrackerHit&);
    const TrackerHit& operator=(const TrackerHit&);
    G4int operator==(const TrackerHit&) const;

    inline void *operator new(size_t);
    inline void operator delete(void* aHit);

  public:
    // set and get id of the scintillator
    inline void SetId(G4int id){ m_id = id; };
    inline G4int GetId() { return m_id; };

    // set and get energy deposition
    inline void SetEdep(G4double de) { m_Edep = de; };
    inline void AddEdep(G4double de) { m_Edep += de; };
    inline G4double GetEdep() { return m_Edep; };

    // set and get step length
    inline void SetStepLength(G4double sl) { m_stepLength = sl; };
    inline G4double GetStepLength() { return m_stepLength; };

    // set and get position
    inline void SetPosition(G4ThreeVector xyz) { m_position = xyz; };
    inline G4ThreeVector GetPosition() { return m_position; };

    // set and get post position
    inline void SetPostPosition(G4ThreeVector xyz) { m_postPosition = xyz; };
    inline G4ThreeVector GetPostPosition() { return m_postPosition; };

    // set and get momentum
    inline void SetMomentum(G4ThreeVector pxyz) { m_momentum = pxyz; };
    inline G4ThreeVector GetMomentum() { return m_momentum; };

    // set and get post momentum
    inline void SetPostMomentum(G4ThreeVector pxyz) { m_postMomentum = pxyz; };
    inline G4ThreeVector GetPostMomentum() { return m_postMomentum; };

    // set and get particle
    inline void SetParticle(G4DynamicParticle* par) { m_particle = par; };
    inline G4DynamicParticle* GetParticle() { return m_particle; };

    // set and get particle name/charge
    inline void SetParticleName(G4String name) { m_particleName = name; };
    inline G4String GetParticleName() { return m_particleName; };
    inline void SetCharge(G4double charge) { m_charge = charge; };
    inline G4double GetCharge() { return m_charge; };

    // set and get kinetic energy of the particle
    inline void SetKineticEnergy(G4double kine) { m_kineticEnergy = kine; };
    inline G4double GetKineticEnergy() { return m_kineticEnergy; };
    inline void SetPostKineticEnergy(G4double kine) { m_postKineticEnergy = kine; };
    inline G4double GetPostKineticEnergy() { return m_postKineticEnergy; };

    // set and get process name
    inline void SetProcessName(G4String name) { m_processName = name; };
    inline G4String GetProcessName() { return m_processName; };

    // set and get polarization vector
    inline void SetPolarization(G4ThreeVector pol) { m_polarization = pol; };
    inline G4ThreeVector GetPolarization() { return m_polarization; };
    inline void SetPostPolarization(G4ThreeVector ppol) { m_postPolarization = ppol; };
    inline G4ThreeVector GetPostPolarization() { return m_postPolarization; };

    // print all information
    inline void PrintAll() {
		std::cout << "Detector ID " << m_id << std::endl;
		std::cout << "Energy " << m_Edep/keV << std::endl;
		std::cout << "Step length " << m_stepLength << std::endl;
		std::cout << "Particle name " << m_particleName << std::endl;
		std::cout << "Particle charge " << m_charge << std::endl;
		std::cout << "Pre step " << m_position << std::endl;
		std::cout << "Post step " << m_postPosition << std::endl;
		std::cout << "Pre momentum " << m_momentum << std::endl;
		std::cout << "Post momentum " << m_postMomentum << std::endl;
		std::cout << "Pre kinetic energy " << m_kineticEnergy/keV << std::endl;
		std::cout << "Post kinetic energy " << m_postKineticEnergy/keV << std::endl;
		std::cout << "Process name " << m_processName << std::endl;
		std::cout << "Pre polarization " << m_polarization << std::endl;
		std::cout << "Post polarization " << m_postPolarization << std::endl;
		std::cout << std::endl;
	}

  private:
    // id of the scintillator
    G4int m_id;
    // deposit energy and step length
    G4double m_Edep, m_stepLength; 
    // particle and particle name/charge
    G4DynamicParticle* m_particle;
    G4String m_particleName;
    G4double m_charge;
    // start and end position of the track
    G4ThreeVector m_position, m_postPosition;
    // start and end momentum of the track
    G4ThreeVector m_momentum, m_postMomentum;
    // kinetic energy of the particle
    G4double m_kineticEnergy, m_postKineticEnergy;
    // process name
    G4String m_processName;
    // polarization verctor
    G4ThreeVector m_polarization, m_postPolarization;
};

typedef G4THitsCollection<TrackerHit> TrackerHitsCollection;

extern G4Allocator<TrackerHit> TrackerHitAllocator;

inline void* TrackerHit::operator new(size_t)
{
  void* aHit = (void*)TrackerHitAllocator.MallocSingle();
  return aHit;
}

inline void TrackerHit::operator delete(void* aHit)
{
  TrackerHitAllocator.FreeSingle((TrackerHit*) aHit);
}

#endif
