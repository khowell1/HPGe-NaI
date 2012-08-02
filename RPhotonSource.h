/*********************************************************************
 Photon source
 Simple class that generates photon position and direction
 It allows for multiple monoenergetic photons and a 
 continuous spectrum.

 Currently only acts as a point source, will upgrade to allow
 for solid shape sources eventually.

 To create any number of line sources, simply call 
 AddLine(energy, relProb)
 for each one, where relProb is the "relative amount".  For 
 example, if you want a line at 660 and a line at 220, with 660
 producing 10 times as many photons as 220, call
 AddLine(660, 10)
 AddLine(220, 1)

 To add a continuous background, generate a TF1 with the 
 appropriate functional shape (over the correct range - 
 internally, it calls TF1::GetRandom()).  To add it, call
 AddContinuous(functionTF1, relProb)
 this relProb is the same as above, 
 so to get 10:1:2 ratio of photons for 660 to 220 to continuous, call
 AddContinuous(myTF1, 2)
 after calling AddLine as above

 You must call Initialize() before generating photons (this converts
 relProb to actual probabilities)

 To get photons, just call GetPhoton.  It returns the photon energy, 
 and will overwrite the contents of any vector or array passed to it
 with the appropriate output direction and position, as 
 3-element arrays (or vectors) in cartesian coordinates (normalized to
 have length 1)
 
 Alternatively, call GeneratePhoton.  This will return the energy and
 update internal data.  To get the photon direction, us GetPhi, GetTheta 
 or GetDirectionX, GetDirectionY, GetDirectionZ  (the former is 
 spherical Theta and Phi, the latter is the same normalized cartesian
 vector from GetPhoton).  Note that calling GetPhoton will *not* update
 this internal array

**********************************************************************/

#include <vector>
#include <string>
#include "TROOT.h"
#include "TF1.h"
#include "TRandom3.h"
#include "TRandom.h"
#include "TMath.h"
using namespace std;

#ifndef RPHOTONSOURCE_H
#define RPHOTONSOURCE_H

class RPhotonSource {

 private:
  Bool_t setupOK;
  vector<Double_t> lineEnergies;
  vector<Double_t> lineRelProbs;
  vector<Double_t> lineProbs;
  TF1 *continuous;
  Double_t contRelProb;
  Double_t contProb;
  Double_t generatedPhotonEnergy;
  Double_t positionArr[3];
  Double_t directionArr[3];
  Double_t sourceCenterPosition[3];
  Double_t generatedPhotonTheta;
  Double_t generatedPhotonPhi;
  
 public:
  RPhotonSource();
  ~RPhotonSource();
  Bool_t InitializeNoSetSeed();
  Bool_t Initialize(UInt_t setSeed = 0);
  void ClearPhotons();
  Int_t AddLine(Double_t energy, Double_t relProb=1);
  Int_t AddContinuous(TF1 *spectrum, Double_t relProb=1);
  Double_t GetPhoton();
  Double_t GetPhoton(vector<Double_t>& direction);
  Double_t GetPhoton(vector<Double_t>& direction, vector<Double_t>& position);
  Double_t GetPhoton(Double_t direction[]);
  Double_t GetPhoton(Double_t direction[], Double_t position[]);
  void SetSourcePosition(vector<Double_t> position);
  void SetSourcePosition(Double_t position[]);
  void GetSourcePosition(vector<Double_t> position);
  void GetSourcePosition(Double_t position[]);
  vector<Double_t> GetSourcePosition();
  Double_t GeneratePhoton();
  Double_t GetEnergy() {return generatedPhotonEnergy;}
  Double_t GetPhi() {return generatedPhotonPhi;}
  Double_t GetTheta() {return generatedPhotonTheta;}
  Double_t GetDirectionX() {return directionArr[0];}
  Double_t GetDirectionY() {return directionArr[1];}
  Double_t GetDirectionZ() {return directionArr[2];}
  Bool_t IsSetupOK() { return setupOK; }
  void Print();

};

#endif
