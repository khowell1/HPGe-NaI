#include "Setup_Geometry.h"

Double_t Setup_Geometry::PI=3.141592653589793238462;

Setup_Geometry::Setup_Geometry() { //default object constructor
  gecylinder_radius=41.1; //cm
  gecylinder_height=6.53; //cm
  Double_t cylposition[3];
}
Setup_Geometry::SetupGeometry(Double_t new_gecylinder_radius,Double_t new_gecylinder_height) {
  //normal object constructor
  gecylinder_radius=new_gecylinder_radius;
  gecylinder_height=new_gecylinder_height;
  Double_t cylposition[3];

Double_t Setup_Geometry::GeVolumeCalc(Double_t gecylinder_radius,Double_t gecylinder_height,Double_t ge_volume) {
  ge_volume=PI*gecylinder_radius*gecylinder_radius*gecylinder_height;
  return (ge_volume);
}

//have variables within function only for phi, theta and photon_distance that will be set equal to the real class variables at the end. Need these so can find the change in angles and distance and get the correct position!


void Setup_Geometry::PhotonSphCoordChanger(Double_t oldsphposition[],Double_t newsphposition[]) {
   oldsphposition[0]=newsphposition[0];
   oldsphposition[1]=newsphposition[1];
   oldsphposition[2]=newsphposition[2];
 }

void PhotonSphCoordUpdater(Double_t oldsphposition[],Double newsphposition[],Double_t finalsphposition[],Double_t cartposition[],Double_t PI) {

//switching to cartesian...and adding up the new and old coordinates
   cartposition[0]=oldsphposition[0]*sin(oldsphposition[1]*180.0/PI)*cos(oldsphposition[2]*180.0*PI)+newsphposition[0]*sin(newsphposition[1]*180.0/PI)*cos(newsphposition[2]*180.0/PI); //cart x
  cartposition[1]=oldsphposition[0]*sin(oldsphposition[1]*180.0/PI)*sin(oldsphposition[2]*180.0*PI)+newsphposition[0]*sin(newsphposition[1]*180.0/PI)*sin(newsphposition[2]*180.0/PI); //cart y
  cartposition[2]=oldsphposition[0]*cos(oldsphposition[1]*180.0/PI)+newsphposition[0]*cos(newsphposition[1]*180.0/PI); //cart z
  cout<<"cart: "<<cartposition[0]<<","<<cartposition[1]<<","<<cartposition[2]<<endl;

  //switching from cartesian to spherical
  finalsphposition[0]=sqrt(cartposition[0]*cartposition[0]+cartposition[1]*cartposition[1]+cartposition[2]*cartposition[2]); //new spherical radius
  finalsphposition[1]=TMath::ACos(cartposition[2]/finalsphposition[0]);//theta
  newarray[2]=TMath::ATan2(cartposition[1],cartposition[0]);//phi
  cout<<"spherical: "<<finalsphposition[0]<<","<<finalsphposition[1]<<","<<finalsphposition[2]<<endl;
  //could just leave the cart position array as a local variable...
}

//checks whether the calculated cylindrical coordinates are inside or outside the cylinder of given dimensions. 
Double_t InDetectorChecker(Double_t finalsphposition[],Double_t gecyl_radius,Double_t gecyl_height) {
  //to change the new array's spherical coordinates to cylinder:
  cylposition[0]=finalsphposition[0]*sin(finalsphposition[1]*180.0/PI); //cylindrical radius=distance*sin(theta)
  cylposition[1]=finalsphposition[1]; //same in both systems
  cylposition[2]=finalsphposition[0]*cos(finalsphposition[1]*180.0/PI);//cylindrical height=distance*cos(theta)
  //  cout<<"cylindrical: "<<newarray_cyl[0]<<","<<newarray_cyl[1]<<","<<newarray_cyl[2]<<endl;
  if (cylposition[0]>gecyl_radius || newarray_cyl[2]>ge_height) {//checks whether the photon's cylindrical radius is greater than the given ge_radius 
    cout<<"escaped!"<<endl;
    exitsphposition[0]=2*cylposition[0]-gecyl_radius;
    exitarray_cyl[1]=cylposition[1];
    exitarray_cyl[2]=2*cylposition[2]-gecyl_height;
    cout<<"exit array: "<<exitarray_cyl[0]<<","<<exitarray_cyl[1]<<","<<exitarray_cyl[2]<<endl;
    return true;
  }
  return 0;
}

//need to decide how much is done within the in detector checker, probably should just do the checking and call on another method to fill the exitsphposition array. need to decide what really can just be local variables and what actually needs to be global. 
