//all-commanding master that will control everything!

Master::Initializer() {
  //need to figure out how to deal with time stamps...
  Initialize();
  GetPhoton();
  FileReader();
  Setup_Geometry ge_volume(0,4.11,6.53);
  Setup_Geometry space_volume(1,4.20,6.53);
  Setup_Geometry nai_volume(2,5.00,6.53);

}

Master::Walk1Photon() {
  while (photon_energy>0 /*figure this out*/) {
    travel_length=PhotonStepperSlick();
    new_phi=PhiFinder();
    new_theta=ThetaFinder();
    typeinteraction=InteractionFinder();
    if (typeinteraction==1) {
      //pair production... figure out what to do later!
    }
    else if (typeinteraction==2) {
      //compton scattering!
      newphotonenergy=ComptonEnergyCalc(/*stuff goes in here...*/);
    }
    else if (typeinteraction==3) {
      //photo
      //have it so that everything is completed and necessary info saved. 
  }

}
