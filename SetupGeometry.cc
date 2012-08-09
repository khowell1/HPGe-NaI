#include "SetupGeometry.h"

Double_t SetupGeometry::PI=3.141592653589793238462;
Double_t SetupGeometry::dr=0.1;

SetupGeometry::SetupGeometry() { //default object constructor
  //only creates the ge detector geometry in the first array position  
  cyl_radius=4.11; //cm
  cyl_height=6.53; //cm
  volume_number=0;
  geometrydata[0][0]=volume_number;
  geometrydata[0][1]=cyl_radius;
  geometrydata[0][2]=cyl_height;
}

SetupGeometry::SetupGeometry(Int_t new_volume_number,Double_t new_cyl_radius,Double_t new_cyl_height) { //normal object constructor
  cyl_radius=new_cyl_radius;
  cyl_height=new_cyl_height;
  volume_number=new_volume_number;
  geometrydata[new_volume_number][0]=new_volume_number;
  geometrydata[new_volume_number][1]=new_cyl_radius;
  geometrydata[new_volume_number][2]=new_cyl_height;
}

void SetupGeometry::SetVolumeNumber(Int_t new_volume_number) {//sets new given volume number
  volume_number=new_volume_number;
}

Int_t SetupGeometry::GetVolumeNumber() {//returns current volume number
  return volume_number;
}

void SetupGeometry::SetGeometryData(Double_t new_geometrydata[]) {//sets new geometry data
  geometrydata[int(new_geometrydata[0])][0]=new_geometrydata[0];
  geometrydata[int(new_geometrydata[0])][1]=new_geometrydata[1];
  geometrydata[int(new_geometrydata[0])][2]=new_geometrydata[2];
}

Double_t SetupGeometry::GetGeometryData() {// prints current volume number geometry data
  cout<<geometrydata[volume_number][0]<<","<<geometrydata[volume_number][1]<<","<<geometrydata[volume_number][2]<<endl;
  return 0;
}

void SetupGeometry::SetOriginalPosition(Double_t sourceposition[]) {
  //sets geo class cartposition array to source position
  cartposition[0]=sourceposition[0];
  cartposition[1]=sourceposition[1];
  cartposition[2]=sourceposition[2];
}

void SetupGeometry::SetPhotonPosition(Double_t currentcartposition[]) {
  //sets a given array equal to geo class cart coords
  currentcartposition[0]=cartposition[0];
  currentcartposition[1]=cartposition[1];
  currentcartposition[2]=cartposition[2];
}

void SetupGeometry::SetNewSphAddition(Double_t new_sphaddition[]) {
  //adds new theta and phi to current theta and phi,updates the cart and position
  newsphaddition[0]=new_sphaddition[0];
  newsphaddition[1]=newsphaddition[1]+new_sphaddition[1];
  newsphaddition[2]=newsphaddition[2]+new_sphaddition[2];
  SetCartPosition();
}

void SetupGeometry::SetCartPosition() { //updates cart position with current sph data
  cartposition[0]=cartposition[0]+newsphaddition[0]*sin(newsphaddition[1])*cos(newsphaddition[2]);
  cartposition[1]=cartposition[1]+newsphaddition[0]*sin(newsphaddition[1])*sin(newsphaddition[2]);
  cartposition[2]=cartposition[2]+newsphaddition[0]*cos(newsphaddition[1]);
}



