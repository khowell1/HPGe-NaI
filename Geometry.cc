//code before i translate to classes... why didnt i do this first?


#include <math.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>]
#include "TMath.h"


// (0.5, pi/2, 0)
// (0.5, 0, 0)
// (0.5, pi/2, pi)
// (0.5, pi, 0)
using namespace std;
//deals with changing both the spherical coordinates and cylindrical
void coord_sys_changer(double distance[],double theta[],double phi[],double oldarray[],double newarray[],double newarray_cyl[],double newarray_cart[],double PI,int n) {

  //switching to cartesian...and adding up the new and old coordinates so that I can translate this back to spherical
  newarray_cart[0]=newarray_cart[0]+distance[n]*sin(theta[n]*180.0/PI)*cos(phi[n]*180.0/PI); //newarray_cart[0]+oldarray[0]*sin(oldarray[1]*180.0/PI)*cos(oldarray[2]*180.0/PI) //cartesian x
  newarray_cart[1]=newarray_cart[1]+distance[n]*sin(theta[n]*180.0/PI)*sin(phi[n]*180.0/PI); //newarray_cart[1]+oldarray[0]*sin(oldarray[1]*180.0/PI)*sin(oldarray[2]*180/PI); //cartesian y
  newarray_cart[2]=newarray_cart[2]+distance[n]*cos(theta[n]*180.0/PI); //newarray_cart[2]+oldarray[0]*cos(oldarray[1]*180/PI); //cartesian z
  cout<<"cart: "<<newarray_cart[0]<<","<<newarray_cart[1]<<","<<newarray_cart[2]<<endl;

  //switching from cartesian to spherical, will be new coordinates that are fed back to photon class
  newarray[0]=sqrt(newarray_cart[0]*newarray_cart[0]+newarray_cart[1]*newarray_cart[1]+newarray_cart[2]*newarray_cart[2]); //new spherical radius
  newarray[1]=TMath::ACos(newarray_cart[2]/newarray[0]);//theta
  newarray[2]=TMath::ATan2(newarray_cart[1],newarray_cart[0]);//phi
  cout<<"spherical: "<<newarray[0]<<","<<newarray[1]<<","<<newarray[2]<<endl;

  //to change the new array's spherical coordinates to cylinder:
  newarray_cyl[0]=newarray_cyl[0]+newarray[0]*sin(newarray[1]*180.0/PI); //cylindrical radius=distance*sin(theta)
  newarray_cyl[1]=newarray[1]; //same in both systems
  newarray_cyl[2]=newarray_cyl[2]+newarray[0]*cos(newarray[1]*180.0/PI);//cylindrical height=distance*cos(theta)
  //  cout<<"cylindrical: "<<newarray_cyl[0]<<","<<newarray_cyl[1]<<","<<newarray_cyl[2]<<endl;
}

//checks whether the calculated cylindrical coordinates are inside or outside the cylinder of given dimensions. 
double in_detector_checker(double const *newarray_cyl,double exitarray_cyl[],double ge_height,double ge_radius) {
  if (newarray_cyl[0]>ge_radius || newarray_cyl[2]>ge_height) {//checks whether the photon's cylindrical radius is greater than the given ge_radius 
    cout<<"escaped!"<<endl;
    exitarray_cyl[0]=2*newarray_cyl[0]-ge_radius;
    exitarray_cyl[1]=newarray_cyl[1];
    exitarray_cyl[2]=2*newarray_cyl[2]-ge_height;
    cout<<"exit array: "<<exitarray_cyl[0]<<","<<exitarray_cyl[1]<<","<<exitarray_cyl[2]<<endl;
    return true;
  }
  return 0;
}

//good to separate from the geo function
void position_changer(double oldarray[],double newarray[]) {
  //now making the old set of coordinates the new set...
  oldarray[0]=newarray[0];
  oldarray[1]=newarray[1];
  oldarray[2]=newarray[2];
}
int main() {
  double PI=3.141592653589793238462;
  double oldarray[3]={0,0,0}; //from zero point, {distance,theta,phi}, spherical
  double newarray[3]={0,0,0}; //new coords from photon class functions, spherical
  double newarray_cart[3]={0,0,0};
  double newarray_cyl[3]; //cyl coords, {radius,phi,height}
  double exitarray_cyl[3];
  double distance[4]={0.5,0.5,0.5,0.5}; //for example, will be photon_distance from current code, but just incrementally changing distance, phi and theta for now!
  double theta[4]={PI/2.0,2.0*PI/2.0,3.0*PI/2.0,4.0*PI/2.0}; //rad
  double phi[4]={0,PI,-PI,0}; //rad
  double max_geradius=4.11; //cm
  double ge_height=4.11; //cm
  double ge_radius=6.85; //cm
  bool result;

  for (int n=0;n<4;n++) {
    coord_sys_changer(distance,theta,phi,oldarray,newarray,newarray_cyl,newarray_cart,PI,n);
    //    result=in_detector_checker(newarray_cyl,exitarray_cyl,ge_height,ge_radius);
    if (result==true) {
      break;
    }
    position_changer(oldarray,newarray);
  }
  return 0;
}
