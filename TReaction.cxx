#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <cstring>
#include <sstream>
#include <cmath>
using namespace std;

#include "TMath.h"
#include "TReaction.h"
#include "TString.h"
#include "TVector2.h"

#include "Constant.h"


ClassImp(TReaction)


TReaction::TReaction()
{
   //------------- Constructeur par defaut -------------

   fNoy1       = new TBeam();
   fNoy2       = new TNoyau();
   fNoy3       = new TNoyau();
   fNoy4       = new TNoyau();
   fThetaCM         = 0;
   fThetaLab        = 0;
   fExcitationLight = 0;
   fExcitationHeavy = 0;
   fQValue          = 0;

   Initialize();
}



TReaction::TReaction(const char* name1, const char* name2, const char* name3, const char* name4)
{
   //------------- Constructeur avec les noms des noyaux ------------
   
   fNoy1       = new TBeam(name1);
   fNoy2       = new TNoyau(name2);
   fNoy3       = new TNoyau(name3);
   fNoy4       = new TNoyau(name4);
   fThetaCM    = 0;
   fThetaLab   = 0;
   fExcitationLight = 0;
   fExcitationHeavy = 0;
   fBeamEnergy = 0;
   fQValue     = (  fNoy1->GetExcesMasse() + fNoy2->GetExcesMasse()
                  - fNoy3->GetExcesMasse() - fNoy4->GetExcesMasse()) / 1000;
   
   fMass1      = fNoy1->Mass();
   fMass2      = fNoy2->Mass();
   fMass3      = fNoy3->Mass();
   fMass4      = fNoy4->Mass();

   Int_t masse  = fNoy1->GetA() + fNoy2->GetA() - fNoy3->GetA() - fNoy4->GetA();
   Int_t charge = fNoy1->GetZ() + fNoy2->GetZ() - fNoy3->GetZ() - fNoy4->GetZ();
   if ( masse || charge ) cout << "Attention, conservation de la charge ou de la masse non respectee" << endl;

   Initialize();
}



TReaction::TReaction(const char* name1, const char* name2, const char* name3, const char* name4, Double_t efais)
{
   //------------- Constructeur avec les noms des noyaux ------------
     
   fNoy1       = new TBeam(name1);
   fNoy2       = new TNoyau(name2);
   fNoy3       = new TNoyau(name3);
   fNoy4       = new TNoyau(name4);
   fThetaCM    = 0;
   fThetaLab   = 0;
   fExcitationLight = 0;
   fExcitationHeavy = 0;
   fQValue     = (  fNoy1->GetExcesMasse() + fNoy2->GetExcesMasse()
		  - fNoy3->GetExcesMasse() - fNoy4->GetExcesMasse()) / 1000;

   fMass1      = fNoy1->Mass();
   fMass2      = fNoy2->Mass();
   fMass3      = fNoy3->Mass();
   fMass4      = fNoy4->Mass();

   // on stocke l'energie du faisceau
   fBeamEnergy = efais;
   fNoy1->SetEnergie(efais);
   
   Int_t masse  = fNoy1->GetA() + fNoy2->GetA() - fNoy3->GetA() - fNoy4->GetA();
   Int_t charge = fNoy1->GetZ() + fNoy2->GetZ() - fNoy3->GetZ() - fNoy4->GetZ();
   if ( masse || charge ) cout << "Attention, conservation de la charge ou de la masse non respectee" << endl;

   Initialize();
}



TReaction::TReaction(Int_t Z1, Int_t A1, Int_t Z2, Int_t A2, Int_t Z3, Int_t A3, Int_t Z4, Int_t A4)
{
   //------------- Constructeur avec les couples (Z,A) de chaque noyau --------
   
   fNoy1       = new TBeam(Z1, A1);
   fNoy2       = new TNoyau(Z2, A2);
   fNoy3       = new TNoyau(Z3, A3);
   fNoy4       = new TNoyau(Z4, A4);
   fThetaCM    = 0;
   fThetaLab   = 0;
   fExcitationLight = 0;
   fExcitationHeavy = 0;
   fBeamEnergy = 0;
   fQValue     = (  fNoy1->GetExcesMasse() + fNoy2->GetExcesMasse()
                  - fNoy3->GetExcesMasse() - fNoy4->GetExcesMasse()) / 1000;
   
   fMass1      = fNoy1->Mass();
   fMass2      = fNoy2->Mass();
   fMass3      = fNoy3->Mass();
   fMass4      = fNoy4->Mass();

   Int_t masse  = fNoy1->GetA() + fNoy2->GetA() - fNoy3->GetA() - fNoy4->GetA();
   Int_t charge = fNoy1->GetZ() + fNoy2->GetZ() - fNoy3->GetZ() - fNoy4->GetZ();
   if ( masse || charge ) cout << "Attention, conservation de la charge ou de la masse non respectee" << endl;

   Initialize();
}



