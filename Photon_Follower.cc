//all the get/set methods! 
#include "Photon.h"

Double_t Photon::PhotonStepperSlick() { //slick photon depth method, simply gives path length
  Double_t random_number=anything.Rndm(); 
  //creates random number
  return ((-1/mu)*log(1-random_number)); //actually using mu here... 
}

Double_t Photon::InteractionFinder(Double_t photon_energy) {
  Double_t photo_mu=photoelec_spline->Eval(photon_energy);
  //  Double_t coherent_mu=coherent_spline->Eval(photon_energy);
  Double_t incoherent_mu=incoherent_spline->Eval(photon_energy);
  Double_t pair_mu=pairprod_spline->Eval(photon_energy);
  Double_t total_mu=photo_mu+incoherent_mu+pair_mu;
  Double_t random_num=anything.Rndm();
  Int_t x;
  if (random_num<(pair_mu/total_mu))  {
    return(2); //pretend no pair production, just compton and photo! so this is compton for now!
  }
  else if (random_num<((pair_mu+incoherent_mu)/total_mu)) {
    return(2);
  }
  else if (random_num<((pair_mu+incoherent_mu+photo_mu)/total_mu)) {
    return(3);
  }
}

Double_t Photon::ThetaFinder(Double_t photon_energy) {
  TF1 *thetafinder= new TF1("theta","((1/(1+([0]/511)*(1-x)))^2)*((1/(1+([0]/511)*(1-x)))+(1+([0]/511)*(1-x))-(1-x^2))",-1,1);
  thetafinder->SetParameter(0,photon_energy);
  theta=acos(thetafinder->GetRandom()); //could just make this one line...
  return (theta); //gives theta (not cos theta!) in radians!
}

Double_t Photon::PhiFinder() {   //will choose from 2pi!
  Double_t random=anything.Rndm();
  phi = random*2.0*PI;
  return (phi);   //returns phi in radians
}

Double_t Photon::ComptonEnergyCalc(Double_t photon_energy,Double_t theta) {
  SetPhotonEnergy(photon_energy*MeV_Jules_convert/(1+(photon_energy*MeV_Jules_convert/(electron_mass*c*c))(1-cos(theta*180.0/PI))))
}
