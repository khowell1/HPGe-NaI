#include "Photon.h"

using namespace std;

int main() {

  Double_t result_slick;
  Double_t result_SF;
  Double_t mu;
  Double_t detector_length;
  Double_t sum_slick_escape=0;
  Double_t sumsq_slick_escape=0;
  Double_t sum_SF_escape=0;
  Double_t sumsq_SF_escape=0;
  Double_t std_slick_escape;
  Double_t std_SF_escape;
  Double_t slick_BD=0;
  Double_t SF_BD=0;
  Double_t SF_escape_percent,slick_escape_percent;
  Double_t number=100000; //number of loops

  TH1D *hist_slick=new TH1D("hist","Slick MC class method",100,0,100);
  TH1D *hist_SF=new TH1D("hist1","Straight Forward class method",51,0,51);
  cout<< "mu: ";
  cin>> mu;
  cout<<"detector length: ";
  cin>>detector_length;
  Photon photon;
  photon.SetMu(mu);
  photon.SetDetectorLength(detector_length);
  for (int n=0;n<number;n++) {
    result_slick=photon.PhotonStepperSlick();
    result_SF=photon.PhotonStepperSF();
    hist_SF->Fill(result_SF);
    hist_slick->Fill(result_slick);
    if (result_SF>photon.GetDetectorLength()) { //seeing if the photon escapes, both if statements for both methods
      SF_BD++;
      sum_SF_escape=sum_SF_escape+result_SF; //finding the sum, both for sf method, for finding escaped photons
      sumsq_SF_escape=sumsq_SF_escape+result_SF*result_SF; //finding the sum squared
    }
    if (result_slick>photon.GetDetectorLength()) {
      slick_BD++;
      sum_slick_escape=sum_slick_escape+result_slick; //finding the sum, both for slick method, for finding escaped photons
      sumsq_slick_escape=sumsq_slick_escape+result_slick*result_slick; //finding the sum squared
    }
  }
  std_slick_escape=sqrt(abs((sumsq_slick_escape/number)-(sum_slick_escape/number)*(sum_slick_escape/number))); 
  std_SF_escape=sqrt(abs((sumsq_SF_escape/number)-(sum_SF_escape/number)*(sum_SF_escape/number)));
  slick_escape_percent=100.0*slick_BD/number;
  SF_escape_percent=100.0*SF_BD/number;

  cout<<"prob of slick escape: "<<slick_escape_percent<<"+/-"<<std_slick_escape<<endl;
  cout<<"prob of SF escape: "<<SF_escape_percent<<"+/-"<<std_SF_escape<<endl;
  //   //graphing
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


  // Double_t sum_slick=0;
  // Double_t sum_SF=0;
  // Double_t sumsq_slick=0;
  // Double_t sumsq_SF=0;
  // Double_t std_slick=0;
  // Double_t std_SF=0;

    // sum_slick=sum_slick+result_slick; //finding the sum, both for slick method
    // sumsq_slick=sumsq_slick+result_slick*result_slick; //finding the sum squared

  // std_slick=sqrt(abs((sumsq_slick/number)-(sum_slick/number)*(sum_slick/number)));   
  // std_SF=sqrt(abs((sumsq_SF/number)-(sum_SF/number)*(sum_SF/number)));
//  cout<<"STD (slick,SF): "<<std_slick<<","<<std_SF<<endl;
