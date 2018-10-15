CXXFLAGS += -Wall -O2 -funroll-loops `root-config --cflags --libs`
LDFLAGS += -lstdc++

G4dataAnalysis: G4dataAnalysis.cc G4data.h
	g++ G4dataAnalysis.cc -o G4dataAnalysis $(CXXFLAGS) $(LDFLAGS)
