//code before i translate to classes... why didnt i do this first?


#include <math.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>

using namespace std;
//deals with changing both the spherical coordinates and cylindrical
void new_geo(double distance,double theta,double phi,double oldarray[],double newarray[],double newarray_cyl[], double PI) {
  //say ive run the three functions to get new distance, theta and phi, then:
  newarray[0]=oldarray[0]+distance; //just incrementing all three by a set amount for now!
  newarray[1]=oldarray[1]+theta;
  newarray[2]=oldarray[2]+phi;
  if (theta>2*PI) { //just making sure theta<2PI
      theta=theta-2*PI;
  }
  if (phi>2*PI) {//same as theta
      phi=phi-2*PI;
  }
  cout<<newarray[0]<<","<<newarray[1]<<","<<newarray[2]<<endl;
  //to change the new array's spherical coordinates to cylinder:
  newarray_cyl[0]=newarray_cyl[0]+newarray[0]*sin(theta*180.0/PI); //cylindrical radius=distance*sin(theta)
  newarray_cyl[1]=newarray[1]; //same in both systems
  newarray_cyl[2]=newarray_cyl[2]+newarray[0]*cos(theta*180.0/PI);//cylindrical height=distance*cos(theta)
  cout<<newarray_cyl[0]<<","<<newarray_cyl[1]<<","<<newarray_cyl[2]<<endl;
  oldarray[0]=newarray[0];
  oldarray[1]=newarray[1];
  oldarray[2]=newarray[2];
}
//checks whether the calculated cylindrical coordinates are inside or outside the cylinder of given dimensions. 
double in_detector_checker(double const *newarray_cyl,double ge_height,double ge_radius) {
  if (newarray_cyl[0]>ge_radius) {//checks whether the photon's cylindrical radius is greater than the given ge_radius 
    cout<<"escaped!"<<endl;
    return true;
  }
  if (newarray_cyl[2]>ge_height) {//checks whether the photon's cylindrical height is greater than the given ge_height
    cout<<"escaped!"<<endl;
    return true;
  }
  return 0;
}

int main() {
  double PI=3.141592653589793238462;
  double oldarray[3]={0,0,0}; //from zero point, {distance,theta,phi}
  double newarray[3]={0,0,0}; //new coords from photon class functions
  double newarray_cyl[3]; //cyl coords, {radius,phi,height}
  double distance=1.0; //for example, will be photon_distance from current code, but just incrementally changing distance, phi and theta for now!
  double theta=PI/10.0; //rad
  double phi=PI/10.0; //rad
  double max_geradius=4.11; //cm
  double ge_height=4.11; //cm
  double ge_radius=6.85; //cm
  bool result;

  for (int n;n<10;n++) {
    new_geo(distance,theta,phi,oldarray,newarray,newarray_cyl,PI);
    result=in_detector_checker(newarray_cyl,ge_height,ge_radius);
    if (result==true) {
      break;
    }
  }
  return 0;
}
