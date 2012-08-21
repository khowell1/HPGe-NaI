
// Master class, coordinates actions of geometry and photons

#include "Master.h"

Master::Master() {
  myfile = NULL;
  outTree = NULL;
  photonSource = NULL;
  photonNumber = 0;
  volumeNumber = 0;
  interactionType = 0;
  photonEnergy = -1;
  depositedEnergy = 0;
  simulationBoundaryRadius = 1000;
  for(int i=0; i < 3; i++) {
    cartPosition[i] = 0;
    cartDirection[i] = 0;
  }
}

Master::~Master() {
  if(outTree != NULL)
    delete outTree;
  if(photonSource != NULL)
    delete photonSource;
  if(myfile != NULL)
    delete myfile;

}

Bool_t Master::Initializer() {
  //Returns true if successful
  //Sets up system to run with default behavior
  myfile = new TFile("kaitlin_results.root","RECREATE");
  myfile->cd();
  outTree = new TTree("Photons","Photons");
  outTree->Branch("PhotonNumber",&photonNumber,"PhotonNumber/I");
  outTree->Branch("PhotonEnergy",&photonEnergy,"PhotonEnergy/D");
  outTree->Branch("VolumeNumber",&volumeNumber,"VolumeNumber/I");
  outTree->Branch("InteractionType",&interactionType,"InteractionType/I");
  outTree->Branch("DepositedEnergy",&depositedEnergy,"DepositedEnergy/D");
  outTree->Branch("CartPosition",cartPosition,"CartPosition[3]/D");

  photonEnergy = 500; //keV, converted to MeV for photon class later

  //Setup default photon source at +20 cm in z, shooting straight down
  photonSource = new RPhotonSource();
  Double_t fixedDirection[3] = {0, 0, -1};
  photonSource->SetFixedDirection(fixedDirection);
  Double_t sourcePosition[3] = {0, 0, 20}; //in cm
  photonSource->SetSourcePosition(sourcePosition);
  photonSource->AddLine(0.8,1);
  photonSource->AddLine(0.6,0.001);
  photonSource->Initialize();

  //Add central Ge cylinder of radius 4.11 cm, height 6.53 cm, located with
  //top center at 0,0,0
  photonWalkers.push_back(new Photon(0.5, "Ge_cross_notitles.txt", 5.3));
  Double_t gePosition[3] = {0, 0, -3.265};
  spaces.push_back(new Cylinder(-1, 4.11, 6.53, gePosition));

  //Add ring of NaI, with radius from 5 cm to 10 cm, height 6.53, concentric
  //with Ge (and same top and bottom heights)
  photonWalkers.push_back(new Photon(0.5, "NaI_cross_notitles.txt", 3.67));
  spaces.push_back(new Cylinder(5, 15, 10, gePosition));

  //A 10 m sphere should be more than enough
  simulationBoundaryRadius = 1000;

  return true;
}



Double_t Master::Walk1Photon() {
  //This generates one photon from the source, then walks it until it either
  //is absorbed or escapes.
  //Future upgrade: allow for pair production, which generates an extra photon
  //presumably, this would just call WalkPhoton again
  
  //Get photon from source
  photonNumber++;
  Double_t position[3], direction[3];
  photonEnergy = photonSource->GetPhoton(direction, position);
  //cout << "Direction: " << direction[0] << ", " << direction[1] << ", ";
  //cout << direction[2] << endl;
  
  WalkPhoton(direction, position);

  return 0;

}

