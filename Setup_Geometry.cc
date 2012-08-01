#include "Setup_Geometry.h"

Double_t Setup_Geometry::PI=3.141592653589793238462;

Setup_Geometry::Geometry() { //default object constructor
  cyl_radius=4.11; //cm
  cyl_height=6.53; //cm
  Double_t cylposition[3];
  volume_number=1;
  geometrydata[0][0]=volume_number;
  geometrydata[0][1]=cyl_radius;
  geometrydata[0][2]=cyl_height;
}
Setup_Geometry::Geometry(Double_t new_cyl_radius,Double_t new_cyl_height,Int_t new_volume_number,Double_t geometrydata[][]) {
  //normal object constructor
  cyl_radius=new_cyl_radius;
  cyl_height=new_cyl_height;
  Double_t cylposition[3];
  volume_number=new_volume_number;
  geometrydata[new_volume_number][0]=new_volume_number;
  geometrydata[new_volume_number][1]=new_cyl_radius;
  geometrydata[new_volume_number][2]=new_cyl_height;
}

Setup_Geometry::~Geometry() { //deconstructor
  delete GeGeometry();
  delete GeGeometry(Double_t cyl_radius,Double_t cyl_height,Int_t volume_number,Double_t geometrydata[][])
}

void Setup_Geometry::PhotonSphCoordChanger(Double_t oldsphposition[],Double_t newsphaddition[]) { //just changing the old spherical array to the new calculated spherical array
   oldsphposition[0]=newsphaddition[0];
   oldsphposition[1]=newsphaddition[1];
   oldsphposition[2]=newsphaddition[2];
}

void PhotonSphCoordUpdater(Double_t oldsphposition[],Double newsphaddition[],Double_t finalsphposition[],Double_t cartposition[],Double_t PI) {
//switching to cartesian...and adding up the new and old coordinates
   cartposition[0]=oldsphposition[0]*sin(oldsphposition[1]*180.0/PI)*cos(oldsphposition[2]*180.0*PI)+newsphaddition[0]*sin(newsphaddition[1]*180.0/PI)*cos(newsphaddition[2]*180.0/PI); //cart x
  cartposition[1]=oldsphposition[0]*sin(oldsphposition[1]*180.0/PI)*sin(oldsphposition[2]*180.0*PI)+newsphaddition[0]*sin(newsphaddition[1]*180.0/PI)*sin(newsphaddition[2]*180.0/PI); //cart y
  cartposition[2]=oldsphposition[0]*cos(oldsphposition[1]*180.0/PI)+newsphaddition[0]*cos(newsphaddition[1]*180.0/PI); //cart z
  cout<<"cart: "<<cartposition[0]<<","<<cartposition[1]<<","<<cartposition[2]<<endl;

  //switching from cartesian to spherical
  finalsphposition[0]=sqrt(cartposition[0]*cartposition[0]+cartposition[1]*cartposition[1]+cartposition[2]*cartposition[2]); //new spherical radius
  finalsphposition[1]=TMath::ACos(cartposition[2]/finalsphposition[0]);//theta
  newarray[2]=TMath::ATan2(cartposition[1],cartposition[0]);//phi
  cout<<"spherical: "<<finalsphposition[0]<<","<<finalsphposition[1]<<","<<finalsphposition[2]<<endl;
}

Double_t ExitGeometryChecker(Double_t finalsphposition[],Double_t exitsphposition,Double_t cyl_radius,Double_t cyl_height) {
  //to change the new array's spherical coordinates to cylinder:
  cylposition[0]=finalsphposition[0]*sin(finalsphposition[1]*180.0/PI); 
  cylposition[1]=finalsphposition[1];
  cylposition[2]=finalsphposition[0]*cos(finalsphposition[1]*180.0/PI);
  if (cylposition[0]>cyl_radius || newarray_cyl[2]>cyl_height) {
    exitsphposition[0]=finalsphposition[0]-(finalsphposition[0]-cyl_radius);
    exitsphposition[1]=finalsphposition[1];
    exitsphposition[2]=finalsphposition[2]-(finalsphposition[2]-cyl_radius);
    return true; //need to tell master class that the photon exited!
  }
  return 0;
}

void WhichVolumeChecker(Double_t finalsphposition,Double_t geometrydata[][],Double_t dr) {
  //will decide which volume the photon is in by increasing r by dr, checking cyl coords
  Double_t fakesphposition[3]={finalsphposition+dr,finalsphposition[1],finalsphposition[2]};
  Double_t fakecylposition[3]={fakesphposition[0]*sin(fakesphposition[1]*180.0/PI),fakesphposition[1],fakesphposition[0]*cos(fakesphposition[1]*180.0/PI)};
  for (int n=0;n<=(sizeof(geometrydata)/sizeof(*geometrydata));n++) {
    if (fakecylposition[0]<geometrydata[n][1]) {
      volume_number=n;
      break;
    }
    else if (fakecylposition[2]<geometrydata[n][2]){
      volume_number=n;
      break;
    }
  }
}
