#include "RLinearInterpolant.h"

RLinearInterpolant::RLinearInterpolant() {
  //Default to nothing
  nPoints = 0;
}

RLinearInterpolant::RLinearInterpolant(Int_t nPointsIn) {
  //If only given number of points, allocate that much space
  //If more points are fed in later, no harm is done, only speed impacted
  nPoints = nPointsIn;
  xVals.reserve(nPoints);
  yVals.reserve(nPoints);
}

RLinearInterpolant::RLinearInterpolant(Int_t nPointsIn, Double_t *xIn,
				       Double_t *yIn) {
  //Arrays xIn and yIn are assumed to be of length nPointsIn
  //No out-of-bounds checking possible!
  //The first nPointsIn of xIn and yIn are copied in as data to be interpolated
  //x is checked for monotinicity.  If not monotonic, read-in fails, an
  //error message is given, and the data is cleared

  nPoints = nPointsIn;
  SetPoints(nPointsIn, xIn, yIn);
  
}

RLinearInterpolant::RLinearInterpolant(Int_t nPointsIn, Float_t *xIn,
				       Float_t *yIn) {
  //Arrays xIn and yIn are assumed to be of length nPointsIn
  //No out-of-bounds checking possible!
  //The first nPointsIn of xIn and yIn are copied in as data to be interpolated
  //x is checked for monotinicity.  If not monotonic, read-in fails, an
  //error message is given, and the data is cleared

  nPoints = nPointsIn;
  SetPoints(nPointsIn, xIn, yIn);
  
}

RLinearInterpolant::RLinearInterpolant(vector<Double_t>& xIn, 
				       vector<Double_t>& yIn) {
  //Vectors are copied.  If xIn.size() != yIn.size()
  // nPoints is set to the smaller of the two, and only the first
  // nPoints values in each vector are copied.  An error message is also given
  //x is checked for monotinicity.  If not monotonic, read-in fails, an
  //error message is given, and the data is cleared

  nPoints = xIn.size();
  SetPoints(xIn, yIn);
  
}

RLinearInterpolant::RLinearInterpolant(vector<Float_t>& xIn, 
				       vector<Float_t>& yIn) {
  //Vectors are copied.  If xIn.size() != yIn.size()
  // nPoints is set to the smaller of the two, and only the first
  // nPoints values in each vector are copied.  An error message is also given
  //x is checked for monotinicity.  If not monotonic, read-in fails, an
  //error message is given, and the data is cleared

  nPoints = xIn.size();
  SetPoints(xIn, yIn);
  
}

void RLinearInterpolant::SetPoints(Int_t nPointsIn, Double_t *xIn, 
				   Double_t *yIn) {
  //Arrays xIn and yIn are assumed to be of length nPointsIn
  //No out-of-bounds checking possible!
  //The first nPointsIn of xIn and yIn are copied in as data to be interpolated
  //x is checked for monotinicity.  If not monotonic, read-in fails, an
  //error message is given, and the data is cleared

  //Clear old data
  nPoints = 0;
  xVals.clear();
  yVals.clear();

  bool isOrdered = true;
  Double_t xLast=0;

  nPoints = nPointsIn;
  if(nPointsIn != 0)
    xLast = xIn[0];
  for(int i=0; i < nPoints; i++) {
    xVals.push_back(xIn[i]);
    yVals.push_back(yIn[i]);
    if(xIn[i] < xLast) 
      isOrdered = false;
    xLast = xIn[i];
  }

  if(!isOrdered) {
    cerr << "x values not increasing, expect errors!\n";
    nPoints = 0;
    xVals.clear();
    yVals.clear();
  }

}

void RLinearInterpolant::SetPoints(Int_t nPointsIn, Float_t *xIn, Float_t *yIn) {
  //Arrays xIn and yIn are assumed to be of length nPointsIn
  //No out-of-bounds checking possible!
  //The first nPointsIn of xIn and yIn are copied in as data to be interpolated
  //x is checked for monotinicity.  If not monotonic, read-in fails, an
  //error message is given, and the data is cleared

  //Clear old data
  nPoints = 0;
  xVals.clear();
  yVals.clear();

  bool isOrdered = true;
  Double_t xLast = 0;

  nPoints = nPointsIn;
  if(nPointsIn != 0)
    xLast = xIn[0];
  for(int i=0; i < nPoints; i++) {
    xVals.push_back(xIn[i]);
    yVals.push_back(yIn[i]);
    if(xIn[i] < xLast) 
      isOrdered = false;
    xLast = xIn[i];
  }

  if(!isOrdered) {
    cerr << "x values not increasing, data not accepted.\n";
    nPoints = 0;
    xVals.clear();
    yVals.clear();
  }

}