TReaction::TReaction(Int_t Z1, Int_t A1, Int_t Z2, Int_t A2, Int_t Z3, Int_t A3, Int_t Z4, Int_t A4, Double_t efais)
{   
   //------------- Constructeur avec les couples (Z,A) de chaque noyau --------
   
   fNoy1       = new TBeam(Z1, A1);
   fNoy2       = new TNoyau(Z2, A2);
   fNoy3       = new TNoyau(Z3, A3);
   fNoy4       = new TNoyau(Z4, A4);
   fThetaCM    = 0;
   fThetaLab   = 0;
   fExcitationLight = 0;
   fExcitationHeavy = 0;
   fQValue     = (  fNoy1->GetExcesMasse() + fNoy2->GetExcesMasse()
		  - fNoy3->GetExcesMasse() - fNoy4->GetExcesMasse()) / 1000;
   
   fMass1      = fNoy1->Mass();
   fMass2      = fNoy2->Mass();
   fMass3      = fNoy3->Mass();
   fMass4      = fNoy4->Mass();

   // on stocke l'energie du faisceau
   fBeamEnergy = efais;
   fNoy1->SetEnergie(efais);
   
   Int_t masse  = fNoy1->GetA() + fNoy2->GetA() - fNoy3->GetA() - fNoy4->GetA();
   Int_t charge = fNoy1->GetZ() + fNoy2->GetZ() - fNoy3->GetZ() - fNoy4->GetZ();
   if ( masse || charge ) cout << "Attention, conservation de la charge ou de la masse non respectee" << endl;

   Initialize();
}



TReaction::TReaction(TBeam* noy1, TNoyau* noy2, TNoyau* noy3, TNoyau* noy4)
{
   //------------- Constructeur avec les objets TNoyaux -----------------------
   
   fNoy1       = noy1;
   fNoy2       = noy2;
   fNoy3       = noy3;
   fNoy4       = noy4;
   fThetaCM    = 0;
   fThetaLab   = 0;
   fExcitationLight = 0;
   fExcitationHeavy = 0;
   fBeamEnergy = fNoy1->GetEnergie();
   fQValue     = (  fNoy1->GetExcesMasse() + fNoy2->GetExcesMasse()
		  - fNoy3->GetExcesMasse() - fNoy4->GetExcesMasse()) / 1000;
   
   fMass1      = fNoy1->Mass();
   fMass2      = fNoy2->Mass();
   fMass3      = fNoy3->Mass();
   fMass4      = fNoy4->Mass();

   Int_t masse  = fNoy1->GetA() + fNoy2->GetA() - fNoy3->GetA() - fNoy4->GetA();
   Int_t charge = fNoy1->GetZ() + fNoy2->GetZ() - fNoy3->GetZ() - fNoy4->GetZ();
   if ( masse || charge ) cout << "Attention, conservation de la charge ou de la masse non conservee" << endl;

   Initialize();
}



TReaction::~TReaction()
{
   //------------- Destructeur par defaut ------------

   delete fNoy1;
   delete fNoy2;
   delete fNoy3;
   delete fNoy4;
}



void TReaction::Initialize()
{
   // Particle 3
   fLabAngle3     = -1;
   fLabEnergy3    = -1;
   fLabBeta3      = -1;
   fLabGamma3     = -1;
   fBrho3         = -1;
   fTimeOfFlight3 = -1;

   // Particle 4
   fLabAngle4     = -1;
   fLabEnergy4    = -1;
   fLabBeta4      = -1;
   fLabGamma4     = -1;
   fBrho4         = -1;
   fTimeOfFlight4 = -1;

   // Jacobian
   fJacobian      = -1;

   // check kinematics
   isValid = true;
}



/*Double_t TReaction::CoulombBarrier(Option_t* option) const
{
   // Barriere coulombienne en MeV
   // si option = "in",  calcul dans la voie d'entree
   //           = "out", calcul dans la voie de sortie

   Double_t R, Z1Z2;
   if ( option == "in" ) {
      R = fR0 * (TMath::Power(fNoy1->GetA(), 1./3.) + TMath::Power(fNoy2->GetA(), 1./3.));
      Z1Z2  = fNoy1->GetZ() * fNoy2->GetZ();
   }
   else {
      R = fR0 * (TMath::Power(fNoy3->GetA(), 1./3.) + TMath::Power(fNoy4->GetA(), 1./3.));
      Z1Z2  = fNoy3->GetZ() * fNoy4->GetZ();
   }

   return hbarc/alpha * Z1Z2 / R;
}*/


