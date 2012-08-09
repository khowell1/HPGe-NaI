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

  void SetVolumeNumber(Int_t new_volume_number);
  Int_t GetVolumeNumber();
  void SetGeometryData(Double_t new_geometrydata[]);
  Double_t GetGeometryData();
  void SetPhotonPosition(Double_t currentcartposition[]); //fills an array with cart coords
  void SetNewSphAddition(Double_t new_sphaddition[]);
  void SetCartPosition(); //adds the new spherical info to current cart coords
  void PhotonVolumePosition();
  void PhotonExitCartPosition(Int_t old_volume_number);
  bool GetNewVolumeReached();
  void SetOriginalPosition(Double_t sourceposition[]);
};

#endif

  //  Double_t GetNewSphAddition();
  //  Double_t GetPhotonPosition(); //prints current cart coords
  //  void SetPhotonPosition(Double_t new_cartposition[]);//sets cart position to given coords
