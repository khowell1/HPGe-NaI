
#include "RPhotonSource.h"

RPhotonSource::RPhotonSource() {
  gRandom->SetSeed(0);
  continuous = NULL;
  contRelProb = 0;
  contProb = 0;
  generatedPhotonEnergy = 0;
  generatedPhotonTheta = 0;
  generatedPhotonPhi = 0;
  for(int i=0; i < 3; i++) {
    positionArr[i] = 0;
    directionArr[i] = 0;
    sourceCenterPosition[i] = 0;
    fixedDirection[i] = 0;
  }
  directionType = 'i';
  setupOK = false;
}

RPhotonSource::~RPhotonSource() {
  if(continuous != NULL)
    delete continuous;
}

Bool_t RPhotonSource::InitializeNoSetSeed() {
  //Use this if you do not wish to reset the seed on gRandom

  Double_t totalRelProb = contRelProb;
  for(int i=0; i < lineRelProbs.size(); i++)
    totalRelProb += lineRelProbs[i];

  if(totalRelProb < 1e-20) {
    //All effectively 0, so nothing set
    setupOK = false;
    return false;
  }
  
  //Correct probs, know total > 0
  contProb = contRelProb / totalRelProb;
  lineProbs.clear();
  Double_t cumProb = contProb;
  for(int i=0; i < lineRelProbs.size(); i++) {
    cumProb += lineRelProbs[i]/totalRelProb;
    lineProbs.push_back(cumProb);
  }

  //Debug
  if(lineRelProbs.size() != lineEnergies.size() ||
     lineEnergies.size() != lineProbs.size()) {
    cerr << "Something went terribly, terribly wrong\n";
    setupOK = false;
    return false;
  }

  setupOK = true;
  return true;
  
}

Bool_t RPhotonSource::Initialize(UInt_t setSeed) {
  //Sets up needed internal structures and converts relative probs to
  //the correct cumulative probs
  //Call this before calling GetPhoton or GeneratePhoton !
  //Returns true if successful, false if not
  //setSeed defaults to 0, can be set as argument
  //If you don't want to reset the seed for gRandom, call InitializeNoSetSeed()

  gRandom->SetSeed(setSeed);

  return InitializeNoSetSeed();

}

void RPhotonSource::ClearPhotons() {
  //Empties the class of lines, continuous dists, etc
  //Basically resets it back to the starting state
  lineEnergies.clear();
  lineRelProbs.clear();
  lineProbs.clear();
  contRelProb = 0;
  contProb = 0;
  if(continuous != NULL)
    delete continuous;
  generatedPhotonEnergy = 0;
  generatedPhotonTheta = 0;
  generatedPhotonPhi = 0;
  for(int i=0; i < 3; i++) {
    positionArr[i] = 0;
    directionArr[i] = 0;
    sourceCenterPosition[i] = 0;
    fixedDirection[i] = 0;
  }
  directionType = 'i';
  setupOK = false;
}

Int_t RPhotonSource::AddLine(Double_t energy, Double_t relProb) {
  //Adds a monoenergetic line source.  relProb is optional
  //Defaults to "1", so multiple calls without relProb will
  //add multiple lines with the same weight
  //A line added with 0 or negative relProb is ignored
  //Repeated lines are treated as separate lines with the same energy
  //Returns current number of lines (after adding this one)

  if(relProb <= 0)
    return lineEnergies.size();

  lineEnergies.push_back(energy);
  lineRelProbs.push_back(relProb);

  setupOK = false;

  return lineEnergies.size();

}

Int_t RPhotonSource::AddContinuous(TF1 *spectrum, Double_t relProb) {
  //Adds a photon source continuous in energy, with distribution
  //given by TF1 *spectrum
  //relProb is optional, and is relative to the lines (i.e. the 
  //photon generator treats the continuous spectrum as if it were a "line"
  //that has a random energy)
  //relProb <= 0 causes setup failure
  //Returns number of lines plus itself (i.e. lines + 1), returns -1 if
  //setup fails

  //Since you're changing something
  setupOK = false;

  if(spectrum == NULL || relProb <= 0) {
    continuous = NULL;
    return -1;
  }

  continuous = dynamic_cast<TF1*>(spectrum->Clone());
  contRelProb = relProb;
  
  if(continuous->IsZombie()) {
    continuous = NULL;
    return -1;
  }

  return (lineEnergies.size()+1);

}

