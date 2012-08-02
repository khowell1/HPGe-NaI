//master class that will control everything :)

#include <math.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include "TRandom3.h"
#include "TGraphErrors.h"
#include "TAxis.h"
#include "TCanvas.h"
#include "TH1D.h"
#include "TSpline.h"
#include <string>
#include "RLinearInterpolant.h"
#include "TF1.h"
#include "RPhotonSource.h"
#include "Photon.h"
#include "Setup_Geometry.h"
#include "RLinearInterpolant.h"

using namespace std;

#ifndef MASTER_H
#define MASTER_H

class Master
{
 private: //variables


 public: //methods
  void Initializer();
  void Walk1Photon();
};

#endif
