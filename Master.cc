//all-commanding master that will control everything!
#include "Master.h"

Double_t Master::sourceposition[3]={0.0,0.0,10.0};
Double_t Master::gecylgeometry[3]={0,4.11,6.53};
Double_t Master::spacecylgeometry[3]={1,4.20,6.53};
Double_t Master::naicylgeometry[3]={2,5.00,6.53};
Double_t Master::initialphotonenergy=500; //keV but switched to MeV in initializer

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
  initialphotonenergy=initialphotonenergy/1000; //to get MeV! 
  photonsource->SetSourcePosition(sourceposition);
  photonsource->AddLine(initialphotonenergy);
  photonsource->Initialize();
  //  photonsource->Print();
  //deal with photon initializing
  photon->FileReader();
  //deal with geometry initializing
  geometry->SetGeometryData(gecylgeometry);
  geometry->SetGeometryData(spacecylgeometry);
  geometry->SetGeometryData(naicylgeometry);
  return 0;
}

Double_t Master::Walk1Photon() {
  photon->SetPhotonEnergy(photonsource->GetPhoton()); //remember in MeV
  geometry->SetPhotonPosition(sourceposition);
  photon->SetSplineMu();
  newsphaddition[1]=0;
  newsphaddition[2]=0;
  while (true) {
    //need to fill that position array to give to geometry!
    newsphaddition[0]=photon->PhotonStepperSlick();
    geometry->SetNewPhotonAddition(newsphaddition);

    Int_t typeinteraction=photon->InteractionFinder(); //find which interaction
    // if (typeinteraction==1) {
    //   //pair production... figure out what to do later!
    // }
    if (typeinteraction==2) {
      //compton scattering!
      newsphaddition[1]=photon->ThetaFinder(); //differential cross section theta!
      newsphaddition[2]=photon->PhiFinder();
      cout<<"Compton Interaction"<<endl;
      photon->SetPhotonEnergy(photon->ComptonEnergyCalc());
      if (photon->GetPhotonEnergy()<=0) {
	return (false);
      }
      geometry->SetNewPhotonAddition(newsphaddition);
      geometry->CartPositionUpdater();
      geometry->GetPhotonPosition();//cart coords
      geometry->PhotonVolumePosition();
      geometry->GetCylPosition();
      return (false);//for now...
    }
    else if (typeinteraction==3) {
      //photo
      //have it so that everything is completed and necessary info saved. 
      cout<<"Photoelectric effect"<<endl;
      return (false);
      //could just put a break in too... either way works, but break has less lines!
    }
  }
  return 0;
}