void TReaction::Cinematique(Double_t &ThetaLab3, Double_t &EnergieLab3,
			    Double_t &ThetaLab4, Double_t &EnergieLab4)
{
// Cinematique a 2 corps directe et inverse (non-relativiste)
// EnergieLab3,4 : energie des noyaux dans la voie de sortie en MeV
// ThetaLab3,4   : angle des noyaux dans la voie de sortie en rad

   // reset result members
   Initialize();

//   cout << "Cinematique non relativiste" << endl;
   Double_t m1 = fMass1;
   Double_t m2 = fMass2;
   Double_t m3 = fMass3 + fExcitationLight;;
   Double_t m4 = fMass4 + fExcitationHeavy;
//   cout << m1 << " " << m2 << " " << m3 << " " << m4 <<endl;

//   cout << " Nouveau : " << endl;
   Double_t TdispCM = (Double_t) m2 / (m1+m2) * fNoy1->GetEnergie();
   Double_t Ttrans  = TdispCM + fQValue - (fExcitationLight+fExcitationHeavy);
//   cout << TdispCM << " " << Ttrans << endl;

   Double_t T3cm = (Double_t) m4 / (m3+m4) * Ttrans;
   Double_t T4cm = (Double_t) m3 / (m3+m4) * Ttrans;
//   cout << m1*m3*TdispCM / (m2*m4*Ttrans) << endl;
   Double_t K3 = sqrt(m1*m3*TdispCM / (m2*m4*Ttrans));
   Double_t K4 = sqrt(m1*m4*TdispCM / (m2*m3*Ttrans));
//   cout << K3 << " " << K4 << endl;
   
   // cas de la cinematique inverse
   Double_t theta = fThetaCM;
   if (m1 > m2) theta = TMath::Pi() - fThetaCM;
//   cout << TMath::Pi() << " " << TMath::Sqrt(4.) << endl;
//   cout << "angle " << theta << endl;

   // angles dans le laboratoire
   fLabAngle3 = atan(sin(theta) / (cos(theta) + K3));
//   cout << "theta 3 : " << fLabAngle3 * 180/3.14 << endl;
   if (fLabAngle3 < 0) fLabAngle3 += TMath::Pi();
   fLabAngle4 = atan(sin(TMath::Pi() + theta) / (cos(TMath::Pi() + theta) + K4));
//   cout << "theta 4 : " << fLabAngle4 * 180/3.14 << endl;
   if (fabs(fLabAngle3) < 1e-6) fLabAngle3 = 0;
   fLabAngle4 = fabs(fLabAngle4);
   if (fabs(fLabAngle4) < 1e-6) fLabAngle4 = 0;
   ThetaLab3 = fLabAngle3;
   ThetaLab4 = fLabAngle4;
   
   // energies dans le laboratoire
   fLabEnergy3 = T3cm * (1. + pow(K3,2) + 2.*K3*cos(theta));
   fLabEnergy4 = T4cm * (1. + pow(K4,2) + 2.*K4*cos(theta + TMath::Pi()));
   EnergieLab3 = fLabEnergy3;
   EnergieLab4 = fLabEnergy4;
   
   // beta's and gamma's
   fLabBeta3 = sqrt(2 * fLabEnergy3 / fMass3);
   fLabBeta4 = sqrt(2 * fLabEnergy4 / fMass4);
   fLabGamma3 = 1 / sqrt(1 - pow(fLabBeta3,2));
   fLabGamma4 = 1 / sqrt(1 - pow(fLabBeta4,2));
//   cout << fLabAngle3 << " " << EnergieLab3 << " " << fLabAngle4 << " " << EnergieLab4 << endl;
}



void TReaction::NonRelativisticKinematics()
{
// Non relativistic two-body direct and inverse kinematics

   // reset result members
   Initialize();

//   cout << "Cinematique non relativiste" << endl;
   Double_t m1 = fMass1;
   Double_t m2 = fMass2;
   Double_t m3 = fMass3 + fExcitationLight;
   Double_t m4 = fMass4 + fExcitationHeavy;
//   cout << m1 << " " << m2 << " " << m3 << " " << m4 <<endl;

//   cout << " Nouveau : " << endl;
   Double_t TdispCM = (Double_t) m2 / (m1+m2) * fNoy1->GetEnergie();
   Double_t Ttrans  = TdispCM + fQValue - (fExcitationLight+fExcitationHeavy);
//   cout << TdispCM << " " << Ttrans << endl;

   Double_t T3cm = (Double_t) m4 / (m3+m4) * Ttrans;
   Double_t T4cm = (Double_t) m3 / (m3+m4) * Ttrans;
//   cout << m1*m3*TdispCM / (m2*m4*Ttrans) << endl;
   Double_t K3 = sqrt(m1*m3*TdispCM / (m2*m4*Ttrans));
   Double_t K4 = sqrt(m1*m4*TdispCM / (m2*m3*Ttrans));
//   cout << K3 << " " << K4 << endl;

   // cas de la cinematique inverse
   Double_t theta = fThetaCM;
   if (m1 > m2) theta = TMath::Pi() - fThetaCM;
//   cout << TMath::Pi() << " " << TMath::Sqrt(4.) << endl;
//   cout << "angle " << theta << endl;

   // angles dans le laboratoire
   fLabAngle3 = atan(sin(theta) / (cos(theta) + K3));
//   cout << "theta 3 : " << fLabAngle3 * 180/3.14 << endl;
   if (fLabAngle3 < 0) fLabAngle3 += TMath::Pi();
   fLabAngle4 = atan(sin(TMath::Pi() + theta) / (cos(TMath::Pi() + theta) + K4));
//   cout << "theta 4 : " << fLabAngle4 * 180/3.14 << endl;
   if (fabs(fLabAngle3) < 1e-6) fLabAngle3 = 0;
   fLabAngle4 = fabs(fLabAngle4);
   if (fabs(fLabAngle4) < 1e-6) fLabAngle4 = 0;

   // energies dans le laboratoire
   fLabEnergy3 = T3cm * (1. + pow(K3,2) + 2.*K3*cos(theta));
   fLabEnergy4 = T4cm * (1. + pow(K4,2) + 2.*K4*cos(theta + TMath::Pi()));

   // beta's and gamma's
   fLabBeta3 = sqrt(2 * fLabEnergy3 / fMass3);
   fLabBeta4 = sqrt(2 * fLabEnergy4 / fMass4);
   fLabGamma3 = 1 / sqrt(1 - pow(fLabBeta3,2));
   fLabGamma4 = 1 / sqrt(1 - pow(fLabBeta4,2));
}




