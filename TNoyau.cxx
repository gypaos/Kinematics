#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <cmath>
using namespace std;

#include "TNoyau.h"
#include "Constant.h"


ClassImp(TNoyau)

TNoyau::TNoyau() 
   : fNom("unknown"),
     fMasse(0),
     fCharge(0),
     fExcesMasse(0),
     fExcitationEnergy(0),
     fDemiVie("unknown"),
     fSpin(0),
     fParite("unknown"),
     fEnergy(0),
     fMomentum(0),
     fWaveVector(0),
     fBeta(0),
     fGamma(0),
     fBrho(0),
     fTimeOfFlight(0)
{
   //----------- Constructeur par defaut ----------
   // initialize chart
   for (Int_t i = 0; i < 119; i++) {
      for (Int_t j = 0; j < 294; j++) {
         fChart[i][j] = 0;
      }
   }

   // on ouvre le fichier de donnees
   TString path = getenv("DATANUC");
   TString fileName = path + "/Masses/nubtab03.asc";
   ifstream fich;
   fich.open(fileName);
   if (!fich)
     cout << "Probleme d'ouverture dans le fichier " << fileName << endl;

   // on cherche la ligne correspondante dans le fichier de donnes
   Char_t line[256];
   TString s, s_masse, s_charge;
   Int_t masse, charge;
   while (!fich.eof()) {
      fich.getline(line, sizeof(line));
      s = line;
      s_masse = s(0,3);
      s_charge = s(4,4);
      masse = atoi(s_masse.Data());
      charge = floor(atoi(s_charge.Data())/10);
      fChart[charge][masse] = 1;
   }
}   



TNoyau::TNoyau(Int_t Z, Int_t A) 
{
   //----------- Constructeur avec des entiers ------------
   // (Z,A) charge et masse du noyau
   
   // on ouvre le fichier de donnees
   TString path = getenv("DATANUC");
   TString fileName = path + "/Masses/nubtab03.asc";
   ifstream fich;
   fich.open(fileName);
   if ( !fich )
     cout << "Probleme d'ouverture dans le fichier " << fileName << endl;

   // on cherche la ligne correspondante dans le fichier de donnes
   Char_t line[256];
   TString s, s_masse, s_charge;
   Int_t masse, charge;
   while ( !fich.eof() ) {
      fich.getline(line, sizeof(line));
      s = line;
      s_masse = s(0,3);
      s_charge = s(4,4);
      masse = atoi( s_masse.Data() );
      charge = atoi( s_charge.Data() );
      if ( (masse == A && charge == Z*10) ) break;
   }
   
   // on extrait les proprietes du noyau
   Extract(line);
}


TNoyau::TNoyau(const char* name) 
{
   //----------- Constructeur avec une chaine de caracteres ------------
   // name = "18F", "4He" ...
   
   // on ouvre le fichier de donnees
   TString path = getenv("DATANUC");
   TString fileName = path + "/Masses/nubtab03.asc";
   ifstream fich;
   fich.open(fileName);
   if ( !fich )
     cout << "Probleme d'ouverture dans le fichier " << fileName << endl;

   // on cherche la ligne correspondante dans le fichier de donnes
   Char_t line[256];
   TString s, nom;
   while ( !fich.eof() ) {
      fich.getline(line, sizeof(line));
      s = line;
      nom = s(11,6);
      nom = nom.Strip();
      if (nom.Contains(name) && nom.Length() == strlen(name)) break;
   }

   // on extrait les proprietes du noyau
   Extract(line);
}


TNoyau::~TNoyau() 
{
   //----------- Destructeur par defaut ------------
}




