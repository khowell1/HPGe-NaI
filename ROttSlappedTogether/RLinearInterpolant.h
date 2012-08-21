/*******************************************************
 A simple class that interpolates linearly between data
 points.  Needed as the ROOT version requires the MathMore
 library, which we do not have installed.

 The class requires that the independent variable (x)
 be monotonically increasing.  If an x value is repeated,
 this creates a discontinuity but behaves properly.  If
 an x value is repeated more than once, only the first and last
 y values are used.  A minimum of two points is necessary.

 Variables are stored internally as Double_t, but input
 can be Double_t or Float_t

 If passed an x out of the array bounds, returns 0

 General use:
   Set the points to use either by constructor or set points
   Call IsZombie to make sure things set up properly (will return false
     if everything is OK)
   Call Eval(x) to find the interpolated value at x
 
 *******************************************************/


#include <iostream>
#include <vector>
#include <string>
#include "TROOT.h"
using namespace std;

#ifndef RLINEARINTERPOLANT_H
#define RLINEARINTERPOLANT_H

class RLinearInterpolant {
  
 private:
  vector<Double_t> xVals;
  vector<Double_t> yVals;
  Int_t nPoints;
  
  //Helper Functions
  Int_t FindBin(Double_t x); //Returns i st xVals[i] <= x < xVals[i+1]

 public:
  RLinearInterpolant();
  RLinearInterpolant(Int_t nPointsIn);
  RLinearInterpolant(Int_t nPointsIn, Double_t *xIn, Double_t *yIn);
  RLinearInterpolant(Int_t nPointsIn, Float_t *xIn, Float_t *yIn);
  RLinearInterpolant(vector<Double_t>& xIn, vector<Double_t>& yIn);
  RLinearInterpolant(vector<Float_t>& xIn, vector<Float_t>& yIn);
  void SetPoints(Int_t nPointsIn, Double_t *xIn, Double_t *yIn);
  void SetPoints(Int_t nPointsIn, Float_t *xIn, Float_t *yIn);
  void SetPoints(vector<Double_t>& xIn, vector<Double_t>& yIn);
  void SetPoints(vector<Float_t>& xIn, vector<Float_t>& yIn);
  Double_t Eval(Double_t x);
  Double_t EvalSafe(Double_t x);
  Int_t GetNPoints() { return nPoints; };
  Bool_t IsZombie(); //Returns true if problems, false if OK
  void Print();

};


#endif
