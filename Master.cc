//all-commanding master that will control everything!
#include "Master.h"
//source position and 
Double_t Master::sourceposition[3]={0.0,0.0,20.0}; //cart position {x,y,z}
Double_t Master::topspacecylgeometry[3]={0,20.00,20.0};//cyl coords {volnumber,radius,height}
Double_t Master::gecylgeometry[3]={1,4.11,6.53};
Double_t Master::spacecylgeometry[3]={2,4.20,6.53};
Double_t Master::naicylgeometry[3]={3,10.00,6.53};
Double_t Master::initialphotonenergy=500; //keV but switched to MeV in initializer
Double_t Master::densityarray[4]={10e-16,5.323,10e-16,3.67}; //g/cm^3;vacuum,ge,space,nai
string Master::mufiles[2]={"Ge_cross_notitles.txt","NaI_cross_notitles.txt"};

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
  delete vacphoton;
  delete gephoton;
  delete spacephoton;
  delete naiphoton;
}

Double_t Master::Initializer() {
  //need to figure out how to deal with time stamps...

  //dealing with the photon source,reading the coefficient files, setting geometries up
  initialphotonenergy=initialphotonenergy/1000; //to get MeV! 
  photonsource->SetSourcePosition(sourceposition);
  photonsource->AddLine(initialphotonenergy);
  photonsource->Initialize();

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
  loopnumber=0;
  vacphoton=new Photon(photonsource->GetPhoton(),10e-6,densityarray[0]);
  gephoton=new Photon(photonsource->GetPhoton(),mufiles[0],densityarray[1]);
  spacephoton=new Photon(photonsource->GetPhoton(),10e-6,densityarray[2]);
  naiphoton=new Photon(photonsource->GetPhoton(),mufiles[1],densityarray[3]);
  return 0;
}

Double_t Master::Walk1Photon() {

  energy=vacphoton->GetPhotonEnergy(); //for ttree branch
  geometry->SetOriginalPosition(sourceposition);//of course starting at source :)
  geometry->SetPhotonPosition(cartposition);//puts cart coords in cartposition array

  newsphaddition[1]=3.1415926; //original theta
  newsphaddition[2]=0; //original phi
  bool loopcontinue=true; //lets me know whether the loop will continue!
  interactiontype=0;
  outTree->Fill();
  //initally,the cartposition in both geo and master class, the energy in both classes, the mu in photon class, the interaction type, and theta/phi are declared above.

  while (loopcontinue) {//will loop until all energy deposited or escape!
    
    newsphaddition[0]=photon->PhotonStepperSlick(); //find photon walk length

    cout<<"sphadd:"<<newsphaddition[0]<<","<<newsphaddition[1]<<","<<newsphaddition[2]<<endl;
    geometry->SetNewSphAddition(newsphaddition); //find new photon cart position
    
    geometry->SetPhotonPosition(cartposition);//sets master class cartposition to geo class one
    cout<<"new1:"<<cartposition[0]<<","<<cartposition[1]<<","<<cartposition[2]<<endl;
    geometry->PhotonVolumePosition(loopnumber); //finds which volume the photon is in, if the photon reaches a new volume it will call exitcartposition to find the new cart position at the boundary.
    geometry->SetPhotonPosition(cartposition);
    cout<<"new2:"<<cartposition[0]<<","<<cartposition[1]<<","<<cartposition[2]<<endl;    
    volumenumber=geometry->GetVolumeNumber();//just set volumenumber to geo class's vol number

    interactiontype=photon->InteractionFinder(); //finds which interaction occured

    if (geometry->GetVolumeNumber()==5) { //checking photon is within total volume bounds
      cout<<"out of bounds"<<endl;
      loopcontinue=false;
    }
    else if (geometry->GetVolumeNumber()==6) {
      cout<<"Photonvolumeposition unsuccessful!"<<endl;
      loopcontinue=false;
    }
    else if (geometry->GetNewVolumeReached()==true) { //checking if new volume reached
      photon->SetSplineMu(volumenumber);
      cout<<"photon crossed a boundary!"<<endl;
      //  geometry->SetCartPosition();
    }
    else if (interactiontype==1) { //compton scattering!
      cout<<"compton"<<endl;
      newsphaddition[1]=photon->ThetaFinder(); //differential cross section theta!
      newsphaddition[2]=photon->PhiFinder(); //new random phi direction

      photon->SetPhotonEnergy(photon->ComptonEnergyCalc());//new photon energy is from compton
      energy=photon->GetPhotonEnergy();

      if (photon->GetPhotonEnergy()<=0) {//checking that new photon energy is larger than 0
	cout<<"compton absorbed"<<endl;
	energy=0.0; //so photon has been fully absorbed!
	loopcontinue=false;//tells while loop to end
      }
      outTree->Fill();
    }
    else if (interactiontype==2) { //photoelectric effect!
      cout<<"photo"<<endl;
      energy=0.0;
      outTree->Fill();
      loopcontinue=false; //tells while loop to end
    }
    loopnumber++; //increases the loop number by one for every time the while loops...
  }
  photonnumber++;//increases the photon number for everytime a photon finishes doing its stuff
  cout<<"total photon loop complete"<<endl;
  return 0;
}

void Master::Finisher() {
  photonsource->ClearPhotons();
  outTree->Write();
  myfile->Close();
}
