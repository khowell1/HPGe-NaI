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
#include "TSpline.h"
#include <string>
#include "RLinearInterpolant.h"
#include "TF1.h"

using namespace std;

#ifndef PHOTON_H
#define PHOTON_H

class Photon
{
 private: //variables
  Double_t initial_photon_energy;
  Double_t mu;
  Double_t detector_length;
  static Double_t n_a;
  static Double_t dx;
  static Double_t number;
  Double_t random_number;
  Double_t prob;
  RLinearInterpolant *mu_spline;
  RLinearInterpolant *coherent_spline;
  RLinearInterpolant *incoherent_spline;
  RLinearInterpolant *photoelec_spline;
  RLinearInterpolant *pairprod_spline;
  TF1 *thetafinder;
  Double_t new_theta;

 public: //constructors
  Photon();//constructor for default photon object

  Photon(Double_t mu,Double_t detector_length,Double_t initial_photon_energy); //constructor for photon object

 public: //methods
  Double_t PhotonStepperSlick();
  Double_t InteractionFinder(Double_t initial_photon_energy);
  Double_t ThetaFinder(Double_t initial_photon_energy);
  void SetMu(Double_t mu);
  void SetSplineMu(Double_t initial_photon_energy);
  Double_t GetMu();
  void SetDetectorLength(Double_t detector_length);
  Double_t GetDetectorLength(); 
  void SetInitialPhotonEnergy(Double_t initial_photon_energy);
  Double_t GetInitialPhotonEnergy();
  void FileReader();
};

#endif
