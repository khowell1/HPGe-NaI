void resultseer(Int_t eventnumber) 
{
	TFile results("kaitlin_results.root");
	TTree* result=(TTree*)results.Get("Photons");
	result->Show(eventnumber);
	result->Draw("CartPosition");
}