void TNoyau::Extract(char *line)
{
   // Extract() permet d'extraire les parametres du noyau, une fois qu'il est
   // trouve dans le fichier de donnees "nubtab97.asc"
   // Extract() est utilise par le constructeur
   
   TString s = (TString) line;

   // on teste si le noyau existe
   if ( s.IsNull() ) {
      cout << "Attention, ce noyau n'existe pas" << endl;
      fNom        = "unknown";
      fMasse      = 0;
      fCharge     = 0;
      fExcesMasse = 0;
      fDemiVie    = "unknown";
      fSpin       = 0;
      fParite     = "unknown";
   }
   else {
      // on extrait le nom
      fNom = s(11,6);
      fNom = fNom.Strip();

      // on extrait la masse et la charge
      TString s_masse = s(0,3);
      TString s_charge = s(4,4);
      fMasse = atoi( s_masse.Data() );
      fCharge = atoi( s_charge.Data() ); fCharge /= 10;

      // on extrait l'exces de masse
      TString s_exces = s(18,10);
      fExcesMasse = atof( s_exces.Data() );

      // on extrait la demi-vie
      fDemiVie = s(60,11);

      // on extrait le spin et la parite
      TString jpi = s(79,13);
      // parite
      if ( jpi.Contains("+") ) fParite = "+";
      if ( jpi.Contains("-") ) fParite = "-";
      if ( !jpi.Contains("+") && !jpi.Contains("-") ) fParite = "?";
      //spin
      if ( jpi.Contains("0") )     fSpin = 0;
      if ( jpi.Contains("1") )     fSpin = 1;
      if ( jpi.Contains("2") )     fSpin = 2;
      if ( jpi.Contains("3") )     fSpin = 3;
      if ( jpi.Contains("4") )     fSpin = 4;
      if ( jpi.Contains("5") )     fSpin = 5;
      if ( jpi.Contains("6") )     fSpin = 6;
      if ( jpi.Contains("7") )     fSpin = 7;
      if ( jpi.Contains("8") )     fSpin = 8;
      if ( jpi.Contains("9") )     fSpin = 9;
      if ( jpi.Contains("10") )    fSpin = 10;
      if ( jpi.Contains("1/2") )   fSpin = 0.5;
      if ( jpi.Contains("3/2") )   fSpin = 1.5;
      if ( jpi.Contains("5/2") )   fSpin = 2.5;
      if ( jpi.Contains("7/2") )   fSpin = 3.5;
      if ( jpi.Contains("9/2") )   fSpin = 4.5;
      if ( jpi.Contains("11/2") )  fSpin = 5.5;
      if ( jpi.Contains("13/2") )  fSpin = 6.5;
      if ( jpi.Contains("15/2") )  fSpin = 7.5;
      if ( jpi.Contains("17/2") )  fSpin = 8.5;
      if ( jpi.Contains("19/2") )  fSpin = 9.5;
      if ( jpi.Contains("21/2") )  fSpin = 10.5;
   }
}



bool TNoyau::IsKnown(Int_t charge, Int_t mass) const
{
   bool isKnown = false;
   if (fChart[charge][mass] == 1) isKnown = true;

   return isKnown;
}



Int_t TNoyau::FirstKnown(Int_t charge) const
{
   Int_t mass = 0;
   while (!IsKnown(charge, mass)) {
      mass++;
   }

   return mass;
}



Int_t TNoyau::LastKnown(Int_t charge) const
{
   Int_t mass = 277;
   while (!IsKnown(charge, mass)) {
      mass--;
   }

   return mass;
}



bool TNoyau::IsStable() const
{
   bool isStable = false;

   if (fDemiVie.Contains("stbl")) isStable = true;

   return isStable;
}



Double_t TNoyau::Mass() const
{
   // we assume ions to be fully stripped
   return (fMasse + fExcesMasse / 1000 / uma - me*fCharge) * uma;
   // we neglect electron binding energy
//   return (fMasse + fExcesMasse / 1000 / uma) * uma;
}



void TNoyau::EnergyToBrho()
{
   fBrho = sqrt(pow(fEnergy,2) + 2*fEnergy*Mass()) * 1e6 / C / fCharge;
}



void TNoyau::EnergyToMomentum()
{
   fMomentum = sqrt(pow(fEnergy,2) + 2*fEnergy*Mass());
}



void TNoyau::MomentumToWaveVector()
{
   fWaveVector = fMomentum / hbarc;
}



void TNoyau::BetaToGamma()
{
   fGamma = 1 / sqrt(1 - pow(fBeta, 2));
}



void TNoyau::BrhoToEnergy()
{
   Double_t P = fBrho / 1e6 * C * fCharge;
   vector<Double_t> E = QuadraticSolver(1, 2*Mass(), -pow(P,2));
   for (UInt_t i = 0; i < E.size(); i++) {
      if (E.at(i) > 0) fEnergy = E.at(i);
   }
}



vector<Double_t> TNoyau::QuadraticSolver(Double_t a, Double_t b, Double_t c) const
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



void TNoyau::Print() const
{
   //------------ Imprime a l'ecran les caracteristiques d'un noyau -------

   cout << endl;
   cout << fNom << " : " << "Z = " << fCharge << "  A = " << fMasse << "  Ex = " << fExcesMasse << " keV  " << fParite << endl;
   cout << "         T1/2 = " << fDemiVie << endl;
}
