#include "Photon.h"

using namespace std;

int main() {

  Double_t result_slick;
  Double_t result_SF;
  Double_t cross_section;
  TH1D *hist_slick=new TH1D("hist","Slick MC class method",100,0,100);
  TH1D *hist_SF=new TH1D("hist1","Straight Forward class method",51,0,51);
  cout<< "cross section: ";
  cin>> cross_section;
  Photon photon;
  photon.SetCrossSection(cross_section);
  //  cout<<photon.PhotonStepperSlick()<<endl;
  for (int n=0;n<100000;n++) {
    result_slick=photon.PhotonStepperSlick();
    result_SF=photon.PhotonStepperSF();
    hist_SF->Fill(result_SF);
    hist_slick->Fill(result_slick);
  }
//graphing
  TCanvas *blah_slick=new TCanvas("Slick","Slick");
  hist_slick->GetXaxis()->SetTitle("Path Length");
  hist_slick->GetXaxis()->CenterTitle();
  hist_slick->GetYaxis()->SetTitle("Number of Photons");
  hist_slick->GetYaxis()->CenterTitle();
  hist_slick->Draw();
  blah_slick->SaveAs("slick_test_class.png");

  TCanvas *blah_SF=new TCanvas("SF","SF");
  hist_SF->GetXaxis()->SetTitle("Path Length");
  hist_SF->GetXaxis()->CenterTitle();
  hist_SF->GetYaxis()->SetTitle("Number of Photons");
  hist_SF->GetYaxis()->CenterTitle();
  hist_SF->Draw();
  blah_SF->SaveAs("SF_test_class.png");  

  return 0;
}    
