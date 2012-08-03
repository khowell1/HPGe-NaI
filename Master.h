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
//#include "makeTTree.cc"

using namespace std;

#ifndef MASTER_H
#define MASTER_H

class Master
{
 private:

 public: //methods
  static Double_t sourceposition[3];
  static Double_t gecylgeometry[3];
  static Double_t spacecylgeometry[3];
  static Double_t naicylgeometry[3];
  static Double_t initialphotonenergy;

  RPhotonSource *photonsource;
  Photon *photon;
  SetupGeometry *geometry;

  Master();
  ~Master();

  Double_t Initializer();
  void Walk1Photon();
};

#endif
