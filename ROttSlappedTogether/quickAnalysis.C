
{
  TFile myfile("kaitlin_results.root");
  TTree *mytree = (TTree*)myfile.Get("Photons");
  Int_t nEntries = mytree->GetEntries();
  Int_t photonNumber = 0;
  Int_t photonNumberOld = 0;
  Int_t volumeNumber = 0;
  Double_t depEnergy;
  mytree->SetBranchAddress("PhotonNumber",&photonNumber);
  mytree->SetBranchAddress("VolumeNumber",&volumeNumber);
  mytree->SetBranchAddress("DepositedEnergy",&depEnergy);

  Double_t totalEnergyGe = 0;
  Double_t totalEnergyNaI = 0;

  gROOT->cd();
  TH1D gehist("GeDepEnergy","Ge Deposited Energy",300,0.01,0.9);
  TH1D nahist("NaIDepEnergy","NaI Deposited Energy",300,0.01,0.9);
  TH1D geSuppressedHist("GeSuppressed","Ge Dep Energy, Compton Suppressed",
			300,0.01,0.9);
  
  myfile.cd();
  mytree->GetEntry(0);
  photonNumberOld = photonNumber;

  for(int i=0; i < nEntries; i++) {
    mytree->GetEntry(i);
    if(photonNumberOld == photonNumber) {
      //Same photon, just update totals
      if(volumeNumber == 0)
	totalEnergyGe += depEnergy;
      if(volumeNumber == 1)
	totalEnergyNaI += depEnergy;
    } else {
      totalEnergyGe = gRandom->Gaus(totalEnergyGe,totalEnergyGe*0.01);
      gehist->Fill(totalEnergyGe);
      totalEnergyNaI = gRandom->Gaus(totalEnergyNaI,totalEnergyNaI*0.06);
      if(totalEnergyNaI < 0.01)
	geSuppressedHist.Fill(totalEnergyGe);
      nahist->Fill(totalEnergyNaI);
      //New photon, reset energies
      totalEnergyGe = 0;
      totalEnergyNaI = 0;
      photonNumberOld = photonNumber;
      if(volumeNumber == 0)
	totalEnergyGe = depEnergy;
      if(volumeNumber == 1)
	totalEnergyNaI = depEnergy;
    }
      
  }

  delete mytree;
  myfile.Close();

}