void TReaction::NonRelativisticLabKinematics()
{
   // reset result members
   Initialize();

   // calculate masses
   Double_t m1 = fMass1;
//   Double_t m2 = fMass2;
   Double_t m3 = fMass3 + fExcitationLight;
   Double_t m4 = fMass4 + fExcitationHeavy;

   // case of light ejectile (index 3)
   // angle
   fLabAngle3 = fThetaLab;

   // energy
   Double_t a3 = 1 + m3/m4;
   Double_t b3 = -2*sqrt(m1*m3)/m4*sqrt(fBeamEnergy)*cos(fThetaLab);
   Double_t c3 = -(fQValue - fExcitationLight - fExcitationHeavy) - fBeamEnergy*(1 - m1/m4);
   vector<Double_t> energy = QuadraticSolver(a3, b3, c3);
   // check if solutions exist
   if (energy.size() > 0) {
      for (UInt_t i = 0; i < energy.size(); ++i) {   // loop on energy solutions
         if (energy.at(i) > 0) {
            fLabEnergy3 = pow(energy.at(i), 2);
         }
      } // end loop on energy solutions
   }

   // case of beam-like ejectile (index 4)
   // angle

   // energy

   // beta's and gamma's
   fLabBeta3 = sqrt(2 * fLabEnergy3 / fMass3);
   fLabBeta4 = sqrt(2 * fLabEnergy4 / fMass4);
   fLabGamma3 = 1 / sqrt(1 - pow(fLabBeta3,2));
   fLabGamma4 = 1 / sqrt(1 - pow(fLabBeta4,2));

   // brhos and tof
   NonRelativisticBrho();
   NonRelativisticTimeOfFlight();
}




Double_t TReaction::ReconstructExcitation(Double_t EnergieLab, Double_t ThetaLab) const
{
   Double_t m1 = fMass1;
   Double_t m3 = fMass3;
   Double_t m4 = fMass4;

   Double_t E_exc = EnergieLab * (1. + (Double_t)m3 / m4) - fBeamEnergy * (1. - (Double_t)m1 / m4) -
     2. * sqrt((Double_t)m1 * m3) / m4 * sqrt(fBeamEnergy * EnergieLab) * cos(ThetaLab);

   return fQValue - E_exc;
}



void TReaction::KinRelat(Double_t &ThetaLab3, Double_t &EnergieLab3,
			 Double_t &ThetaLab4, Double_t &EnergieLab4)
{
// Cinematique a 2 corps directe et inverse (non-relativiste)
// EnergieLab3,4 : energie des noyaux dans la voie de sortie en MeV
// ThetaLab3,4   : angle des noyaux dans la voie de sortie en rad

   // reset result members
   Initialize();

//   cout << "Cinematique relativiste" << endl;
   // masses exactes en MeV
   Double_t m1 = fMass1;
   Double_t m2 = fMass2;
   Double_t m3 = fMass3 + fExcitationLight;
   Double_t m4 = fMass4 + fExcitationHeavy;
//   cout << m1 << " " << m2 << " " << m3 << " " << m4 <<endl;

   // vitesse du centre de masse
   Double_t WtotLab = (fNoy1->GetEnergie() + m1) + m2;
   Double_t P1 = sqrt(pow(fNoy1->GetEnergie(),2) + 2*m1*fNoy1->GetEnergie());
   Double_t B = P1 / WtotLab;
   Double_t G = 1 / sqrt(1 - pow(B,2));
//   Double_t WtotCM = WtotLab / G - (fExcitationLight+fExcitationHeavy);
//   cout << WtotLab << "  " << B << "  " << G << "  " << WtotCM << endl;

   // energie totale des ejectiles dans le centre de masse
   Double_t W3cm = (pow(WtotLab,2) + pow(G,2)*(pow(m3,2) - pow(m4,2)))
   		   / (2 * G * WtotLab);
   Double_t W4cm = (pow(WtotLab,2) + pow(G,2)*(pow(m4,2) - pow(m3,2)))
   		   / (2 * G * WtotLab);
//   cout << W3cm << "   " << W4cm << endl;

   // vitesse des ejectiles dans le centre de masse
   Double_t beta3cm  = sqrt(1 - pow(m3,2)/pow(W3cm,2));
   Double_t beta4cm  = sqrt(1 - pow(m4,2)/pow(W4cm,2));
//   cout << beta3cm << "  " << beta4cm << endl;

   // constantes du mouvement
   Double_t K3 = B / beta3cm;
   Double_t K4 = B / beta4cm;
//   cout << K3 << " " << K4 << endl;
   
   // cas de la cinematique inverse
   Double_t theta = fThetaCM;
   if (m1 > m2) theta = TMath::Pi() - fThetaCM;
//   cout << TMath::Pi() << " " << TMath::Sqrt(4.) << endl;
//   cout << "angle " << theta << endl;

   // angles dans le laboratoire
   fLabAngle3 = atan(sin(theta) / (cos(theta) + K3) / G);
//   cout << "theta 3 : " << fLabAngle3 * 180/M_PI << endl;
   if (fLabAngle3 < 0) fLabAngle3 += TMath::Pi();
   fLabAngle4 = atan(sin(TMath::Pi() + theta) / (cos(TMath::Pi() + theta) + K4) / G);
//   cout << "theta 4 : " << fLabAngle4 * 180/M_PI << endl;
   if (fabs(fLabAngle3) < 1e-6) fLabAngle3 = 0;
   fLabAngle4 = fabs(fLabAngle4);
   if (fabs(fLabAngle4) < 1e-6) fLabAngle4 = 0;
   ThetaLab3 = fLabAngle3;
   ThetaLab4 = fLabAngle4;
   
   // energies totales et cinetiques dans le laboratoire
   Double_t W3lab = W3cm * G * (1 + B*beta3cm*cos(theta));
   Double_t W4lab = W4cm * G * (1 + B*beta4cm*cos(theta + TMath::Pi()));
   // on test la conservation de l'energie totale
   if (fabs(WtotLab - (W3lab+W4lab)) > 1e-6) 
      cout << "Attention, probleme de conservation de l'energie" << endl;
   fLabEnergy3 = W3lab - m3;
   fLabEnergy4 = W4lab - m4;
   EnergieLab3 = fLabEnergy3;
   EnergieLab4 = fLabEnergy4;
   
   // beta's and gamma's
   fLabBeta3 = sqrt(1 - pow(m3,2)/pow(W3lab,2));
   fLabBeta4 = sqrt(1 - pow(m4,2)/pow(W4lab,2));
   fLabGamma3 = W3lab / m3;
   fLabGamma4 = W4lab / m4;
//   cout << fLabAngle3 << " " << EnergieLab3 << " " << fLabAngle4 << " " << EnergieLab4 << endl;
}



