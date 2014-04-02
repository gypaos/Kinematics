#include <iostream>
using namespace std;

#include "TBeam.h"
#include "TRandom.h"

ClassImp(TBeam)
  
TBeam::TBeam() : TNoyau()
{
   fEnergie      = 0;
   fPosX         = 0;
   fPosY         = 0;
   fResolEnergie = 0;
   fResolSpatial = 0;
}


TBeam::TBeam(const char* name) : TNoyau(name)
{
   // default constructor

   fEnergie      = 0;
   fPosX         = 0;
   fPosY         = 0;
   fResolEnergie = 0;
   fResolSpatial = 0;
}


TBeam::TBeam(const char* name, Double_t efais) : TNoyau(name)
{
   // default constructor

   fEnergie      = efais;
   fPosX         = 0;
   fPosY         = 0;
   fResolEnergie = 0;
   fResolSpatial = 0;
}


TBeam::TBeam(const char* name, Double_t efais, Double_t x, Double_t y, 
	     Double_t defais, Double_t drfais) : TNoyau(name)
{
   fEnergie      = efais;
   fPosX         = x;
   fPosY         = y;
   fResolEnergie = defais;
   fResolSpatial = drfais;
}


TBeam::TBeam(Int_t Z, Int_t A) : TNoyau(Z, A)
{
   fEnergie      = 0;
   fPosX         = 0;
   fPosY         = 0;
   fResolEnergie = 0;
   fResolSpatial = 0;
}


TBeam::TBeam(Int_t Z, Int_t A, Double_t efais) : TNoyau(Z, A)
{
   fEnergie = efais;
   fPosX = 0;
   fPosY = 0;
   fResolEnergie = 0;
   fResolSpatial = 0;
}


TBeam::TBeam(Int_t Z, Int_t A, Double_t efais, Double_t x, Double_t y, 
	     Double_t defais, Double_t drfais) : TNoyau(Z, A)
{
   fEnergie = efais;
   fPosX = x;
   fPosY = y;
   fResolEnergie = defais;
   fResolSpatial = drfais;
}


TBeam::~TBeam() 
{
}


Double_t TBeam::RealEnergie()
{
   Double_t RealEner = fEnergie +  gRandom->Gaus(0,1) * 0.425 * fResolEnergie * 1.e-3;
   
   return RealEner;
}


Double_t* TBeam::RealPosition()
{
   Double_t *RealPos = new Double_t[2];
   RealPos[0] = fPosX + gRandom->Gaus(0,1) * 0.425 * fResolSpatial;
   RealPos[1] = fPosY + gRandom->Gaus(0,1) * 0.425 * fResolSpatial;
   
   return RealPos;
}
   

void TBeam::Print() const
{
   TNoyau::Print();
   cout << endl;
   cout << "         Ebeam = " << fEnergie << " +- " << fResolEnergie/1000 << " MeV" << endl;
}
