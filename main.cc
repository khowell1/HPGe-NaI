//main function!
#include "Photon.h"

using namespace std;

int main() {
  Int_t response;
  Double_t result_slick;
  Double_t mu;
  Double_t detector_length;
  Double_t photon_energy;
  Double_t sum_slick_escape=0;
  Double_t sumsq_slick_escape=0;
  Double_t std_slick_escape;
  Double_t slick_BD=0;
  Double_t slick_escape_percent;
  //  Double_t number=10000000; //number of loops
  Photon photon;
  Int_t loop_ender=0;
  //  start of data creation
  photon.FileReader();
  cout<<"Direct Mu or Spline Mu? Give 1 or 2: ";
  cin>>response;
  while (loop_ender==0) {
    if (response==1) { //directly receive mu
      cout<< "mu: ";
      cin>> mu;
      cout<<"detector length: ";
      cin>>detector_length;
      cout<<"photon energy: ";
      cin>>photon_energy;
      photon.SetMu(mu);
      photon.SetDetectorLength(detector_length);
      photon_energy=photon_energy/1000.0;
      photon.SetInitialPhotonEnergy(photon_energy);
      loop_ender=1;
	}
    else if (response==2) { //use spline to get mu
      cout<<"detector length: ";
      cin>>detector_length;
      cout<<"photon energy: ";
      cin>>photon_energy;
      photon_energy=photon_energy/1000.0;
      photon.SetSplineMu(photon_energy);
      photon.SetDetectorLength(detector_length);
      photon.SetInitialPhotonEnergy(photon_energy);
      loop_ender=1;
    }
    else {
      cout<<"Direct Mu or Spline Mu? Give 1 or 2: ";
      cin>>response;
    }
  }
  photon.SetPhotonProperties(Double_t photonproperties);
  photon.GetPhotonProperties();
  //  cout<<photon.GetMu()<<endl;
  //cout<<photon.InteractionFinder(photon_energy)<<endl;
  //photon.ThetaFinder(photon_energy);
 //  TH1D *hist_slick=new TH1D("hist","Slick MC class method",100,0,detector_length*1.01);
 //  for (int n=0;n<number;n++) {
 //    result_slick=photon.PhotonStepperSlick();
 //     hist_slick->Fill(result_slick);
 //     if (result_slick>photon.GetDetectorLength()) {//slick
 //      slick_BD++;
 //      sum_slick_escape=sum_slick_escape+result_slick; //finding the sum, slick
 //      sumsq_slick_escape=sumsq_slick_escape+result_slick*result_slick; //finding the sum squared
 //    }
 //  }
 //  //std stuff for escaping...
 //  std_slick_escape=sqrt(abs((sumsq_slick_escape/number)-(sum_slick_escape/number)*(sum_slick_escape/number))); 
 //   slick_escape_percent=100.0*slick_BD/number;
 
 //  cout<<"prob of slick escape: "<<slick_escape_percent<<"+/-"<<std_slick_escape<<endl;
 
 // //graphing
 //  TCanvas *blah_slick=new TCanvas("Slick","Slick");
 //  hist_slick->GetXaxis()->SetTitle("Path Length");
 //  hist_slick->GetXaxis()->CenterTitle();
 //  hist_slick->GetYaxis()->SetTitle("Number of Photons");
 //  hist_slick->GetYaxis()->CenterTitle();
 //  hist_slick->Draw();
 //  blah_slick->SaveAs("slick_test_class.png");  

  return 0;
}