void TReaction::RelativisticKinematics()
{
// Relativistic direct and inverse two-body kinematics

   // reset result members
   Initialize();

//   cout << "Cinematique relativiste" << endl;
   // atomic masses in MeV/c2
   Double_t m1 = fMass1;
   Double_t m2 = fMass2;
   Double_t m3 = fMass3 + fExcitationLight;
   Double_t m4 = fMass4 + fExcitationHeavy;
//   cout << m1 << " " << m2 << " " << m3 << " " << m4 <<endl;

   // available energy in the c.m.
   Double_t WtotLab = (fNoy1->GetEnergie() + m1) + m2;
   Double_t P1 = sqrt(pow(fNoy1->GetEnergie(),2) + 2*m1*fNoy1->GetEnergie());
   Double_t B = P1 / WtotLab;
   Double_t G = 1 / sqrt(1 - pow(B,2));
//   Double_t WtotCM = WtotLab / G - (fExcitationLight+fExcitationHeavy);
//   cout << WtotLab << "  " << B << "  " << G << "  " << WtotCM << endl;

   // total ejectile energy in the c.m.
   Double_t W3cm = (pow(WtotLab,2) + pow(G,2)*(pow(m3,2) - pow(m4,2)))
                   / (2 * G * WtotLab);
   Double_t W4cm = (pow(WtotLab,2) + pow(G,2)*(pow(m4,2) - pow(m3,2)))
                   / (2 * G * WtotLab);
//   cout << W3cm << "   " << W4cm << endl;

   // ejectile velocity in the c.m.
   Double_t beta3cm  = sqrt(1 - pow(m3,2)/pow(W3cm,2));
   Double_t beta4cm  = sqrt(1 - pow(m4,2)/pow(W4cm,2));
   if ((pow(m3,2)/pow(W3cm,2) > 1) || (pow(m4,2)/pow(W4cm,2) > 1)) isValid = false;
//   cout << beta3cm << "  " << beta4cm << endl;

   // constantes du mouvement
   Double_t K3 = B / beta3cm;
   Double_t K4 = B / beta4cm;
//   cout << K3 << " " << K4 << endl;

   // cas de la cinematique inverse
   Double_t theta = fThetaCM;
   if (m1 > m2) theta = TMath::Pi() - fThetaCM;
//   cout << "angle " << theta << endl;

   // angles dans le laboratoire
   fLabAngle3 = atan(sin(theta) / (cos(theta) + K3) / G);
   if (fLabAngle3 < 0) fLabAngle3 += TMath::Pi();
   fLabAngle4 = atan(sin(TMath::Pi() + theta) / (cos(TMath::Pi() + theta) + K4) / G);
   if (fabs(fLabAngle3) < 1e-6) fLabAngle3 = 0;
   fLabAngle4 = fabs(fLabAngle4);
   if (fabs(fLabAngle4) < 1e-6) fLabAngle4 = 0;

   // total and kinetic energy in the lab
   Double_t W3lab = W3cm * G * (1 + B*beta3cm*cos(theta));
   Double_t W4lab = W4cm * G * (1 + B*beta4cm*cos(theta + TMath::Pi()));
   // test for total energy conservation
   if (fabs(WtotLab - (W3lab+W4lab)) > 1e-6)
      cout << "!!! problem with total energy conservation" << endl;
   fLabEnergy3 = W3lab - m3;
   fLabEnergy4 = W4lab - m4;

   // beta's and gamma's
   fLabBeta3 = sqrt(1 - pow(m3,2)/pow(W3lab,2));
   fLabBeta4 = sqrt(1 - pow(m4,2)/pow(W4lab,2));
   fLabGamma3 = W3lab / m3;
   fLabGamma4 = W4lab / m4;

   // Jacobian
   fJacobian = G * (1 + K3*cos(theta)) / pow(pow(G*(K3+cos(theta)), 2) + pow(sin(theta), 2), 1.5); 
}



