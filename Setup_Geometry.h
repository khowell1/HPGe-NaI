
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

class Setup_Geometry
{
 private:
  Double_t gecylinder_radius; //cm^2
  Double_t gecylinder_height;
  Double_t ge_volume;
  Double_t PI;
  Double_t oldposition[3];
  Double_t newposition[3];
 public:
  Double_t Ge_Volume_Calc(Double_t gecyclinder_radius,Double_t gecylinder_height,Double_t ge_volume);
  Double_t Photon_Location(Double_t oldposition[3],Double_t newposition[3]); //old array is previous position in spherical, newarray is new coordinates taken from the functions in photon_follower, this function will 
}
#endif
