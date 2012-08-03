//all-commanding master that will control everything!
#include "Master.h"

Double_t Master::sourceposition[3]={0.0,0.0,10.0};
Double_t Master::gecylgeometry[3]={0,4.11,6.53};
Double_t Master::spacecylgeometry[3]={1,4.20,6.53};
Double_t Master::naicylgeometry[3]={2,5.00,6.53};
Double_t Master::initialphotonenergy=1000; //keV

Master::Master() {
  photonsource= new RPhotonSource();
  photon=new Photon();
  geometry=new SetupGeometry();
  //eventually set seeds and such within here that arent already done
}

Master::~Master() {
  delete photonsource;
  delete photon;
  delete geometry;
}

Double_t Master::Initializer() { //gets a photon too... probably should do this in walker!
  //need to figure out how to deal with time stamps...
  //dealing with the photon source,reading the coefficient files, setting geometries up
  initialphotonenergy=initialphotonenergy/1000; //get to get MeV! 
  photonsource->SetSourcePosition(sourceposition);
  photonsource->AddLine(initialphotonenergy);
  photonsource->Initialize();
  photonsource->Print();
  //deal with photon initializing
  photon->FileReader();
  photon->SetSplineMu();
  //deal with geometry initializing
  geometry->SetGeometryData(gecylgeometry);
  geometry->SetGeometryData(spacecylgeometry);
  geometry->SetGeometryData(naicylgeometry);
  return 0;
}

void Master::Walk1Photon() {
  photon->SetPhotonEnergy(photonsource->GetPhoton());
  geometry->SetPhotonPosition(sourceposition);
  //  Double_t fakeenergy=photonsource->GetPhoton();  
  //  Double_t fakeposition[3]=photonsource->GetPhoton(Double_t sourceposition[3]);
  bool result=false;
  while (result=false) {
    //need to fill that position array to give to geometry!
    Double_t travel_length=photon->PhotonStepperSlick();
    Double_t new_phi=photon->PhiFinder(Double_t fakeenergy);
    Double_t new_theta=photon->ThetaFinder();
    typeinteraction=photon->InteractionFinder(fakeenergy);
    // if (typeinteraction==1) {
    //   //pair production... figure out what to do later!
    // }
    if (typeinteraction==2) {
      //compton scattering!
      Double_t newphotonenergy=photon->ComptonEnergyCalc(Double_t fakeenergy,Double_t new_theta);
      photon->SetPhotonEnergy(newphotonenergy);
    }
    else if (typeinteraction==3) {
      //photo
      //have it so that everything is completed and necessary info saved. 
      result=true;
      return (result);
      //could just put a break in too... either way works, but break has less lines!
    }
  }
}
