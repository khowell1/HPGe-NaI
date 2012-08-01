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
  Double_t cyl_radius;//cm
  Double_t cyl_height;//cm
  Int_t volume_number; //will be set to whatever volume the photon is currently in
  Double_t geometrydata[3][3]; //holds volume number,radius and height for each geometry.
  Double_t PI; //duh.
  Double_t dr=0.1;
  Double_t oldsphposition[3];//from previous runthrough of geometry class
  Double_t newsphaddition[3];//new positions to go through geo class,will be from photon class
  Double_t finalsphposition[3];//new calculated spherical position from function
  Double_t exitsphposition[3];//if photon escapes, what the boundary sph coords are
  Double_t cartposition[3];//cart position, current

 public:
  Geometry();//default constructor 
  Geometry(Double_t new_cyl_radius,Double_t cyl_height,Int_t volume_number);//specific
  ~Geometry(); //deconstructor
  void PhotonSphCoordChanger(Double_t oldsphposition[],Double_t newsphaddition[]);
  void PhotonSphCoordUpdater(Double_t oldsphposition[],Double_t newsphaddition[],Double_t finalsphposition[],Double_t cartposition[], Double_t PI);
  Double_t ExitGeometryChecker(Double_t finalsphposition,Double_t exitsphposition,Double_t cyl_radius,Double_t cyl_height,Int_t volume_number);
  void WhichVolumeChecker(Double_t finalsphposition,Double_t geometrydata[][],Double_t dr);
}


#endif

//the master class will create multiple setup_geometry objects and give the radii and heights for each volume

  //  Double_t InDetectorChecker(Double_t finalsphposition[],Double_t cyl_radius,Double_t cyl_height);
