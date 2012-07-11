#include "PHOTON.h"

int PHOTON::PhotonStepper(Double_t) {
  /*Will hold the bulk of the functions for walking the photon through the detector. Both straight forward and slick methods will be in here!*/
  
//lets do slick first!
  TRandom3 anything; //for getting a random number
  anything.SetSeed(0); //creates the seed
  Float_t random_number=anything.Rndm();
  return ((-1/cross_section)*log(1-random_number));
//returns an x value =distance traveled before interaction.

//now the straight forward way!
  Double_t N_a=1;
  Double_t dx=0.1;
  TRandom3 anything; //for getting a random number
  anything.SetSeed(0); //creates the seed
  Float_t Prob,random_number;
  Prob=N_a*cross_section*dx;
  for (int n=0;n<number;n++) { //sees how far the photon goes, static prob
    random_number=anything.Rndm();
    if (random_number<Prob) { //interaction occurs
      return (n);
    }
  }
  return(number+1); 



}

int PHOTON::GetCrossSection(Double_t cross_section) {
  cout<<"Cross Section: ";
  cin<<cross_section;
