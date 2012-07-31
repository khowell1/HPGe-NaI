//header file for geometry class!
#include <math.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include "TRandom3.h"

using namespace std;
#ifndef SETUP_GEOMETRY_H
#define SETUP_GEOMETRY_H

class SetupGeometry
{
 private:
  Double_t gecyl_radius; //cm^2
  Double_t gecyl_height;
  Double_t ge_volume;
  Double_t PI;
  Double_t oldsphposition[3];//from previous runthrough of geometry class
  Double_t newsphposition[3];//new positions to go through geo class
  Double_t finalsphposition[3];//what will be returned to photon class
  Double_t exitsphposition[3];//if photon escapes, what the sph coords are
  Double_t cartposition[3];//cart position, but will make this local...

 public:
  Double_t GeVolumeCalc(Double_t gecyclinder_radius,Double_t gecylinder_height,Double_t ge_volume);
  void PhotonSphCoordChanger(Double_t oldsphposition[],Double_t newsphposition[]);
  void PhotonSphCoordUpdater(Double_t oldsphposition[],Double_t newsphposition[],Double_t finalsphposition[],Double_t cartposition[], Double_t PI);
  Double_t InDetectorChecker(Double_t finalsphposition[],Double_t gecyl_radius,Double_t gecyl_height);
}


#endif
