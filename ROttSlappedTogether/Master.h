#include <iostream>
#include "Photon.h"
#include "Cylinder.h"
#include "RPhotonSource.h"
#include "TFile.h"
#include "TTree.h"
#include "TMath.h"
#include "TVector3.h"
#include "TRotation.h"
using namespace std;

#ifndef MASTER_H
#define MASTER_H

class Master { 

  //For I/O
  TFile *myfile;
  TTree *outTree;

  //Photon tracking
  Int_t photonNumber;
  Int_t volumeNumber;
  Int_t interactionType; //0 for none, 1 compton, 2 photoelec, 3 pair prod (later)
  Double_t cartPosition[3];
  Double_t cartDirection[3];
  TVector3 positionVec, directionVec; //Used later, but repeatedly
  TRotation r; //ditto
  Double_t photonEnergy;
  Double_t depositedEnergy;
  //Everything happens within sphere, photons that make it past this escape
  Double_t simulationBoundaryRadius;

  //Geometry and propagation
  vector<Photon*> photonWalkers;
  vector<Cylinder*> spaces;
  
  RPhotonSource *photonSource; //Only have one for the moment

  //Helper classes
  Int_t FindVolumeNumber();
  void TakeComptonStep();


 public:
  Master();
  ~Master();
  Bool_t Initializer(); //Sets up default behavior
  Double_t Walk1Photon();
  Double_t WalkPhoton(Double_t direction[3], Double_t position[3]);
  void Finisher();


};

#endif
