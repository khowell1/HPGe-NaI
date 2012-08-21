#include <iostream>
#include <string>
#include "Master.h"
using namespace std;

int main() {

  Master *myMaster = new Master;

  myMaster->Initializer(); //Default settings for now
  cout << "Initialized!\n";
  cout.flush();
  Double_t nEvents = 100000;
  for(int i=0; i < nEvents; i++) {
    if(i%5000 == 0)
      cout << "Done " << i << " of " << nEvents << endl;
    myMaster->Walk1Photon();

  }
  cout << "Walked!\n";
  cout.flush();
  myMaster->Finisher();
  cout << "Finished\n";
  cout.flush();

  return 0;

}
