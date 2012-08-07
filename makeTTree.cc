#include <iostream>
#include <string>
#include "TTree.h"
#include "TFile.h"
#include "TRandom3.h"
using namespace std;

int main(int argc, char *argv[]) {

  string filename = "blah.root";

  if(argc == 1)
    cout << "Defaulting to filename blah.root" << endl;
  else
    filename = argv[1];

  //Make a root file filled with random number draws from
  //an flat distribution, an exponential, and a gaussian
  TRandom3 rand;
  rand.SetSeed(0);

  //Important: you have to create the file first, before the TTree,
  //or root will not save it properly
  //Also, "RECREATE" tells it to create a new file
  TFile *outFile = new TFile(filename.c_str(),"RECREATE");

  TTree *outTree = new TTree("Randoms","Randoms");

  Double_t expo, gaussian, flat;
  Int_t event;

  //Note: the variable names can be anything, but the first quoted entry
  //(the "name") and the last one (the "title") should be the same or it will
  //be confusing, as some parts of root look at names and others at titles
  //The thing after the name (/D here) specifies the variable type:
  //  /D is Double_t, you can look the others up in the TTree documentation
  outTree->Branch("exp", &expo, "exp/D");
  outTree->Branch("gaus", &gaussian, "gaus/D");
  outTree->Branch("flat", flat, "flat[3]/D");
  outTree->Branch("eventNum",&event,"eventNum/I");
  
  for(event = 0; event < 10000; event++) {
    //Generate randoms
    gaussian = rand.Gaus(0,0.5); //mean, sigma
    expo = rand.Exp(1); //tau
    flat = rand.Rndm();
    //event is set by loop
    outTree->Fill(); //This reads in all the associated variables
  }

  outTree->Write();

  delete outTree;
  outFile->Close();
  delete outFile;

  return 0;

}