Double_t RPhotonSource::GetPhoton() {
  //Generates photon energy, ignores position/direction

  //Safety switch
  if(!setupOK)
    return 0;

  Double_t rand = gRandom->Rndm();
  Double_t energy = 0;
  Int_t numLines = lineEnergies.size();
  if(rand < contProb) {
    energy = continuous->GetRandom();
  } else {
    //If only a few lines, walk through one-at-a-time (faster)
    //Probably should implement binary search for larger numbers of lines (30-40ish)
    //will do later
    for(int i=numLines-1; i >= 0; i--) {
      if(rand < lineProbs[i]) {
	energy = lineEnergies[i];
      }
    }
    if(energy == 0) {
      //Somehow didn't trigger, force to last bin
      energy = lineEnergies[numLines-1];
    }
  }

  return energy;
  
}

Double_t RPhotonSource::GetPhoton(vector<Double_t>& direction) { //& means pass by reference
  //Returns the generated energy, fills vector direction with a
  //3-vector in cartesian coordinates of the output photon direction
  //with vector length 1
  
  if(direction.size() < 3) {
    direction.resize(3);
  }

  if(directionType == 'f') {
    direction[0] = fixedDirection[0];
    direction[1] = fixedDirection[1];
    direction[2] = fixedDirection[2];
  } else if(directionType == 'i') {
    //Convert from uniform in Cos(theta), this gives correct distribution
    generatedPhotonTheta = TMath::ACos(gRandom->Uniform(-1,1));
    generatedPhotonPhi = gRandom->Rndm()*2*TMath::Pi();
    
    //Set in order x,y,z
    direction[0] = TMath::Sin(generatedPhotonTheta)*
      TMath::Cos(generatedPhotonPhi);
    direction[1] = TMath::Sin(generatedPhotonTheta)*
      TMath::Sin(generatedPhotonPhi);
    direction[2] = TMath::Cos(generatedPhotonTheta);
  } else {
    //Somehow direction type not set correctly, set direction to gibberish
    direction[0] = 0;
    direction[1] = 0;
    direction[2] = 0;
  }    

  return GetPhoton();

}

Double_t RPhotonSource::GetPhoton(vector<Double_t>& direction,
				  vector<Double_t>& position) {
  //Not useful for point source, just returns center position
  //Here for upgrade to spatially distributed sources
  //Note that, since direction and position are independent, can just
  //call previous versions of GetPhoton

  if(position.size() < 3)
    position.resize(3);

  position[0] = sourceCenterPosition[0];
  position[1] = sourceCenterPosition[1];
  position[2] = sourceCenterPosition[2];
  
  return GetPhoton(direction);

}

Double_t RPhotonSource::GetPhoton(Double_t direction[]) {
  //Same as above, but outputs to first three elements of input array
  //This is dangerous!  Must have long enough array!

  if(directionType == 'f') {
    direction[0] = fixedDirection[0];
    direction[1] = fixedDirection[1];
    direction[2] = fixedDirection[2];
  } else if(directionType == 'i') {
    //Convert from uniform in Cos(theta), this gives correct distribution
    generatedPhotonTheta = TMath::ACos(gRandom->Uniform(-1,1));
    generatedPhotonPhi = gRandom->Rndm()*2*TMath::Pi();
    
    //Set in order x,y,z
    direction[0] = TMath::Sin(generatedPhotonTheta)*
      TMath::Cos(generatedPhotonPhi);
    direction[1] = TMath::Sin(generatedPhotonTheta)*
      TMath::Sin(generatedPhotonPhi);
    direction[2] = TMath::Cos(generatedPhotonTheta);
    
  } else {
    //Somehow direction type not set correctly, set direction to gibberish
    direction[0] = 0;
    direction[1] = 0;
    direction[2] = 0;
  }

  return GetPhoton(); //still returns energy


}

Double_t RPhotonSource::GetPhoton(Double_t direction[], Double_t position[]) {
  //Same as above, but outputs to first three elements of input arrays
  //This is dangerous!  Must have long enough arrays!

  position[0] = sourceCenterPosition[0];
  position[1] = sourceCenterPosition[1];
  position[2] = sourceCenterPosition[2];
  
  return GetPhoton(direction);
  
}

Double_t RPhotonSource::GeneratePhoton() {
  //Generates a photon and sets internal variables to be accessed

  if(!setupOK) {
    generatedPhotonEnergy = 0;
    return 0;
  }

  generatedPhotonEnergy = GetPhoton(directionArr, positionArr);
  return generatedPhotonEnergy;

}

void RPhotonSource::SetSourcePosition(vector<Double_t> position) {
  //Set center of source, in cartesian coordinates
  //Does nothing if vector has less than 3 elements

  if(position.size() > 2) {
    sourceCenterPosition[0] = position[0];
    sourceCenterPosition[1] = position[1];
    sourceCenterPosition[2] = position[2];
  }

  setupOK = false;

}