void SetupGeometry::PhotonVolumePosition(Int_t loopnumber) {
  //sets current photon cyl position, checks if z is negative and if so, returns a volume number of 5.
  newvolumereached=false;
  Int_t old_volume_number=volume_number;//thats ingenious lol, just to see if vol # changes from code!
  if (cartposition[2]<0 || cartposition[2]>geometrydata[0][2]) {//z too large or too small
    if (loopnumber==0) {//so 1st photon walk through the vacuum...
      // cout<<"running photonexitcartposition method"<<endl;
      // PhotonExitCartPosition(loopnumber); //will set photon position to top of ge
      SetVolumeNumber(1);
    }
    else {
      SetVolumeNumber(5);
    }
  }
  else {
    for (int n=0;n<=3;n++) {
      Double_t geocartposition[3]={geometrydata[n][1]*cos(newsphaddition[2]),geometrydata[n][1]*sin(newsphaddition[2]),geometrydata[n][2]}; //creates the current cart position of the geometry, with the angle from the spherical addition data, since they will be the same in both systems!
      
      if (cartposition[2]<=geometrydata[0][2] && cartposition[2]>geometrydata[1][2]) {
	cout<<"first if statement"<<endl;
      //checking if in space above detectors, only z component! 
	SetVolumeNumber(0);
	break;
      }
      else if (abs(cartposition[0])<=abs(geocartposition[0]) && abs(cartposition[1])<=abs(geocartposition[1]) && cartposition[2]>geometrydata[1][2]) { //checking the x and y for the space
	cout<<"second if statement"<<endl;
	SetVolumeNumber(n);
	break;
      }
      if (n>0) {
        if (abs(cartposition[0])<=abs(geocartposition[0]) || abs(cartposition[1])<=abs(geocartposition[1])) {
	  cout<<"third if statement"<<endl;
	  SetVolumeNumber(n);
	  break;
	}
	else {
	  cout<<"out of bounds or failed to check position correctly"<<endl;
	  SetVolumeNumber(6);
	}
      }
    }
  }
  if (old_volume_number!=volume_number) {
    cout<<"running photonexitcartposition method1"<<endl;
    cout<<"volumenumber"<<volume_number<<endl;
    SetExitCartPosition(loopnumber);//calls method to find new exit cart coords!
    //make sure new mu's are calculated by giving the new volume number!!!
  }
}

void SetupGeometry::SetExitCartPosition(Int_t loopnumber) {

    Double_t geocartposition[3]={geometrydata[loopnumber][1]*cos(newsphaddition[2]),geometrydata[loopnumber][1]*sin(newsphaddition[2]),geometrydata[loopnumber][2]}; 
    Double_t dr=sqrt(cartposition[0]*cartposition[0]+cartposition[1]*cartposition[1])-sqrt(geocartposition[0]*geocartposition[0]+geocartposition[1]*geocartposition[1]);

    Double_t drsphchange[3]={dr,PI/2-2*newsphaddition[1],0};
    SetNewSphAddition(drsphchange);
    newvolumereached=true;
}

bool SetupGeometry::GetNewVolumeReached() {
  return (newvolumereached);
}


// void SetupGeometry::SetPhotonPosition(Double_t new_cartposition[]) { //sets new cart position
//   cartposition[0]=new_cartposition[0];
//   cartposition[1]=new_cartposition[1];
//   cartposition[2]=new_cartposition[2];
// }

// Double_t SetupGeometry::GetPhotonPosition() { //prints current cart position
//   cout<<"Cart Position: "<<cartposition[0]<<","<<cartposition[1]<<","<<cartposition[2]<<endl;
//   return 0;
// }

// Double_t SetupGeometry::GetNewSphAddition() { //prints current sph addition data
//   //just prints the position, do I actually need to return a position?
//   cout<<"Newsphaddition:"<<newsphaddition[0]<<","<<newsphaddition[1]<<","<<newsphaddition[2]<<endl;
//   return 0;
// }


void SetupGeometry::VolumeChecker(Int_t loopnumber) {
  bool within;
  Int_t newvolumenumber;
  for (int n=0;n<3;n++) {
    Double_t geocartposition[3]={geometrydata[n][1]*cos(newsphaddition[2]),geometrydata[n][1]*sin(newsphaddition[2]),geometrydata[n][2]};
    for (int i=0;i<2;i++) {
      if (cartposition[i]<geocartposition[i]) {
	within=true;
      }
      else {
	within=false;
      }
    }
    if (within=false) {
      //then need to move on to the next volume!
    }
    if (within=true) {
      newvolumenumber=n;
    }
  }
  if (within=true) {
    if (newvolumenumber!=volume_number) {
      SetExitCartPosition(loopnumber);
      SetVolumeNumber(newvolumenumber);
    }
    else {
      //then the volume number and cart position stay the same!
    }
  }
  else if (within=false) {
    if (loopnumber==0) {
      SetExitCartPosition(loopnumber);
      SetVolumeNumber(1);
    }
    else {
      SetVolumeNumber(5);
    }
  }
}
