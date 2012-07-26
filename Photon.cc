//objects, set and get methods
#include "Photon.h"

//static variables
Double_t Photon::n_a=1.0; //number of interactive objects/volume
Double_t Photon::dx=0.01; //cm
Double_t Photon::number=100000; //loop number
Double_t Photon::PI=3.141592653589793238462;
Double_t Photon::c=2.9979245800e8; //m/s
Double_t Photon::electron_mass=9.10938188e-31; //kg
Double_t Photon::MeV_Jules_convert=1.602e-13 //J
TRandom3 anything; //for getting a random number


Photon::Photon() { //constructor for default photon object
  mu=0.1; //1/cm
  detector_length=10; //cm
  photon_energy=500; //keV
  photonproperties[0]=photon_energy;
  anything.SetSeed(0); //creates the seed
  gRandom->SetSeed(0); //for some other random thing
}

Photon::Photon(Double_t new_mu,Double_t new_detector_length, Double_t new_photon_energy) {//constructor for a photon object
  mu = new_mu;
  detector_length = new_detector_length;
  photon_energy = new_photon_energy;
  photonproperties[0]=photon_energy;
  anything.SetSeed(0);
  gRandom->SetSeed(0);
}

Photon::~Photon() {
  delete mu_spline;
  delete coherent_spline;
  delete incoherent_spline;
  delete photoelec_spline;
  delete pairprod_spline;
  delete thetafinder;
  delete Photon();
  delete Photon(Double_t mu,Double_t detector_length,Double_t photon_energy);
}

void Photon::SetMu(Double_t new_mu) {
  mu=new_mu;
}

void Photon::SetSplineMu(Double_t photon_energy) {
  mu=mu_spline->Eval(photon_energy);
  SetMu(mu);
}

Double_t Photon::GetMu() {
  return (mu);
}

void Photon::SetDetectorLength(Double_t new_detector_length) {
  detector_length= new_detector_length;
}

Double_t Photon::GetDetectorLength() {
  return (detector_length);
}

void Photon::SetPhotonEnergy(Double_t new_photon_energy) {
  photon_energy= new_photon_energy;
}

Double_t Photon::GetPhotonEnergy() {
  return (photon_energy);
}

void Photon::SetPhotonProperties(Double_t photonproperties[4]) {
  //[photon energy,distance,theta,phi]
  photonproperties[0]=photon_energy;
  photonproperties[1]=PhotonStepperSlick();
  photonproperties[2]=ThetaFinder(photon_energy); //or just use new theta? later when geometry is set up
  photonproperties[3]=PhiFinder();
}

Double_t Photon::GetPhotonProperties() {
  cout<<"Photon Energy(MeV): "<<photonproperties[0]<<endl;
  cout<<"Photon Distance(cm): "<<photonproperties[1]<<endl;
  cout<<"Theta(rad): "<<photonproperties[2]<<endl;
  cout<<"Phi(rad): "<<photonproperties[3]<<endl;
  return 0;
}
