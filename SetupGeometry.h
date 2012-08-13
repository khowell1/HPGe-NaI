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

  static Double_t PI; //duh.
  static Double_t dr;

  Double_t geometrydata[4][3]; //holds volume number,radius and height for each geometry.
  Double_t cartposition[3];//cart position, current  
  Double_t newsphaddition[3];//new positions to go through geo class,will be from photon class
  Double_t cylposition[3];
  bool newvolumereached;


 public:

  SetupGeometry();//default constructor 
  SetupGeometry(Int_t new_volume_number,Double_t new_cyl_radius,Double_t new_cyl_height);

  void SetGeometryDataArray(Int_t n,Double_t new_radius,Double_t new_height);
  void SetVolumeNumber(Int_t new_volume_number);
  Int_t GetVolumeNumber();
  void SetDistance(Double_t new_distance);
  Double_t GetDistance();
  void SetTheta(Double_t new_theta);
  Double_t GetTheta();
  void SetPhi(Double_t new_phi);
  Double_t GetPhi();
  void SetGeometryData(Double_t new_geometrydata[]);
  void SetPhotonPosition(Double_t currentcartposition[]); //fills an array with cart coords
  void SetNewSphAddition(Double_t new_sphaddition[]);
  void SetCartPosition(); //adds the new spherical info to current cart coords
  void SetCartPosition(Double_t sphaddition[]);//adds a given sphaddition to current cart coords
  void PhotonVolumePosition(Int_t loopnumber);
  void SetExitCartPosition(Int_t old_volume_number);
  bool GetNewVolumeReached();
  void SetOriginalPosition(Double_t sourceposition[]);
  void VolumeChecker(Int_t loopnumber);
};

#endif

  //  Double_t GetGeometryData();
  //  Double_t GetNewSphAddition();
  //  Double_t GetPhotonPosition(); //prints current cart coords
  //  void SetPhotonPosition(Double_t new_cartposition[]);//sets cart position to given coords
