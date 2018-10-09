
     =========================================================
     Geant4 - an Object-Oriented Toolkit for Simulation in HEP
     =========================================================

                    PoGOSim - PoGOLite Simulation
                            ----------

	INTRODUCTION
	------------
   PoGOSim is a realistic simulation of the X-ray polarimeter PoGOLite. 
   It simulates the interaction of polarized X-ray photons coming from an 
   astrophysical source within the detectors of PoGOLite.
   
   The aim of this simulation is to produce precise calibration data for 
   PoGOLite.
    - macros are provided to display the geometry and particle tracks with 
   OpenGL
    - the generation of particles is done via the new General Particle Source
    - the output can be either ascii or root file
 
 1- GEOMETRY DEFINITION

   (The support of gamma detection are scintillating crystals. 
   Crystals are circularly arranged to form a ring. Few rings make up the full
   detector (gamma camera). This is done by positionning Crystals in 
   Ring with an appropriate rotation matrix. Several copies of Ring are 
   then placed in the full detector.)

 2- PHYSICS LIST

   The physics list contains the Livermore polarized electromagnetic processes. 
   It is defined in the PhysicsList class as a Geant4 modular physics list with 
   registered physics builders provided in Geant4:
    - G4EmLivermorePolarizedPhysics - defines all Livermore EM polarized processes
   
   This physics list requires data files for:
    - low energy electromagnetic processes which path is defined via 
      the G4LEDATA environment variable
   
   See more on installation of the datasets in Geant4 Installation Guide,
   Chapter 3.3: Note On Geant4 Datasets:
   http://geant4.web.cern.ch/geant4/UserDocumentation/UsersGuides/InstallationGuide/html/ch03s03.html
 	 
 3- PRIMARY GENERATOR

   The generation of particule is done using the new General Particle Source. 
   Documentation for this can be found in:
   http://reat.space.qinetiq.com/gps/
   The default beam set in vis1.mac and run1.mac is a parallele beam of 
   gamma photons directed towards the instrument, along Z.
   The energy is set by the command /gps/ene/mono 59.5 keV
   and the polarization angle by /gps/polarization 1. 0. 0. # 0 deg
   
 4- DETECTOR RESPONSE

   A HIT is a record, track per track (even step per step), of all the 
   informations needed to simulate and analyse the detector response.
   
   In PoGOSim the slow, fast and BGO scintillators are considered as the 
   detector. Therefore they are declared 'sensitive detectors' (SD) in
   the DetectorConstruction class. 
   
   Then, a Hit is defined as a set of 4 informations per step, inside 
   the chambers, namely:
    (- the track identifier (an integer),
    - the chamber number,
    - the total energy deposit in this step,
    - the position of the deposit.)
   
   A given hit is an instance of the class TrackerHit which is created 
   during the tracking of a particle, step by step, in the method 
   TrackerSD::ProcessHits(). This hit is inserted in a HitsCollection.

   EventAction::EndOfEventAction() collects informations event per event
   from the hits collections, and print every hits in a file (via the method 
   EventAction::PrintHitsCollection(TrackerHitsCollection* HC, G4String name)).

 A- VISUALISATION

   The visualization manager is set via the G4VisExecutive class
   in the main() function in PoGOSim.cc.    
   The initialisation of the drawing is done via a set of /vis/ commands
   in the macro vis.mac. This macro is automatically read from
   the main function when the example is used in interactive running mode.

   By default, vis.mac opens an OpenGL viewer (/vis/open OGL).
   The user can change the initial viewer by commenting out this line
   and instead uncommenting one of the other /vis/open statements, such as
   HepRepFile or DAWNFILE (which produce files that can be viewed with the
   HepRApp and DAWN viewers, respectively).  Note that one can always
   open new viewers at any time from the command line.  For example, if
   you already have a view in, say, an OpenGL window with a name
   "viewer-0", then
      /vis/open DAWNFILE
   then to get the same view
      /vis/viewer/copyView viewer-0
   or to get the same view *plus* scene-modifications
      /vis/viewer/set/all viewer-0
   then to see the result
      /vis/viewer/flush

   The DAWNFILE, HepRepFile drivers are always available
   (since they require no external libraries), but the OGL driver requires
   that the Geant4 libraries have been built with the OpenGL option.

   For more information on visualization, including information on how to
   install and run DAWN, OpenGL and HepRApp, see the visualization tutorials,
   for example,
   http://geant4.slac.stanford.edu/Presentations/vis/G4[VIS]Tutorial/G4[VIS]Tutorial.html
   (where [VIS] can be replaced by DAWN, OpenGL and HepRApp)

   The tracks are automatically drawn at the end of each event, accumulated
   for all events and erased at the beginning of the next run.

B- HOW TO BUILD

    - Edit the file cmake.sh with your Geant4 installation path
    - Edit the env_setup.sh with your Geant4 datasets path
    - In CMakeLists.txt edit the line  "SET(CMAKE_MODULE_PATH /usr/local/geant4/geant4.9.6.p02/cmake/Modules/)" with your Geant4 installation path
        cd build
        ../cmake.sh				# to set up the Geant4_DIR
        source ../env_setup.sh	# to set up the environment variables directory
        make

C- HOW TO RUN

    - Execute PoGOSim in the 'interactive mode' with visualization
        ./PoGOSim
      and type in the commands you want line by line:  
        Idle> /control/verbose 2
        Idle> /tracking/verbose 2
        Idle> ...
        Idle> exit

    - Execute PoGOSim in the 'batch' mode from macro files 
     (without visualization)
        ./PoGOSim ../run1.mac output
