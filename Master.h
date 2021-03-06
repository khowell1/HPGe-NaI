//master class that will control everything :)

#include <math.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include "TRandom3.h"
#include "TSpline.h"
#include <string>
#include "TF1.h"
#include "RLinearInterpolant.h"
#include "RPhotonSource.h"
#include "Photon.h"
#include "SetupGeometry.h"
#include <string>
#include "TTree.h"
#include "TFile.h"

using namespace std;

#ifndef MASTER_H
#define MASTER_H

class Master
{
 private:
  Int_t photonnumber;
  Double_t energy;
  Double_t cartposition[3];
  Int_t volumenumber;
  Int_t interactiontype;//0 is nothing,1 is compton,2 is scattering
  Int_t loopnumber;
  static Double_t sourceposition[3];
  /* static Double_t topspacecylgeometry[3]; */
  /* static Double_t gecylgeometry[3]; */
  /* static Double_t spacecylgeometry[3]; */
  /* static Double_t naicylgeometry[3]; */
  static Double_t initialphotonenergy;
  static Double_t densityarray[4];
  static Double_t cylradii[4];
  static Double_t cylheights[4];
  static string muarray[4]; //filenames for mu attentuation coefficients
  //  Double_t newsphaddition[3];

 public: //methods
  RPhotonSource *photonsource;
  /* Photon *vacphoton; */
  /* Photon *gephoton; */
  /* Photon *spacephoton; */
  /* Photon *naiphoton; */
  Photon *photon;
  SetupGeometry *geometry;
  TFile *myfile;
  TTree *outTree;

  Master();
  ~Master();

  Double_t Initializer();
  Double_t Walk1Photon();
  void Finisher();
};

#endif
