//objects, set and get methods
#include "Photon.h"
#include "TMath.h"

//static variables
Double_t Photon::PI=3.141592653589793238462;
Double_t Photon::c=2.9979245800e8; //m/s
Double_t Photon::electron_mass=9.10938188e-31; //kg
Double_t Photon::MeV_Jules_convert=1.602e-13; //J
TRandom3 Photon::anything;

Photon::Photon() { 
  //constructor for default photon object, creates a vacuum photon object
  thetafinder= new TF1("theta","((1/(1+([0]/511)*(1-x)))^2)*((1/(1+([0]/511)*(1-x)))+(1+([0]/511)*(1-x))-(1-x^2))",-1,1);
  mu_spline = NULL;
  coherent_spline = NULL;
  incoherent_spline = NULL;
  photoelec_spline = NULL;
  pairprod_spline = NULL;
  fixedMu = true;
  photon_energy=0.5; //MeV
  anything.SetSeed(0); //creates the seed
  gRandom->SetSeed(0); //for some other random thing
  density=10e-6;
  SetMu(10e-6);
}

Photon::Photon(Double_t new_photon_energy,string filename,
	       Double_t density_new) {//constructor for a photon object
  thetafinder= new TF1("theta","((1/(1+([0]/511)*(1-x)))^2)*((1/(1+([0]/511)*(1-x)))+(1+([0]/511)*(1-x))-(1-x^2))",-1,1);
  mu_spline = NULL;
  coherent_spline = NULL;
  incoherent_spline = NULL;
  photoelec_spline = NULL;
  pairprod_spline = NULL;
  fixedMu = false;
  photon_energy = new_photon_energy;
  anything.SetSeed(0);
  gRandom->SetSeed(0);
  density = density_new;
  FileReader(filename);

}

Photon::Photon(Double_t new_photon_energy,Double_t new_mu,
	       Double_t density_new) {//constructor for a photon object
  thetafinder= new TF1("theta","((1/(1+([0]/511)*(1-x)))^2)*((1/(1+([0]/511)*(1-x)))+(1+([0]/511)*(1-x))-(1-x^2))",-1,1);
  mu_spline = NULL;
  coherent_spline = NULL;
  incoherent_spline = NULL;
  photoelec_spline = NULL;
  pairprod_spline = NULL;
  fixedMu = true;
  photon_energy = new_photon_energy;
  anything.SetSeed(0);
  gRandom->SetSeed(0);
  SetMu(new_mu);
  density = density_new;
}

Photon::~Photon() { //deconstructor
  if(mu_spline != NULL)
    delete mu_spline;
  if(coherent_spline != NULL)
    delete coherent_spline;
  if(incoherent_spline != NULL)
    delete incoherent_spline;
  if(photoelec_spline != NULL)
    delete photoelec_spline;
  if(pairprod_spline != NULL)
    delete pairprod_spline;
  delete thetafinder;
}

void Photon::SetMu(Double_t new_mu) { //sets mu
  cout<<"doing setmu method!"<<endl;
  mu=new_mu;
}

void Photon::SetSplineMu() { //finds energy_dependent mu from spline, calls spline mu
  cout<<"trying setsplinemu method!"<<endl;
  mu=density*mu_spline->EvalSafe(photon_energy);
  cout << "mu = " << mu << ", density= " << density << endl;
  SetMu(mu);
}

Double_t Photon::GetMu() { //returns mu
  return (mu);
}

void Photon::SetPhotonEnergy(Double_t new_photon_energy) { //sets photon energy
  photon_energy= new_photon_energy;
}

Double_t Photon::GetPhotonEnergy() { //returns photon energy
  return (photon_energy);
}

Double_t Photon::PhotonStepperSlick() { //slick photon depth method, simply gives path length
  Double_t random_number=anything.Rndm();
  return ((-1/mu)*log(1-random_number)); 
}

Int_t Photon::InteractionFinder() {
  //If using fixed mu, always return 0
  if(fixedMu)
    return 0;

//uses spline mu's to calc probabilities and then randomly return a type of interaction
  //cout << "Density: " << density << ", photo: " << photoelec_spline->Eval(photon_energy) << ", energy: " << photon_energy << endl;
  Double_t photo_mu=density*photoelec_spline->Eval(photon_energy);
  //  Double_t coherent_mu=density*coherent_spline->Eval(photon_energy);
  Double_t incoherent_mu=density*incoherent_spline->Eval(photon_energy);
  //  Double_t pair_mu=density*pairprod_spline->Eval(photon_energy);
  Double_t total_mu=photo_mu+incoherent_mu;
  Double_t random_num=anything.Rndm();
  //cout << incoherent_mu << ", " << photo_mu << ", " << total_mu << endl;
  if (random_num<((incoherent_mu)/total_mu)) {
    return(1);
  }
  else if (random_num<((incoherent_mu+photo_mu)/total_mu)) {
    return(2);
  }
  return 0;
}

Double_t Photon::ThetaFinder() {//finds differential random theta
  thetafinder->SetParameter(0,photon_energy*1000);
  theta=TMath::ACos(thetafinder->GetRandom()); //could just make this one line...
  return (theta); //gives theta (not cos theta!) in radians!
}

Double_t Photon::PhiFinder() {   //will choose from 2pi!
  Double_t random=anything.Rndm();
  phi = random*2.0*PI;
  return (phi);   //returns phi in radians
}

Double_t Photon::ComptonEnergyCalc() {//calcs new compton scattering energy
  Double_t new_photon_energy=photon_energy/(1 + (photon_energy/0.511)*(1-TMath::Cos(theta)));

  SetPhotonEnergy(new_photon_energy);
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


  if (filename.find("txt")!=string::npos) {
    cout<<"filename is a file."<<endl;
    ifstream myfile(filename.c_str());
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
    //  pairprod_spline= new RLinearInterpolant(energy_vector,pairprodnucl_vector);
    fixedMu = false;
    SetSplineMu();
  }
  else {
    cout<<"filename is a double: "<<filename<<endl;
    fixedMu = true;
    SetMu(Double_t(atof(filename.c_str())));
  }
}


