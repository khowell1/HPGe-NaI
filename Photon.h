//beautiful header file for Kaitlin Howell's summer project! man header file

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
  //static variables
  static Double_t n_a;
  static Double_t dx;
  static Double_t number;
  static Double_t PI;
  static Double_t c;
  static Double_t electron_mass;
  static Double_t MeV_Jules_convert;
  //variables that are determined from main 
  Double_t photon_energy;
  Double_t mu;
  Double_t detector_length;
  //variables within classes
  TF1 *thetafinder; //a tf1 object 
  Double_t theta;
  Double_t phi; 
  Double_t photonproperties [4];  //[photon energy,distance,theta,phi]
  //objects from rich's interpolator
  RLinearInterpolant *mu_spline;
  RLinearInterpolant *coherent_spline;
  RLinearInterpolant *incoherent_spline;
  RLinearInterpolant *photoelec_spline;
  RLinearInterpolant *pairprod_spline;


 public: //constructors
  Photon();//constructor for default photon object

  Photon(Double_t mu,Double_t detector_length,Double_t photon_energy); //constructor for photon object
  ~Photon();

 public: //methods
  Double_t PhotonStepperSlick();
  Double_t InteractionFinder(Double_t photon_energy);
  Double_t ThetaFinder(Double_t photon_energy);
  Double_t PhiFinder();
  Double_t ComptonEnergyCalc(Double_t photon_energy,Double_t theta);
  void SetMu(Double_t mu);
  void SetSplineMu(Double_t photon_energy);
  Double_t GetMu();
  void SetDetectorLength(Double_t detector_length);
  Double_t GetDetectorLength(); 
  void SetPhotonEnergy(Double_t photon_energy);
  Double_t GetPhotonEnergy();
  //  void SetPhotonPosition(Double_t photonproperties);
  //  Double_t GetPhotonPosition();
  void SetPhotonProperties(Double_t photonproperties);
  Double_t GetPhotonProperties();

  void FileReader();
};

#endif
