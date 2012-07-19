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
  Double_t mu; //will eventually give more to it!
  Double_t detector_length;
  static Double_t n_a;
  static Double_t dx;
  Double_t random_number;
  Double_t prob;


  

 public: //functions

  int PhotonStepperSlick();
  int PhotonStepperSF();
  int GetMu();
  int SetMu(Double_t mu);
  int SetDetectorLength(Double_t detector_length);
  int GetDetectorLength();
  int Looper();

  Photon();//constructor for default photon object

  Photon(Double_t mu,Double_t detector_length); //constructor for photon object

};

#endif