void RPhotonSource::SetSourcePosition(Double_t position[]) {
  //Set center of source, in cartesian coordinates
  //No check for length!  position must be at least 3 long
  
  sourceCenterPosition[0] = position[0];
  sourceCenterPosition[1] = position[1];
  sourceCenterPosition[2] = position[2];
  
  setupOK = false;

}

void RPhotonSource::GetSourcePosition(vector<Double_t> position) {
  //Copies source center position in to position.  Destroys contents of
  //position in the process
  position.resize(3);
  for(int i=0; i < 3; i++)
    position[i] = sourceCenterPosition[i];

}

void RPhotonSource::GetSourcePosition(Double_t position[]) {
  //Copies source center position into position
  //Dangerous!  Assumes position is at least length 3
  for(int i=0; i < 3; i++)
    position[i] = sourceCenterPosition[i];

}

vector<Double_t> RPhotonSource::GetSourcePosition() {
  //Creates and passes out vector<Double_t> of source center position
  //Slowest but safest method
  
  vector<Double_t> position(3);
  for(int i=0; i < 3; i++) {
    position[i] = sourceCenterPosition[i];
  }
  
  return position;

}

void RPhotonSource::SetFixedDirection(vector<Double_t> direction) {
  //Sets source to output in a fixed direction, with direction
  //given by input vector.
  //The input vector will be normalized.  If it is too small or has
  //length zero (or nearly zero) this command will do nothing
  
  if(direction.size() < 3)
    return;

  Double_t length = 0;
  for(int i=0; i < 3; i++)
    length += direction[i]*direction[i];
  
  //Ignore tiny lengths, these are probably zero vectors
  if(length < 1e-12)
    return;
  
  //Ok, good to go, set it up
  directionType = 'f';
  for(int i=0; i < 3; i++)
    fixedDirection[i] = direction[i]/length;
  //Compute Theta, Phi
  generatedPhotonTheta = TMath::ACos(fixedDirection[2]);
  generatedPhotonPhi = TMath::ATan2(fixedDirection[0],fixedDirection[1]);

}

void RPhotonSource::SetFixedDirection(Double_t direction[3]) {
  //Sets source to output in a fixed direction, with direction given
  //by input array.  Note: dangerous!  Must be at least length 3!  
  //The input vector will be normalized.  If it has length zero (or
  //nearly zero) this command will do nothing
  
  Double_t length = 0;
  for(int i=0; i < 3; i++)
    length += direction[i]*direction[i];
  
  //Ignore tiny lengths, these are probably zero vectors
  if(length < 1e-12)
    return;
  
  //Ok, good to go, set it up
  directionType = 'f';
  for(int i=0; i < 3; i++)
    fixedDirection[i] = direction[i]/length;
  //Compute Theta, Phi
  generatedPhotonTheta = TMath::ACos(fixedDirection[2]);
  generatedPhotonPhi = TMath::ATan2(fixedDirection[0],fixedDirection[1]);

}

void RPhotonSource::SetIsotropic() {
  //Switches to isotropic source
  directionType = 'i';
}

void RPhotonSource::Print() {
  //Dumps internal variables to screen, for debugging
  cout << "setupOK = " << setupOK << endl;
  cout << "contRelProb = " << contRelProb << ", contProb = " << contProb;
  cout << endl;
  for(int i=0; i < lineEnergies.size(); i++) {
    cout << "lineEnergies[" << i << "] = " << lineEnergies[i] << endl;
  }
  for(int i=0; i < lineRelProbs.size(); i++) {
    cout << "lineRelProbs[" << i << "] = " << lineRelProbs[i] << endl;
  }
  for(int i=0; i < lineProbs.size(); i++) {
    cout << "lineProbs[" << i << "] = " << lineProbs[i] << endl;
  }
  if(continuous == NULL) {
    cout << "continuous = NULL\n";
  } else {
    continuous->Print();
  }
  for(int i=0; i < 3; i++)
    cout << "positionArr[" << i << "] = " << positionArr[i] << endl;
  for(int i=0; i < 3; i++)
    cout << "directionArr[" << i << "] = " << directionArr[i] << endl;
  for(int i=0; i < 3; i++) {
    cout << "sourceCenterPosition[" << i << "] = " << sourceCenterPosition[i];
    cout<< endl;
  }
  cout << "generatedPhotonTheta = " << generatedPhotonTheta << endl;
  cout << "generatedPhotonPhi = " << generatedPhotonPhi << endl;

}