void TReaction::RelativisticLabKinematics()
{
   // reset result members
   Initialize();

   // calculate masses
   Double_t m1 = fMass1;
   Double_t m2 = fMass2;
   Double_t m3 = fMass3 + fExcitationLight;
   Double_t m4 = fMass4 + fExcitationHeavy;
  
   // Q-value
   Double_t QValue = fQValue - fExcitationLight - fExcitationHeavy;

   // available energy in the c.m.
   Double_t WtotLab = (fNoy1->GetEnergie() + m1) + m2;
   Double_t P1 = sqrt(pow(fNoy1->GetEnergie(),2) + 2*m1*fNoy1->GetEnergie());
   Double_t B = P1 / WtotLab;
   Double_t G = 1 / sqrt(1 - pow(B,2));
 
   // case of light ejectile (index 3)
   // angle
   fLabAngle3 = fThetaLab;
   // total ejectile energy in the c.m.
   Double_t W3cm = (pow(WtotLab,2) + pow(G,2)*(pow(m3,2) - pow(m4,2)))
                   / (2 * G * WtotLab);
   // ejectile velocity in the c.m.
   Double_t beta3cm  = sqrt(1 - pow(m3,2)/pow(W3cm,2));
   // constantes du mouvement
   Double_t K3 = B / beta3cm;
  
   // energy
   Double_t P12 = 2*m1*fBeamEnergy + pow(fBeamEnergy,2);
   Double_t A3 = pow(QValue,2) + 2*QValue*(m4+fBeamEnergy) + 2*fBeamEnergy*(m4-m1);
   Double_t B3 = 2 * (QValue + fBeamEnergy + m3 + m4);
   Double_t a3 = pow(B3, 2) - 4*P12*pow(cos(fLabAngle3), 2);
   Double_t b3 = -2*A3*B3 - 2*m3*4*P12*pow(cos(fLabAngle3), 2);
   Double_t c3 = pow(A3,2);
   vector<Double_t> energy = QuadraticSolver(a3, b3, c3);
   // ensure that solution exists (e.g. enough energy available in the c.m.)
   if (energy.size() > 0) {
      fLabEnergy3 = (fLabAngle3 < TMath::Pi()/2) ? energy[1] : energy[0];
   }

   // case of beam-like ejectile (index 4)
   // angle

   // energy

   // beta's and gamma's
   Double_t W3lab = fLabEnergy3 + m3;
   Double_t W4lab = fLabEnergy4 + m4;
   fLabBeta3 = sqrt(1 - pow(m3,2)/pow(W3lab,2));
   fLabBeta4 = sqrt(1 - pow(m4,2)/pow(W4lab,2));
   fLabGamma3 = W3lab / m3;
   fLabGamma4 = W4lab / m4;

   // brhos and tof
   RelativisticBrho();
   RelativisticTimeOfFlight();

   // Jacobian
   Double_t K3prim = B / fLabBeta3;
   fThetaCM = atan(sin(fLabAngle3) / (G * (cos(fLabAngle3) - K3prim)));
   if (fThetaCM < 0) fThetaCM += TMath::Pi();
   fJacobian = G * (1 + K3*cos(fThetaCM)) / pow(pow(G*(K3+cos(fThetaCM)), 2) + pow(sin(fThetaCM), 2), 1.5);
}





Double_t TReaction::ReconstructExcitationRelat(Double_t EnergieLab, Double_t ThetaLab) const
{
   Double_t P1  = sqrt(2*fMass1*fBeamEnergy + pow(fBeamEnergy,2));
   Double_t P3  = sqrt(2*fMass3*EnergieLab + pow(EnergieLab,2));
   Double_t P4  = sqrt(pow(P1,2) + pow(P3,2) - (2*P1*P3*cos(ThetaLab)));
   Double_t E4  = fBeamEnergy + fMass1 + fMass2 - (EnergieLab + fMass3);
   Double_t m4e = sqrt(pow(E4,2) - pow(P4,2));
   Double_t Eex = m4e - fMass4;

   return Eex;
}



Double_t TReaction::ReconstructExcitationRelatBrho(Double_t BrhoLab, Double_t ThetaLab) const
{
   Double_t Eex = -1;
   Double_t P1  = sqrt(2*fMass1*fBeamEnergy + pow(fBeamEnergy,2));
   Double_t P3  = BrhoLab / 1e6 * C * fNoy3->GetZ();
   vector<Double_t> E3 = QuadraticSolver(1, 2*fMass3, -pow(P3,2));
   for (UInt_t i = 0; i < E3.size(); i++) {
      if (E3.at(i) > 0) {
         Double_t P4  = sqrt(pow(P1,2) + pow(P3,2) - (2*P1*P3*cos(ThetaLab)));
         Double_t E4  = fBeamEnergy + fMass1 + fMass2 - (E3.at(i) + fMass3);
         Double_t m4e = sqrt(pow(E4,2) - pow(P4,2));
         Eex = m4e - fMass4;
      }
   }

   return Eex;
}



void TReaction::NonRelativisticBrho()
{
   fBrho3 = fMass3*1e6 / pow(C,2) * echarge / (fNoy3->GetZ() * echarge) * fLabBeta3*C; 
   fBrho4 = fMass4*1e6 / pow(C,2) * echarge / (fNoy4->GetZ() * echarge) * fLabBeta4*C; 
}



void TReaction::RelativisticBrho()
{
   fBrho3 = sqrt(pow(fLabEnergy3,2) + 2*fLabEnergy3*fMass3) * 1e6 * echarge / C / (fNoy3->GetZ() * echarge);
   fBrho4 = sqrt(pow(fLabEnergy4,2) + 2*fLabEnergy4*fMass4) * 1e6 * echarge / C / (fNoy4->GetZ() * echarge);
}



