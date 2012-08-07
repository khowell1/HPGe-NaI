//all-commanding master that will control everything!
#include "Master.h"
//source position and 
Double_t Master::sourceposition[3]={0.0,0.0,20.0}; //cart position {x,y,z}
Double_t Master::topspacecylgeometry[3]={0,10.00,20.0};//cyl coords {volnumber,radius,height}
Double_t Master::gecylgeometry[3]={1,4.11,6.53};
Double_t Master::spacecylgeometry[3]={2,4.20,6.53};
Double_t Master::naicylgeometry[3]={3,10.00,6.53};
Double_t Master::initialphotonenergy=500; //keV but switched to MeV in initializer

Master::Master() {
  photonsource= new RPhotonSource();
  photon=new Photon();
  geometry=new SetupGeometry();
  string filename="kaitlin_results.root";
  myfile=new TFile(filename.c_str(),"RECREATE");
  outTree=new TTree("Photons","Photons");
}

Master::~Master() {
  delete photonsource;
  delete photon;
  delete geometry;
  delete myfile;
  delete outTree;
}

Double_t Master::Initializer() {
  //need to figure out how to deal with time stamps...

  //dealing with the photon source,reading the coefficient files, setting geometries up
  initialphotonenergy=initialphotonenergy/1000; //to get MeV! 
  photonsource->SetSourcePosition(sourceposition);
  photonsource->AddLine(initialphotonenergy);
  photonsource->Initialize();

  //deal with photon initializing
  photon->FileReader();//reads Ge mu attenuation coefficient file

  //deal with geometry initializing
  geometry->SetGeometryData(topspacecylgeometry);//n=0
  geometry->SetGeometryData(gecylgeometry);//n=1
  geometry->SetGeometryData(spacecylgeometry);//n=2
  geometry->SetGeometryData(naicylgeometry);//n=3

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

  return 0;
}

Double_t Master::Walk1Photon() {
  photon->SetPhotonEnergy(photonsource->GetPhoton()); //value is from photon source, in MeV
  energy=photon->GetPhotonEnergy(); //for ttree branch
  geometry->SetPhotonPosition(sourceposition);//of course starting at source :)
  geometry->SetPhotonPosition(cartposition);//puts cart coords in currentcartposition array
  photon->SetSplineMu(); //finding spline mu for found photon energy
  newsphaddition[1]=0; //original theta
  newsphaddition[2]=0; //original phi
  bool loopcontinue=true; //lets me know whether the loop will continue!
  loopnumber=0;

  while (loopcontinue) {//will loop until all energy deposited or escape!
    
    newsphaddition[0]=photon->PhotonStepperSlick(); //find photon walk length
    geometry->SetNewSphAddition(newsphaddition); //find new photon cart position
    geometry->SetPhotonPosition(cartposition);//sets master class cartposition to geo class one
    geometry->PhotonVolumePosition(); //finds which volume the photon is in, if the photon reaches a new volume it will call exitcartposition to find the new cart position at the boundary.
    
    volumenumber=geometry->GetVolumeNumber();//just set volumenumber to geo class's vol number

    interactiontype=photon->InteractionFinder(); //finds which interaction occured

    if (geometry->GetVolumeNumber()==5) { //checking photon is within total volume bounds
      //cout<<"out of bounds"<<endl;
      loopcontinue=false;
    }
    else if (geometry->GetVolumeNumber()==6) {
      cout<<"Photonvolumeposition unsuccessful!"<<endl;
      loopcontinue=false;
    }
    else if (geometry->GetNewVolumeReached()==true) { //checking if new volume reached
      //cout<<"photon crossed a boundary!"<<endl;
      geometry->GetPhotonPosition();
    }
    else if (interactiontype==1) { //compton scattering!
      newsphaddition[1]=photon->ThetaFinder(); //differential cross section theta!
      newsphaddition[2]=photon->PhiFinder(); //new random phi direction

      photon->SetPhotonEnergy(photon->ComptonEnergyCalc());//new photon energy is from compton

      if (photon->GetPhotonEnergy()<=0) {//checking that new photon energy is larger than 0
	energy=0.0; //so photon has been fully absorbed!
	loopcontinue=false;//tells while loop to end
      }
      outTree->Fill();
    }
    else if (interactiontype==2) { //photoelectric effect!
      energy=0.0;
      outTree->Fill();
      loopcontinue=false; //tells while loop to end
    }
    loopnumber++; //increases the loop number by one for every time the while loops...
  }
  photonnumber++; //increases the photon number for everytime a photon finishes doing its stuff
  return 0;
}

void Master::Finisher() {
  outTree->Write();
  myfile->Close();
  TFile f("kaitlin_results.root");
  T->Show(1);
}
