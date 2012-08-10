//objects, set and get methods
#include "Photon.h"

//static variables
Double_t Photon::PI=3.141592653589793238462;
Double_t Photon::c=2.9979245800e8; //m/s
Double_t Photon::electron_mass=9.10938188e-31; //kg
Double_t Photon::MeV_Jules_convert=1.602e-13; //J
Double_t Photon::densityarray[4]={10e-16,5.323,10e-16,3.67}; //g/cm^3;vacuum,ge,space,nai
TRandom3 anything; //for getting a random number

Photon::Photon() { //constructor for default photon object
  //make the mu so that it uses the interpolation value now!!!!!
  //mu=0.1; //1/cm
  //detector_length=10; //cm
  photon_energy=500; //keV
  anything.SetSeed(0); //creates the seed
  gRandom->SetSeed(0); //for some other random thing
  filename="Ge_cross_notitles.txt"; //filername will be declared in header
  FileReader(filename);
}

Photon::Photon(Double_t new_photon_energy,string filename) {//constructor for a photon object
  //mu = new_mu;//same here!!!! or have another object constructor that you can change mu with?
  photon_energy = new_photon_energy;
  anything.SetSeed(0);
  gRandom->SetSeed(0);
  FileReader(filename);
  SetSplineMu(0); //sets mu for volume number 0
}

Photon::Photon(Double_t new_photon_energy,Double_t new_mu) {//constructor for a photon object
  //mu = new_mu;//same here!!!! or have another object constructor that you can change mu with?
  photon_energy = new_photon_energy;
  anything.SetSeed(0);
  gRandom->SetSeed(0);
  SetMu(new_mu);
}

Photon::~Photon() { //deconstructor
  delete mu_spline;
  //delete coherent_spline;
  delete incoherent_spline;
  delete photoelec_spline;
  delete pairprod_spline;
  delete thetafinder;
}

void Photon::SetMu(Double_t new_mu) { //sets mu
  mu=new_mu;
}

void Photon::SetSplineMu(Int_t volumenumber) { //finds energy_dependent mu from spline, calls spline mu
  mu=densityarray[volumenumber]*mu_spline->Eval(photon_energy);
  SetMu(mu);
}

Double_t Photon::GetMu() { //prints and returns mu
  cout<<mu<<endl;
  return (mu);
}

void Photon::SetPhotonEnergy(Double_t new_photon_energy) { //sets photon energy
  photon_energy= new_photon_energy;
}

Double_t Photon::GetPhotonEnergy() { //prints and returns photon energy
  //  cout<<photon_energy<<endl;
  return (photon_energy);
}

Double_t Photon::PhotonStepperSlick() { //slick photon depth method, simply gives path length
  Double_t random_number=anything.Rndm(); 
  return ((-1/mu)*log(1-random_number)); 
}

Double_t Photon::InteractionFinder() {
//uses spline mu's to calc probabilities and then randomly return a type of interaction
  Double_t photo_mu=photoelec_spline->Eval(photon_energy);
  //  Double_t coherent_mu=coherent_spline->Eval(photon_energy);
  Double_t incoherent_mu=incoherent_spline->Eval(photon_energy);
  //  Double_t pair_mu=pairprod_spline->Eval(photon_energy);
  Double_t total_mu=photo_mu+incoherent_mu;
  Double_t random_num=anything.Rndm();
  if (random_num<((incoherent_mu)/total_mu)) {
    return(1);
  }
  else if (random_num<((incoherent_mu+photo_mu)/total_mu)) {
    return(2);
  }
  return 0;
}

Double_t Photon::ThetaFinder() {//finds differential random theta
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

Double_t Photon::ComptonEnergyCalc() {//calcs new compton scattering energy
  Double_t new_photon_energy=((photon_energy*MeV_Jules_convert)/(1+(photon_energy*MeV_Jules_convert)/(electron_mass*c*c))*(1-cos(theta)));
  new_photon_energy=new_photon_energy*1/MeV_Jules_convert;
  return (new_photon_energy);
}


void Photon::FileReader(string filename) { 
  vector<Double_t> energy_vector;
  vector<Double_t> coherent_scat_vector;
  vector<Double_t> incoherent_scat_vector;
  vector<Double_t> photoelectric_vector;
  vector<Double_t> pairprodnucl_vector;
  vector<Double_t> pairprodelec_vector;
  vector<Double_t> cross_vector;
  vector<Double_t> cross_nocoher_vector;
  Double_t temp_energy;
  Double_t temp_coherent;
  Double_t temp_incoherent;
  Double_t temp_photo;
  Double_t temp_pairnucl;
  Double_t temp_pairelec;
  Double_t temp_cross;
  Double_t temp_cross_nocoh;  


  ifstream myfile(filename);
  while (myfile.good()) { //reading in the values from the file into the vectors
    myfile>>temp_energy>>temp_coherent>>temp_incoherent>>temp_photo>>temp_pairnucl>>temp_pairelec>>temp_cross>>temp_cross_nocoh;
    energy_vector.push_back(temp_energy);
    coherent_scat_vector.push_back(temp_coherent);
    incoherent_scat_vector.push_back(temp_incoherent);
    photoelectric_vector.push_back(temp_photo);
    pairprodnucl_vector.push_back(temp_pairnucl);
    pairprodelec_vector.push_back(temp_pairelec);
    cross_vector.push_back(temp_cross);
    cross_nocoher_vector.push_back(temp_cross_nocoh);
  }    
  myfile.close();

  //splines!!!
  mu_spline= new RLinearInterpolant(energy_vector,cross_vector);
  //  coherent_spline= new RLinearInterpolant(energy_vector,coherent_scat_vector);
  incoherent_spline= new RLinearInterpolant(energy_vector,incoherent_scat_vector);
  photoelec_spline= new RLinearInterpolant(energy_vector,photoelectric_vector);
  pairprod_spline= new RLinearInterpolant(energy_vector,pairprodnucl_vector);
}


