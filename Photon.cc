#include "Photon.h"

//static variables
Double_t Photon::N_a=1;
Double_t Photon::dx=0.1;
Double_t Photon::detector_length=50;

Photon::Photon() { //constructor for default photon object
  cross_section=0.1;
}

Photon::Photon(Double_t new_cross_section) { //constructor for a photon object
  cross_section = new_cross_section;
}

int Photon::PhotonStepperSlick() { //slick photon depth method
  anything.SetSeed(0); //creates the seed
  random_number=anything.Rndm(); //creates random number
  return ((-1/cross_section)*log(1-random_number));
//returns distance traveled before interaction.
}

int Photon::PhotonStepperSF() { //Straight forward photon depth method
  anything.SetSeed(0); //creates the seed
  Prob=N_a*cross_section*dx; //finds the general probablity that there is an interaction
  for (int n=0;n<detector_length/dx;n++) { //sees how far the photon goes by testing at each slice
    random_number=anything.Rndm();
    if (random_number<Prob) { //if interaction occurs, will return the distance travelled. 
      return (n*dx);
    }
  }
  return(50); //otherwise will return a slice beyond the detector
}

int Photon::GetCrossSection() {
  return (cross_section);
}

int Photon::SetCrossSection(Double_t new_cross_section) {
  cross_section=new_cross_section;
}

