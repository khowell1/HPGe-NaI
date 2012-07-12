#include "Photon.h"

using namespace std;

int main() {
  Photon photon;
  photon.SetCrossSection();
  photon.GetCrossSection();
  photon.PhotonStepper();
