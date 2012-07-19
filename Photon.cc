#include "Photon.h"

//static variables
Double_t Photon::n_a=0.1;
Double_t Photon::dx=0.1; //cm
TRandom3 anything; //for getting a random number


Photon::Photon() { //constructor for default photon object
  mu=0.1; //1/cm
  detector_length=10; //cm
  anything.SetSeed(0); //creates the seed
}

Photon::Photon(Double_t new_mu,Double_t new_detector_length) {//constructor for a photon object
  mu = new_mu;
  detector_length = new_detector_length;
  anything.SetSeed(0);
}

int Photon::PhotonStepperSlick() { //slick photon depth method
  random_number=anything.Rndm(); //creates random number
  return ((-1/mu)*log(1-random_number)); //actually using mu here... 
//returns distance traveled before interaction.
}

int Photon::PhotonStepperSF() { //Straight forward photon depth method

  prob=n_a*mu*dx; //finds the general probablity that there is an interaction
  for (int n=0;n<=detector_length/dx;n++) { //sees how far the photon goes by testing at each slice
    random_number=anything.Rndm();
    if (random_number<prob) { //if interaction occurs, will return the distance travelled. 
      return (n*dx);
    }
  }
  return(detector_length+1); //otherwise will return a slice beyond the detector
}

int Photon::GetMu() {
  return (mu);
}

int Photon::SetMu(Double_t new_mu) {
  mu=new_mu;
}
int Photon::SetDetectorLength(Double_t new_detector_length) {
  detector_length= new_detector_length;
}

int Photon::GetDetectorLength() {
  return (detector_length);
}

int Photon::Looper() {

}