void TReaction::NonRelativisticTimeOfFlight()
{

   fTimeOfFlight3 = 1e9 / C / fLabBeta3;
   fTimeOfFlight4 = 1e9 / C / fLabBeta4;
}



void TReaction::RelativisticTimeOfFlight()
{
   NonRelativisticTimeOfFlight();
}



Double_t TReaction::RelativisticKinematicFactor(Double_t Bfield, Double_t LabAngle, Double_t DeltaLabAngle)
{
   // theta + dtheta/2
   SetThetaLab(TMath::Pi()/180*(LabAngle+DeltaLabAngle/2));
   RelativisticLabKinematics();
   RelativisticBrho();
   Double_t brhop = GetBrho3();
   // theta - dtheta/2
   SetThetaLab(TMath::Pi()/180*(LabAngle-DeltaLabAngle/2));
   RelativisticLabKinematics();
   RelativisticBrho();
   Double_t brhom = GetBrho3();

   return 0.6125*1000*(brhop-brhom)/Bfield/(TMath::Pi()/180*DeltaLabAngle);
}




void TReaction::SameBrho(const char* ion, Double_t brho) const
{
   const Int_t num = 7;
   const char* nucleus[num] = {"1H", "2H", "3H", "3He", "4He", "6Li", "7Li"};

   TNoyau ref_ion(ion);
   Double_t ref_energy = pow(brho * ref_ion.GetZ() * C / 1e6, 2) / 2 / ref_ion.Mass();

   cout << "reference ion: " << ref_ion.GetNom() << "(" << brho << " T.m, " << ref_energy << " MeV)" << endl;

   for (Int_t i = 0; i < num; i++) {
      TNoyau dum(nucleus[i]);
      Double_t energy = ref_energy * pow(dum.GetZ()/(Double_t)ref_ion.GetZ(), 2) * ref_ion.Mass()/dum.Mass();
      cout << dum.GetNom() << "  " << dum.GetZ() << "  " << dum.GetA() << "  " << energy << " MeV" << endl;
   }
}



Double_t TReaction::Rutherford() const
{
   // Calcul de la section efficace de Rutherford (mb/sr) si diffusion elastique
   
   Double_t TdispCM = (Double_t) fNoy2->GetA() / (fNoy1->GetA()+fNoy2->GetA()) * fNoy1->GetEnergie();
   
   Double_t coef1 = 1. / 16. * pow(hbarc/alpha, 2) *
                               pow(fNoy1->GetZ()*fNoy2->GetZ()/TdispCM, 2);
   Double_t coef2 = 1. / pow(sin(fThetaCM/2.), 4);
   
   return coef1 * coef2 * 10;    // *10 car 1 fm^2 = 10 mb
}



Double_t TReaction::Rutherford(Double_t theta1, Double_t theta2) const
{
   // Section efficace de Rutherford integree (mb) entre 2 angles (rad)
   // theta1 < theta2

   Double_t TdispCM = (Double_t) fNoy2->GetA() / (fNoy1->GetA()+fNoy2->GetA()) * fNoy1->GetEnergie();
   
   Double_t coef1 = TMath::Pi() / 4. * pow(hbarc/alpha, 2) *
                                  pow(fNoy1->GetZ()*fNoy2->GetZ()/TdispCM, 2);
   Double_t coef2 = 1. / pow(tan(theta1/2.), 2) - 1. / pow(tan(theta2/2.), 2);
   
   return coef1 * coef2 * 10;    // *10 car 1 fm^2 = 10 mb
}



Double_t TReaction::LimitingAngle() const
{
   Double_t m1 = fMass1;
   Double_t m2 = fMass2;
   Double_t m3 = fMass3 + fExcitationLight;
   Double_t m4 = fMass4 + fExcitationHeavy;

   Double_t TdispCM = (Double_t) m2 / (m1+m2) * fNoy1->GetEnergie();
   Double_t Ttrans  = TdispCM + fQValue - (fExcitationLight+fExcitationHeavy);

   Double_t K3 = sqrt(m1*m3*TdispCM / (m2*m4*Ttrans));

   Double_t ThetaMax = -10;
   if (pow(K3,2) > 1) ThetaMax = atan(1 / sqrt(pow(K3,2) - 1));

   return ThetaMax;
}



Double_t TReaction::Mu() const
{
   Double_t m1 = (Double_t)fNoy1->GetA() + fNoy1->GetExcesMasse() / uma / 1000;
   Double_t m2 = (Double_t)fNoy2->GetA() + fNoy2->GetExcesMasse() / uma / 1000;
   
   return m1*m2 / (m1+m2);
}



Double_t TReaction::Rn() const
{
   return 1.4 * (TMath::Power(fNoy1->GetA(), 1./3.) + TMath::Power(fNoy2->GetA(), 1./3.));
}



Double_t TReaction::Rho(Double_t energy) const
{
   Double_t k = TMath::Sqrt(2 * Mu()*uma * energy / TMath::Power(hbarc,2));

   return k*Rn();
}



Double_t TReaction::Rho(Double_t energy, Double_t a) const
{
   Double_t k = TMath::Sqrt(2 * Mu()*uma * energy / TMath::Power(hbarc,2));

   return k*a;
}



Double_t TReaction::Eta(Double_t energy) const
{
   Double_t eta = 1./alpha * fNoy1->GetZ()*fNoy2->GetZ() *
      TMath::Sqrt(Mu()*uma / 2 / energy);

   return eta;
}