void RLinearInterpolant::SetPoints(vector<Double_t>& xIn, 
				       vector<Double_t>& yIn) {
  //Vectors are copied.  If xIn.size() != yIn.size()
  // nPoints is set to the smaller of the two, and only the first
  // nPoints values in each vector are copied.  An error message is also given
  //x is checked for monotinicity.  If not monotonic, read-in fails, an
  //error message is given, and the data is cleared

  //Clear old data
  nPoints = 0;
  xVals.clear();
  yVals.clear();

  bool isOrdered = true;
  Int_t nPointsIn = xIn.size();
  if(xIn.size() < yIn.size()) {
    nPointsIn = xIn.size();
    cerr << "Input arrays to RLinearInterpolant not same size, using size "
	 << nPointsIn << endl;
  }
  if(xIn.size() > yIn.size()) {
    nPointsIn = yIn.size();
    cerr << "Input arrays to RLinearInterpolant not same size, using size "
	 << nPointsIn << endl;
  }

  Double_t xLast=0;
  nPoints = nPointsIn;
  if(nPointsIn != 0)
    xLast = xIn[0];
  for(int i=0; i < nPoints; i++) {
    xVals.push_back(xIn[i]);
    yVals.push_back(yIn[i]);
    if(xIn[i] < xLast) 
      isOrdered = false;
    xLast = xIn[i];
  }

  if(!isOrdered) {
    cerr << "x values not increasing, data not accepted!\n";
    nPoints = 0;
    xVals.clear();
    yVals.clear();
  }
  
}

void RLinearInterpolant::SetPoints(vector<Float_t>& xIn, 
				       vector<Float_t>& yIn) {
  //Vectors are copied.  If xIn.size() != yIn.size()
  // nPoints is set to the smaller of the two, and only the first
  // nPoints values in each vector are copied.  An error message is also given
  //x is checked for monotinicity.  If not monotonic, read-in fails, an
  //error message is given, and the data is cleared

  //Clear old data
  nPoints = 0;
  xVals.clear();
  yVals.clear();

  bool isOrdered = true;
  Int_t nPointsIn = xIn.size();
  if(xIn.size() < yIn.size()) {
    nPointsIn = xIn.size();
    cerr << "Input arrays to RLinearInterpolant not same size, using size "
	 << nPointsIn << endl;
  }
  if(xIn.size() > yIn.size()) {
    nPointsIn = yIn.size();
    cerr << "Input arrays to RLinearInterpolant not same size, using size "
	 << nPointsIn << endl;
  }

  Double_t xLast = 0;
  nPoints = nPointsIn;
  if(nPointsIn != 0)
    xLast = xIn[0];
  for(int i=0; i < nPoints; i++) {
    xVals.push_back(xIn[i]);
    yVals.push_back(yIn[i]);
    if(xIn[i] < xLast) 
      isOrdered = false;
    xLast = xIn[i];
  }

  if(!isOrdered) {
    cerr << "x values not increasing, data not accepted!\n";
    nPoints = 0;
    xVals.clear();
    yVals.clear();
  }
  
}

Double_t RLinearInterpolant::Eval(Double_t x) {
  //Linearly interpolates between the nearest two data points to x
  //to find the corresponding y value
  //Returns 0 if x is out of range
  //For speed, assumes everything is ok.  Use EvalSafe if not sure!

  Int_t bin = FindBin(x);
  //If out-of-range, return 0
  if(bin == -1) 
    return 0;

  Double_t y = (x-xVals[bin])*(yVals[bin+1]-yVals[bin])/(xVals[bin+1]-xVals[bin]);
  y += yVals[bin];
  return y;
  
}

Double_t RLinearInterpolant::EvalSafe(Double_t x) {
  //Same as Eval, but checks for safety first.  Returns 0 if not safe
  if(IsZombie())
    return 0;
  return Eval(x);
}

Bool_t RLinearInterpolant::IsZombie() {
  //Returns true if there is a problem, false if the interpolation
  //is set up properly
  
  if(nPoints < 2 || xVals.size() == 0 || yVals.size() == 0)
    return true;
  else
    return false;
}

Int_t RLinearInterpolant::FindBin(Double_t x) {
  //Locates the bin with the property xVals[i] <= x < xVals[i+1]
  //Deals with the case with repeated xVals by choosing highest correct bin
  //Returns -1 if out-of-range, assumes at least two entries in xVals and yVals
  //Implemented via a binary search

  if(x < xVals[0] || x > xVals[nPoints-1])
    return -1;

  Int_t xLow=0, xHigh=nPoints-1;
  Int_t test=0;
  while(xLow < xHigh-1) {
    test = (xLow + xHigh)/2;
    if(x < xVals[test]) {
      xHigh = test;
    } else {
      xLow = test;
    }
  }
  return xLow;

}

void RLinearInterpolant::Print() {
  //Outputs the internal state of the interpolant, for diagnostics
  
  cout << "nPoints = " << nPoints << endl;

  for(int i=0; i < xVals.size(); i++) {
    cout << "xVals[" << i << "] = " << xVals[i] << endl;
  }

  for(int i=0; i < yVals.size(); i++) {
    cout << "yVals[" << i << "] = " << yVals[i] << endl;
  }

}
