//objects, set and get methods
#include "Photon.h"

//static variables
Double_t Photon::n_a=1.0; //number of interactive objects/volume
Double_t Photon::dx=0.01; //cm
Double_t Photon::number=100000; //loop number
Double_t Photon::PI=3.141592653589793238462;
Double_t Photon::c=2.9979245800e8; //m/s
Double_t Photon::electron_mass=9.10938188e-31; //kg
Double_t Photon::MeV_Jules_convert=1.602e-13; //J
TRandom3 anything; //for getting a random number


Photon::Photon() { //constructor for default photon object
  //make the mu so that it uses the interpolation value now!!!!!
  mu=0.1; //1/cm
  //  detector_length=10; //cm
  photon_energy=500; //keV
  photonproperties[0]=photon_energy;
  anything.SetSeed(0); //creates the seed
  gRandom->SetSeed(0); //for some other random thing
}

Photon::Photon(Double_t new_mu,Double_t new_photon_energy) {//constructor for a photon object
  mu = new_mu;//same here!!!! or have another object constructor that you can change mu with?
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
}

void Photon::SetMu(Double_t new_mu) {
  mu=new_mu;
}

void Photon::SetSplineMu(/*Double_t photon_energy*/) {
  mu=mu_spline->Eval(photon_energy);
  SetMu(mu);
}

Double_t Photon::GetMu() {
  cout<<mu<<endl;
  return (mu);
}

void Photon::SetPhotonEnergy(Double_t new_photon_energy) {
  photon_energy= new_photon_energy;
}

Double_t Photon::GetPhotonEnergy() {
  cout<<photon_energy<<endl;
  return (photon_energy);
}

void Photon::SetPhotonProperties() {
  //[photon energy,distance,theta,phi]
  photonproperties[0]=photon_energy;
  photonproperties[1]=PhotonStepperSlick();
  photonproperties[2]=ThetaFinder(); //or just use new theta? later when geometry is set up
  photonproperties[3]=PhiFinder();
}

Double_t Photon::GetPhotonProperties() {
  cout<<"Photon Energy(MeV): "<<photonproperties[0]<<endl;
  cout<<"Photon Distance(cm): "<<photonproperties[1]<<endl;
  cout<<"Theta(rad): "<<photonproperties[2]<<endl;
  cout<<"Phi(rad): "<<photonproperties[3]<<endl;
  return 0;
}

void Photon::FileReader() { //eventually need to make it take any file... but static for now!
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
  int vector_size;

  ifstream myfile("Ge_cross_notitles.txt");
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
  //redefining the arrays' size
  vector_size=energy_vector.size();

  //splines!!!
  mu_spline= new RLinearInterpolant(energy_vector,cross_vector);
  //  coherent_spline= new RLinearInterpolant(energy_vector,coherent_scat_vector);
  incoherent_spline= new RLinearInterpolant(energy_vector,incoherent_scat_vector);
  photoelec_spline= new RLinearInterpolant(energy_vector,photoelectric_vector);
  pairprod_spline= new RLinearInterpolant(energy_vector,pairprodnucl_vector);
}

Double_t Photon::PhotonStepperSlick() { //slick photon depth method, simply gives path length
  Double_t random_number=anything.Rndm(); 
  return ((-1/mu)*log(1-random_number)); 
}

Double_t Photon::InteractionFinder() {
  Double_t photo_mu=photoelec_spline->Eval(photon_energy);
  //  Double_t coherent_mu=coherent_spline->Eval(photon_energy);
  Double_t incoherent_mu=incoherent_spline->Eval(photon_energy);
  Double_t pair_mu=pairprod_spline->Eval(photon_energy);
  Double_t total_mu=photo_mu+incoherent_mu+pair_mu;
  Double_t random_num=anything.Rndm();
  if (random_num<(pair_mu/total_mu))  {
    return(2); //pretend no pair production, just compton and photo! so this is compton for now!
  }
  else if (random_num<((pair_mu+incoherent_mu)/total_mu)) {
    return(2);
  }
  else if (random_num<((pair_mu+incoherent_mu+photo_mu)/total_mu)) {
    return(3);
  }
  return 0;
}

Double_t Photon::ThetaFinder() {
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

// void Photon::ComptonEnergyCalc(Double_t photon_energy,Double_t theta,Double_t PI,Double_t keV_Jules_convert) {
//   Double_t new_photon_energy=((photon_energy*MeV_Jules_convert)/(1+(photon_energy*MeV_Jules_convert)/(electron_mass*c*c))(1-cos(theta)));
// }