Double_t TReaction::WaveVector(UInt_t channel) const
{
   // entrance channel
   Double_t mu = fMass1*fMass2 / (fMass1 + fMass2);
   Double_t TdispCM = fMass2 / (fMass1 + fMass2) * fBeamEnergy;


   if (channel) {   // exit channel
      mu = fMass3*fMass4 / (fMass3 + fMass4);
      TdispCM = TdispCM + fQValue - fExcitationLight - fExcitationHeavy;
   }

   return sqrt(pow(TdispCM,2) + 2*TdispCM*mu) / hbarc;
}



Double_t TReaction::QTransfered() const
{
   TVector2 ki(WaveVector(), 0);
   TVector2 kf;
   kf.SetMagPhi(WaveVector(1), fThetaCM);

   TVector2 qtr = kf - ki;

   return qtr.Mod();
}



Double_t TReaction::GrazingAngle() const
{
   Double_t TdispCM = (Double_t) fNoy2->GetA() / (fNoy1->GetA()+fNoy2->GetA()) * fNoy1->GetEnergie();

   // A. Di Pietro (pptx; An experimental view of elastic and inelastic scattering: kinematics
//   return 2 * asin(Eta(TdispCM) / (Rho(TdispCM) - Eta(TdispCM))) * 180 / M_PI;

   return 2 * atan(Eta(TdispCM)/GrazingAngularMomenta()) * 180 / M_PI;
}



Double_t TReaction::GrazingAngularMomenta() const
{
   Double_t result = 0;

   Double_t TdispCM = (Double_t) fNoy2->GetA() / (fNoy1->GetA()+fNoy2->GetA()) * fNoy1->GetEnergie();

   Double_t a = 1;
   Double_t b = 1;
   Double_t c = -Rho(TdispCM) * (Rho(TdispCM) - 2*Eta(TdispCM));
   vector<Double_t> L0 = QuadraticSolver(a, b, c);
   // check if solutions exist
   if (L0.size() > 0) {
      for (UInt_t i = 0; i < L0.size(); ++i) {   // loop on energy solutions
         cout << L0[i] <<  '\n';
         if (L0.at(i) > 0) {
            result = L0[i];
         }
      } // end loop on energy solutions
   }

   return result;
}



Double_t TReaction::LMatching() const
{
   return QTransfered() * 1.25*(pow(fNoy1->GetA(), 1./3.) + pow(fNoy2->GetA(), 1./3.));
}



TString TReaction::GetReactionName()
{
   TString name = Form("%s(%s,%s)%s", fNoy2->GetNom(), fNoy1->GetNom(), fNoy3->GetNom(), fNoy4->GetNom());

   return name;
}



vector<Double_t> TReaction::QuadraticSolver(Double_t a, Double_t b, Double_t c) const
{
   // Function to solve ax^2 + bx + c = 0

   vector<Double_t> result;

   Double_t delta = pow(b, 2) - 4*a*c;
   if (delta > 0) {
      result.push_back((-b - sqrt(delta)) / 2 / a);
      result.push_back((-b + sqrt(delta)) / 2 / a);
   }

   return result;
}



void TReaction::Print() const
{
   // Affiche les caracteristiques de la reaction

   cout << "Reaction : " << fNoy2->GetNom() << "(" << fNoy1->GetNom() <<
     "," << fNoy3->GetNom() << ")" << fNoy4->GetNom() << "  @  " << 
     fNoy1->GetEnergie() << " MeV" << endl;
   cout << "Excitation Light = " << fExcitationLight << " MeV" << endl;
   cout << "Excitation Heavy = " << fExcitationHeavy << " MeV" << endl;
   cout << "Qgg = " << fQValue << " MeV" << endl;
   cout << "BeamEnergy = " << fBeamEnergy << " MeV" << endl;
}



void TReaction::Dump() const
{
   // Masses
   cout << "Masses: " << endl;
   cout << "\tm1: " << fMass1 << endl;
   cout << "\tm2: " << fMass2 << endl;
   cout << "\tm3: " << fMass3 << endl;
   cout << "\tm4: " << fMass4 << endl;
   cout << endl;

   // Kinematics variables
   cout << "Particle 3: (lab frame)" << endl;
   cout << "\tangle:    " << fLabAngle3 * 180/M_PI << " deg" << endl;
   cout << "\tenergy:   " << fLabEnergy3 << " MeV" << endl;
   cout << "\tbeta:     " << fLabBeta3 << endl;
   cout << "\tgamma:    " << fLabGamma3 << endl;
   cout << "\tbrho:     " << fBrho3 << " T.m" << endl;
   cout << "\ttof:      " << fTimeOfFlight3 << " ns/m" << endl;
   cout << "\tjacobian: " << fJacobian << endl;
   cout << endl;
   cout << "Particle 4: (lab frame)" << endl;
   cout << "\tangle:    " << fLabAngle4 * 180/M_PI << " deg" << endl;
   cout << "\tenergy:   " << fLabEnergy4 << " MeV" << endl;
   cout << "\tbeta:     " << fLabBeta4 << endl;
   cout << "\tgamma:    " << fLabGamma4 << endl;
   cout << "\tbrho:     " << fBrho4 << " T.m" << endl;
   cout << "\ttof:      " << fTimeOfFlight4 << " ns/m" << endl;
}
