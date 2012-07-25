//actual class stuff
#include "Photon.h"

//static variables
Double_t Photon::n_a=0.1; //number of interactive objects/volume
Double_t Photon::dx=0.01; //cm
Double_t Photon::number=100000;
TRandom3 anything; //for getting a random number


Photon::Photon() { //constructor for default photon object
  mu=0.1; //1/cm
  detector_length=10; //cm
  initial_photon_energy=500; //keV
  anything.SetSeed(0); //creates the seed
  gRandom->SetSeed(0);
}

Photon::Photon(Double_t new_mu,Double_t new_detector_length, Double_t new_initial_photon_energy) {//constructor for a photon object
  mu = new_mu;
  detector_length = new_detector_length;
  initial_photon_energy = new_initial_photon_energy;
  anything.SetSeed(0);
  gRandom->SetSeed(0);
}

Double_t Photon::PhotonStepperSlick() { //slick photon depth method, simply gives path length
  random_number=anything.Rndm(); //creates random number
  return ((-1/mu)*log(1-random_number)); //actually using mu here... 
}

Double_t Photon::InteractionFinder(Double_t initial_photon_energy) {
  Double_t photo_mu=photoelec_spline->Eval(initial_photon_energy);
  //  Double_t coherent_mu=coherent_spline->Eval(initial_photon_energy);
  Double_t incoherent_mu=incoherent_spline->Eval(initial_photon_energy);
  Double_t pair_mu=pairprod_spline->Eval(initial_photon_energy);
  Double_t total_mu=photo_mu+incoherent_mu+pair_mu;
  Double_t random_num=anything.Rndm();
  Int_t x;
  if (random_num<(pair_mu/total_mu))  {
    x=1;
  }
  else if (random_num<((pair_mu+incoherent_mu)/total_mu)) {
    x=2;
  }
  else if (random_num<((pair_mu+incoherent_mu+photo_mu)/total_mu)) {
    x=3;
  }
  return (x);
}

void Photon::SetMu(Double_t new_mu) {
  mu=new_mu;
}

void Photon::SetSplineMu(Double_t initial_photon_energy) {
  mu=mu_spline->Eval(initial_photon_energy);
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

void Photon::SetInitialPhotonEnergy(Double_t new_initial_photon_energy) {
  initial_photon_energy= new_initial_photon_energy;
}

Double_t Photon::GetInitialPhotonEnergy() {
  return (initial_photon_energy);
}

Double_t Photon::ThetaFinder(Double_t initial_photon_energy) {
  TF1 *thetafinder= new TF1("theta","((1/(1+([0]/511)*(1-x)))^2)*((1/(1+([0]/511)*(1-x)))+(1+([0]/511)*(1-x))-(1-x^2))",-1,1);
  thetafinder->SetParameter(0,initial_photon_energy);
  new_theta=thetafinder->GetRandom(); //could just make this one line...
  cout<<new_theta<<endl;
  return (new_theta);
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
  mu_spline= new RLinearInterpolant(energy_vector,cross_vector);
  //  coherent_spline= new RLinearInterpolant(energy_vector,coherent_scat_vector);
  incoherent_spline= new RLinearInterpolant(energy_vector,incoherent_scat_vector);
  photoelec_spline= new RLinearInterpolant(energy_vector,photoelectric_vector);
  pairprod_spline= new RLinearInterpolant(energy_vector,pairprodnucl_vector);

}
  // Double_t* energy_list;
  // Double_t* coherent_scat_list;
  // Double_t* incoherent_scat_list;
  // Double_t* photoelectric_list;
  // Double_t* pairprodnucl_list;
  // Double_t* pairprodelec_list;
  // Double_t* cross_section_list;
  // Double_t* cross_section_nocoher_list;
  // energy_list= new Double_t[vector_size];
  // coherent_scat_list = new Double_t[vector_size];
  // incoherent_scat_list = new Double_t[vector_size];
  // photoelectric_list = new Double_t[vector_size];
  // pairprodnucl_list = new Double_t[vector_size];
  // pairprodelec_list = new Double_t[vector_size];
  // cross_section_list = new Double_t[vector_size];
  // cross_section_nocoher_list = new Double_t[vector_size];
  // //for moving the data from the vectors to the arrays. 
  // for (int i=0;i<vector_size;i++) { //filling in the arrays from the vectors
  //   energy_list[i]=energy_vector[i];
  //   coherent_scat_list[i]=coherent_scat_vector[i];
  //   incoherent_scat_list[i]=incoherent_scat_vector[i];
  //   photoelectric_list[i]=photoelectric_vector[i];
  //   pairprodnucl_list[i]=pairprodnucl_vector[i];
  //   pairprodelec_list[i]=pairprodelec_vector[i];
  //   cross_section_list[i]=cross_vector[i];
  //   cross_section_nocoher_list[i]=cross_nocoher_vector[i];
  // }



//need to change list name to mu name or attentuation coefficients! but do when just bored. 


//need to get the splines for the individual cross sections so that I can calculate the probability of an interaction occuring. remember that all the different probabilities will add up to 1, and will use a random number from 0 to 1 and use the individual probability number as endpoints of ranges between 0 and 1. 

//note that phi can be linearly chosen, while theta will come from the differential cross section stuff.


// Photon::histo() {
// 	TCanvas *blah=new TCanvas("Histo","Histo");
// 	hist->GetXaxis()->SetTitle("Path Length");
// 	hist->GetXaxis()->CenterTitle();
// 	hist->GetYaxis()->SetTitle("Number of Photons");
// 	hist->GetYaxis()->CenterTitle();
// 	hist->Draw();
// 	blah->SaveAs("histo_test_class.png");
// }

// TH1D* Photon::Looper(Double_t number) {
// 	Double_t result;
// 	for (int n=0;n<number;n++) {
// 		TH1D *hist=new TH1D("hist","Slick MC class",100,0,100);
// 		result=PhotonStepperSlick();
// 		hist->Fill(result);
// 	}
// 	return hist;
// }


// Double_t Photon::PhotonStepperSF() { //Straight forward photon depth method

//   prob=n_a*mu*dx; //finds the general probablity that there is an interaction
//   for (int n=0;n<=detector_length/dx;n++) { //sees how far the photon goes by testing at each slice
//     random_number=anything.Rndm();
//     if (random_number<prob) { //if interaction occurs, will return the distance travelled. 
//       return (n*dx);
//     }
//   }
//   return(detector_length+1); //otherwise will return a slice beyond the detector
// }
