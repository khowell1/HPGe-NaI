#include "Setup_Geometry.h"

Double_t Setup_Geometry::PI=3.141592653589793238462;

Setup_Geometry::Setup_Geometry() { //default object constructor
  gecylinder_radius=41.1; //cm
  gecylinder_height=6.53; //cm
}
Setup_Geometry::Setup_Geometry(Double_t new_gecylinder_radius,Double_t new_gecylinder_height) {
  //normal object constructor
  gecylinder_radius=new_gecylinder_radius;
  gecylinder_height=new_gecylinder_height;

Double_t Setup_Geometry::Ge_Volume_Calc(Double_t gecylinder_radius,Double_t gecylinder_height,Double_t ge_volume) {
  ge_volume=PI*gecylinder_radius*gecylinder_radius*gecylinder_height;
  return (ge_volume);
}

//have variables within function only for phi, theta and photon_distance that will be set equal to the real class variables at the end. Need these so can find the change in angles and distance and get the correct position!


 Double_t Setup_Geometry::Photon_Position_Changer(Double_t *oldposition
