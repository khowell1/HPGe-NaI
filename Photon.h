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

#ifndef photon
#define photon

class Photon
{
 public: //variables
  Double_t initial_photon_energy;
  Double_t cross_section; //will eventually give more to it!
  Double_t detector_length;
  Double_t N_a;
  Double_t dx;
  TRandom3 anything; //for getting a random number
  Double_t random_number;
  Double_t Prob;
  
  Photon();//constructor for default photon object

  Photon(Double_t); //constructor for photon object

 public: //functions
  int PhotonStepperSF();
  int PhotonStepperSlick();
  int SetCrossSection();
  int SetPhotonEnergy();
  int GetCrossSection();
  int GetPosition();

};

#endif
