#include <iostream>
#include "TMath.h"
#include "TROOT.h"
using namespace std;

#ifndef CYLINDER_H
#define CYLINDER_H

class Cylinder {

 private:
  Double_t innerRadius;
  Double_t outerRadius;
  Double_t height;
  Double_t centerPosition[3]; //In cartesian

  //Helper functions, no outside access
  void ConvertToLocalCyl(Double_t position[], Double_t localCyl[]);

 public:
  Cylinder();
  Cylinder(Double_t setInnerRadius, Double_t setOuterRadius, 
	   Double_t setHeight, Double_t setCenterPosition[]);
  void SetInnerRadius(Double_t setInnerRadius);
  void SetOuterRadius(Double_t setOuterRadius);
  void SetHeight(Double_t setHeight);
  void SetCenterPosition(Double_t setCenterPosition[]);
  Double_t GetInnerRadius();
  Double_t GetOuterRadius();
  Double_t GetHeight();
  //This will copy center position into argument array
  void GetCenterPosition(Double_t getCenterPosition[]);
  Bool_t IsPointInside(Double_t position[]);
  Double_t FindCrossPoint(Double_t currentPosition[], Double_t direction[],
			 Double_t exitPosition[]);

};

#endif
