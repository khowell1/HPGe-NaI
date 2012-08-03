//header file for geometry class!
#include <math.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include "TRandom3.h"

using namespace std;
#ifndef SETUPGEOMETRY_H
#define SETUPGEOMETRY_H

class SetupGeometry
{
 private:
  Double_t cyl_radius;//cm
  Double_t cyl_height;//cm
  Int_t volume_number; //will be set to whatever volume the photon is currently in
  Double_t geometrydata[3][3]; //holds volume number,radius and height for each geometry.
  static Double_t PI; //duh.
  static Double_t dr;

  Double_t oldsphposition[3];//from previous runthrough of geometry class
  Double_t finalsphposition[3];//new calculated spherical position from function
  Double_t exitsphposition[3];//if photon escapes, what the boundary sph coords are
  Double_t exitcartposition[3];

  Double_t cartposition[3];//cart position, current
  Double_t newsphaddition[3];//new positions to go through geo class,will be from photon class

 public:
  SetupGeometry();//default constructor 
  SetupGeometry(Int_t volume_number,Double_t new_cyl_radius,Double_t cyl_height);//specific

  void SetVolumeNumber(Int_t new_volume_number);
  Int_t GetVolumeNumber();
  void SetGeometryData(Double_t new_geometrydata[]);
  Double_t GetGeometryData(Int_t volume_number);
  void SetPhotonPosition(Double_t new_cartposition[]);
  Double_t GetPhotonPosition();
  void SetNewPhotonAddition(Double_t new_sphaddition[]);
  Double_t GetNewPhotonAddition();

  /* void PhotonSphCoordChanger(Double_t oldsphposition[],Double_t newsphaddition[]); */
  /* void PhotonSphCoordUpdater(Double_t oldsphposition[],Double_t newsphaddition[],Double_t finalsphposition[],Double_t cartposition[], Double_t PI); */
  /* Double_t ExitGeometryChecker(Double_t finalsphposition,Double_t exitsphposition,Double_t cyl_radius,Double_t cyl_height,Int_t volume_number); */
  /* void WhichVolumeChecker(Double_t finalsphposition,Double_t geometrydata[][],Double_t dr); */
};


#endif

//the master class will create multiple setup_geometry objects and give the radii and heights for each volume

  //  Double_t InDetectorChecker(Double_t finalsphposition[],Double_t cyl_radius,Double_t cyl_height);
