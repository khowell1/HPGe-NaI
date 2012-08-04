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
  Double_t newsphaddition[3];

  RPhotonSource *photonsource;
  Photon *photon;
  SetupGeometry *geometry;

  Master();
  ~Master();

  Double_t Initializer();
  Double_t Walk1Photon();
};

#endif


//I don't think I'm correctly changing the cart coords to cyl so then I always get out of bounds for my photon volume position.
//Need to test it and see what is actually going on!
