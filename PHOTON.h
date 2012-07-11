//beautiful header file for Kaitlin Howell's summer project!

#include <math.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include "TRandom3.h"
#include "TGraphErrors.h"
#include "TAxis.h"
#include "TCanvas.h"
#include "TH1D.h"

using namespace std;

#ifndef PHOTON
#define PHOTON

class PHOTON 
{
 public: //variables
  Double_t initial_photon_energy;
  Double_t cross_section; //will eventually give more to it!
  Double_t detector_length;

 public: //functions
  int PhotonStepper();
  int SetCrossSection();
  int SetPhotonEnergy();
  int GetCrossSection();
  int GetPosition();

};

#endif
