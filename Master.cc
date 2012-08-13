//all-commanding master that will control everything!
#include "Master.h"
//source position and 
Double_t Master::sourceposition[3]={0.0,0.0,20.0}; //cart position {x,y,z}
// Double_t Master::topspacecylgeometry[3]={0,20.00,20.0};//cyl coords {volnumber,radius,height}
// Double_t Master::gecylgeometry[3]={1,4.11,6.53};
// Double_t Master::spacecylgeometry[3]={2,4.20,6.53};
// Double_t Master::naicylgeometry[3]={3,10.00,6.53};
Double_t Master::cylradii[4]={20.0,4.11,5.0,10.0};
Double_t Master::cylheights[4]={20.0,6.53,6.53,6.53};
Double_t Master::initialphotonenergy=500; //keV but switched to MeV in initializer
Double_t Master::densityarray[4]={10e-16,5.323,10e-16,3.67}; //g/cm^3;vacuum,ge,space,nai
string Master::muarray[4]={"10e-6","Ge_cross_notitles.txt","10e-6","NaI_cross_notitles.txt"};

Master::Master() {
  photonsource= new RPhotonSource();
  geometry=new SetupGeometry();
  string filename="kaitlin_results.root";
  myfile=new TFile(filename.c_str(),"RECREATE");
  outTree=new TTree("Photons","Photons");
}

Master::~Master() {
  delete photonsource;
  delete geometry;
  delete myfile;
  delete photon;
  // delete vacphoton;
  // delete gephoton;
  // delete spacephoton;
  // delete naiphoton;
}

Double_t Master::Initializer() {
  //need to figure out how to deal with time stamps...

  //dealing with the photon source,reading the coefficient files, setting geometries up
  initialphotonenergy=initialphotonenergy/1000; //to get MeV! 
  photonsource->SetSourcePosition(sourceposition);
  photonsource->AddLine(initialphotonenergy);
  photonsource->Initialize();

  //deal with geometry initializing
  // geometry->SetGeometryData(topspacecylgeometry);//n=0
  // geometry->SetGeometryData(gecylgeometry);//n=1
  // geometry->SetGeometryData(spacecylgeometry);//n=2
  // geometry->SetGeometryData(naicylgeometry);//n=3

  for (int n=0;n<3;n++) {
    geometry->SetGeometryDataArray(n,cylradii[n],cylheights[n]);
  }

  //dealing with ttree branches
  outTree->Branch("PhotonNumber",&photonnumber,"PhotonNumber/I");
  outTree->Branch("Energy",&energy,"Energy/D");
  outTree->Branch("Cartposition",cartposition,"Cartposition[3]/D");
  outTree->Branch("VolumeNumber",&volumenumber,"VolumeNumber/I");
  outTree->Branch("InteractionType",&interactiontype,"Interactiontype/I");
  outTree->Branch("LoopNumber",&loopnumber,"LoopNumber/I");

  //initial conditions for the first photon!
  photonnumber=0;
  volumenumber=0;
  loopnumber=0;
  interactiontype=0;
  geometry->SetPhotonPosition(cartposition);//puts cart coords in cartposition array
  geometry->SetOriginalPosition(sourceposition);//of course starting at source :)

  //creating the 1st photon object
  photon=new Photon(photonsource->GetPhoton(),muarray[0],densityarray[0]);
  energy=photon->GetPhotonEnergy();
  outTree->Fill();
  //  gephoton=new Photon(photonsource->GetPhoton(),muarray[1],densityarray[1]);
  //  spacephoton=new Photon(photonsource->GetPhoton(),muarray[2],densityarray[2]);
  //  naiphoton=new Photon(photonsource->GetPhoton(),muarray[3],densityarray[3]);
  return 0;
}

Double_t Master::Walk1Photon() {

  geometry->SetTheta(3.1415926);//so photon goes straight down...
  geometry->SetPhi(0.0); 

  bool loopcontinue=true; //lets me know whether the loop will continue!

  while (loopcontinue) {//will loop until all energy deposited or escape!

    geometry->SetDistance(photon->PhotonStepperSlick());
    geometry->SetCartPosition();
    geometry->SetPhotonPosition(cartposition);//sets master class cartposition to geo class one

    geometry->VolumeChecker(loopnumber); //finds which volume the photon is in, if the photon reaches a new volume it will call exitcartposition to find the new cart position at the boundary.
    geometry->SetPhotonPosition(cartposition);
    volumenumber=geometry->GetVolumeNumber();//just set volumenumber to geo class's vol number

    if (geometry->GetVolumeNumber()==5 || geometry->GetVolumeNumber()==6) { //checking if photon escaped
      volumenumber=geometry->GetVolumeNumber();
      interactiontype=0;
      loopcontinue=false;
      outTree->Fill();
    }
    // else if (geometry->GetVolumeNumber()==6) {//something went wrong with method
    //   volumenumber=geometry->GetVolumeNumber();
    //   interactiontype=0;
    //   loopcontinue=false;
    //   outTree->Fill();
    // }

    else if (geometry->GetNewVolumeReached()==true) { //checking if new volume reached
      volumenumber=geometry->GetVolumeNumber();
      photon=new Photon(energy,muarray[volumenumber],densityarray[volumenumber]);
    }

    else {
      interactiontype=photon->InteractionFinder(); //finds which interaction occured

      if (interactiontype==1) { //compton scattering!
	geometry->SetTheta(photon->ThetaFinder()); //differential cross section theta!
	geometry->SetPhi(photon->PhiFinder()); //new random phi direction

	photon->SetPhotonEnergy(photon->ComptonEnergyCalc());//new photon energy is from compton
	energy=photon->GetPhotonEnergy();

	if (photon->GetPhotonEnergy()<=0) {//checking that new photon energy is larger than 0
	  energy=0.0; //so photon has been fully absorbed!
	  loopcontinue=false;//tells while loop to end
	}
	outTree->Fill();
      }

      else if (interactiontype==2) { //photoelectric effect!
	energy=0.0;
	loopcontinue=false; //tells while loop to end
	outTree->Fill();
      }
    }
    loopnumber++; //increases the loop number by one for every time the while loops...
  }
  photonnumber++;//increases the photon number for everytime a photon finishes doing its stuff
  return 0;
}

void Master::Finisher() {
  photonsource->ClearPhotons();
  outTree->Write();
  myfile->Close();
}
