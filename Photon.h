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

#ifndef PHOTON_H
#define PHOTON_H

class Photon
{
 private: //variables
  //  Double_t initial_photon_energy;
  Double_t cross_section; //will eventually give more to it!
  static Double_t detector_length;
  static Double_t N_a;
  static Double_t dx;
  TRandom3 anything; //for getting a random number
  Double_t random_number;
  Double_t Prob;
  

 public: //functions
  int PhotonStepperSF();
  int PhotonStepperSlick();
  int SetCrossSection(Double_t new_cross_section);
  //   int SetPhotonEnergy();
  int GetCrossSection();

  Photon();//constructor for default photon object

  Photon(Double_t new_cross_section); //constructor for photon object

};

#endif
