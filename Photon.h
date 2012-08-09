//beautiful photon header file for Kaitlin Howell's summer project!

#include <math.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include "TRandom3.h"
#include "TSpline.h"
#include <string>
#include "RLinearInterpolant.h"
#include "TF1.h"
#include <string>

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
  static Double_t densityarray[4];
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

  //  string muattentuationfile;
  

 public: //constructors
  Photon();//constructor for default photon object
  Photon(Double_t mu,Double_t photon_energy); //constructor for photon object
  ~Photon();//deconstructor

 public: //methods
  void SetMu(Double_t new_mu);//saves a new given mu
  void SetSplineMu(Int_t volumenumber);//calcs spline mu w/ currently saved energy, calls setmu
  Double_t GetMu(); //returns currently saved mu
  void SetPhotonEnergy(Double_t new_photon_energy); //saves a new given photon energy
  Double_t GetPhotonEnergy();//returns currently saved photon energy
  void FileReader(); //reads a attenuation coefficient file (mu's) and makes splines
  Double_t PhotonStepperSlick();//returns a random photon walk length
  Double_t InteractionFinder(); //returns either 1 for compton or 2 for photoelectric
  Double_t ThetaFinder(); //returns a random differential cross section theta
  Double_t PhiFinder(); //returns a random phi from 0->2PI
  Double_t ComptonEnergyCalc();//returns a compton scattering calced photon energy
};

#endif
