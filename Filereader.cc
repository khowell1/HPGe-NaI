//just has filereader and spline stuff! should prob separate spline and file reader but easier here for now.
#include "Photon.h"

void Photon::FileReader() { //eventually need to make it take any file... but static for now!
  vector<Double_t> energy_vector;
  vector<Double_t> coherent_scat_vector;
  vector<Double_t> incoherent_scat_vector;
  vector<Double_t> photoelectric_vector;
  vector<Double_t> pairprodnucl_vector;
  vector<Double_t> pairprodelec_vector;
  vector<Double_t> cross_vector;
  vector<Double_t> cross_nocoher_vector;
  Double_t temp_energy;
  Double_t temp_coherent;
  Double_t temp_incoherent;
  Double_t temp_photo;
  Double_t temp_pairnucl;
  Double_t temp_pairelec;
  Double_t temp_cross;
  Double_t temp_cross_nocoh;  
  int vector_size;

  ifstream myfile("Ge_cross_notitles.txt");
  while (myfile.good()) { //reading in the values from the file into the vectors
    myfile>>temp_energy>>temp_coherent>>temp_incoherent>>temp_photo>>temp_pairnucl>>temp_pairelec>>temp_cross>>temp_cross_nocoh;
    energy_vector.push_back(temp_energy);
    coherent_scat_vector.push_back(temp_coherent);
    incoherent_scat_vector.push_back(temp_incoherent);
    photoelectric_vector.push_back(temp_photo);
    pairprodnucl_vector.push_back(temp_pairnucl);
    pairprodelec_vector.push_back(temp_pairelec);
    cross_vector.push_back(temp_cross);
    cross_nocoher_vector.push_back(temp_cross_nocoh);
  }    
  myfile.close();
  //redefining the arrays' size
  vector_size=energy_vector.size();

  //splines!!!
  mu_spline= new RLinearInterpolant(energy_vector,cross_vector);
  //  coherent_spline= new RLinearInterpolant(energy_vector,coherent_scat_vector);
  incoherent_spline= new RLinearInterpolant(energy_vector,incoherent_scat_vector);
  photoelec_spline= new RLinearInterpolant(energy_vector,photoelectric_vector);
  pairprod_spline= new RLinearInterpolant(energy_vector,pairprodnucl_vector);

}

  // Double_t* energy_list;
  // Double_t* coherent_scat_list;
  // Double_t* incoherent_scat_list;
  // Double_t* photoelectric_list;
  // Double_t* pairprodnucl_list;
  // Double_t* pairprodelec_list;
  // Double_t* cross_section_list;
  // Double_t* cross_section_nocoher_list;
  // energy_list= new Double_t[vector_size];
  // coherent_scat_list = new Double_t[vector_size];
  // incoherent_scat_list = new Double_t[vector_size];
  // photoelectric_list = new Double_t[vector_size];
  // pairprodnucl_list = new Double_t[vector_size];
  // pairprodelec_list = new Double_t[vector_size];
  // cross_section_list = new Double_t[vector_size];
  // cross_section_nocoher_list = new Double_t[vector_size];
  // //for moving the data from the vectors to the arrays. 
  // for (int i=0;i<vector_size;i++) { //filling in the arrays from the vectors
  //   energy_list[i]=energy_vector[i];
  //   coherent_scat_list[i]=coherent_scat_vector[i];
  //   incoherent_scat_list[i]=incoherent_scat_vector[i];
  //   photoelectric_list[i]=photoelectric_vector[i];
  //   pairprodnucl_list[i]=pairprodnucl_vector[i];
  //   pairprodelec_list[i]=pairprodelec_vector[i];
  //   cross_section_list[i]=cross_vector[i];
  //   cross_section_nocoher_list[i]=cross_nocoher_vector[i];
  // }
//need to change list name to mu name or attentuation coefficients! but do when just bored. 