Double_t Master::WalkPhoton(Double_t direction[3], Double_t position[3]) {
  //This walks a photon as given.  It *does not* update the photon number
  //Thinking ahead, this could be called multiple times for pileup or pair prod
  
  Bool_t doneWalking = false;

  interactionType = 0; //No interaction, just register generation of photon
  for(int i=0; i < 3; i++) {
    cartPosition[i] = position[i];
    cartDirection[i] = direction[i];
  }
  
  FindVolumeNumber();

  outTree->Fill();

  Double_t newPosition[3] = {0,0,0};

  while(!doneWalking) {
    depositedEnergy = 0;
    if(volumeNumber == -2) {
      //Outside area, quit!
      interactionType = 0;
      doneWalking = true;
    } else if(volumeNumber == -1) {
      //In vaccuum space, find what volume to propagate to
      //by finding closest intersection with a surface.  Then add 10 micron
      //cout << "In vacuum!\n";
      Double_t distance = -1;
      Double_t distance_min = simulationBoundaryRadius;
      for(int i=0; i < spaces.size(); i++) {
	distance = spaces[i]->FindCrossPoint(cartPosition, cartDirection, 
					     newPosition);
	//cout << "Distance to volume " << i << " = " << distance << endl;
	//cout << distance_min << endl;
	if(distance > 0 && distance_min > distance) {
	  //cout << "Setting volume to " << i << endl;
	  distance_min = distance;
	  volumeNumber = i;
	  interactionType = 0;
	  for(int j=0; j < 3; j++) {
	    cartPosition[j] = newPosition[j] + 0.001*cartDirection[j];
	  }
	}
      }
      //If distance < 0, no intersection, walk out of volume
      if(distance_min == simulationBoundaryRadius) {
	interactionType = 0;
	volumeNumber = -2;
	doneWalking = true;
      }
      //cout << "Now in volume " << volumeNumber << endl;
    } else {
      //In an actual volume, take a normal step
      photonWalkers[volumeNumber]->SetPhotonEnergy(photonEnergy);
      Double_t distance = photonWalkers[volumeNumber]->PhotonStepperSlick();
      //cout << "Stepped distance " << distance << endl;
      for(int i=0; i < 3; i++)
	newPosition[i] = cartPosition[i] + distance*cartDirection[i];
      if(spaces[volumeNumber]->IsPointInside(newPosition)) {
	//Stepped somewhere also inside volume,interaction happens
	for(int i=0; i < 3; i++)
	  cartPosition[i] = newPosition[i];
	interactionType = photonWalkers[volumeNumber]->InteractionFinder();
	//cout << "interaction: " << interactionType << endl;
	if(interactionType == 1) {
	  TakeComptonStep();
	} else if(interactionType == 2) {
	  //Photoelectric effect, absorb photon and done
	  depositedEnergy = photonEnergy;
	  doneWalking = true;
	}
      } else {
	//Stepped out of volume, propagate to edge and add 10 micron
	//cout << "Propagating to edge!\n";
	spaces[volumeNumber]->FindCrossPoint(cartPosition, cartDirection,
					     newPosition);
	for(int i=0; i < 3; i++)
	  cartPosition[i] = newPosition[i] + 0.001*cartDirection[i];
	FindVolumeNumber();
      }
    }
    //cout << "Ach mein loopen\n";

    outTree->Fill();
  }

  return 0;

}

void Master::Finisher() {
  //Wraps everything up with a tidy bow
  outTree->Write();
  delete outTree;
  outTree = NULL;
  myfile->Close();
}

Int_t Master::FindVolumeNumber() {
  //Goes through the list of volumes and to find which one the current photon
  //is located in
  volumeNumber = -1;
  for(int i=0; i < spaces.size(); i++) {
    if(spaces[i]->IsPointInside(cartPosition))
      volumeNumber = i;
  }
  
  //This automatically makes volumeNumber -1 if it's not in any volume,
  //this corresponds to vaccuum.
  //Now check if it's outside the boundary
  if(cartPosition[0]*cartPosition[0] + cartPosition[1]*cartPosition[1] +
     cartPosition[2]*cartPosition[2] > simulationBoundaryRadius)
    volumeNumber = -2;

  return volumeNumber;
}

void Master::TakeComptonStep() {
  //Compton scattering, update photon's energy and direction
  //The whole point of the Photon class!

  depositedEnergy = photonEnergy;
  photonEnergy = photonWalkers[volumeNumber]->ComptonEnergyCalc();
  depositedEnergy -= photonEnergy;

  Double_t oldTheta, oldPhi;
  directionVec.SetXYZ(cartDirection[0],cartDirection[1],cartDirection[2]);
  oldTheta = directionVec.Theta();
  oldPhi = directionVec.Phi();
  
  Double_t theta = photonWalkers[volumeNumber]->ThetaFinder();
  Double_t phi = photonWalkers[volumeNumber]->PhiFinder();

  directionVec.SetXYZ(0,0,1);
  directionVec.SetTheta(theta);
  directionVec.SetPhi(phi);
  
  r.Rotate(oldTheta,TVector3(-TMath::Sin(oldPhi),TMath::Cos(oldPhi),0));
  directionVec.Transform(r);

  cartDirection[0] = directionVec.X();
  cartDirection[1] = directionVec.Y();
  cartDirection[2] = directionVec.Z();

  //cout << "New direction: " << cartDirection[0] << ", " << cartDirection[1];
  //cout << ", " << cartDirection[2] << endl;

}
