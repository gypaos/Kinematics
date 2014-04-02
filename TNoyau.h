#ifndef __Noyau__
#define __Noyau__

#include "TObject.h"
#include "TString.h"

#include <vector>
using namespace std;


class TNoyau : public TObject {
 private:
    // intrinsic properties
   TString  fNom;                // Nom du Noyau
   Int_t    fMasse;		         // Nombre de masse 
   Int_t    fCharge;             // Charge
   Double_t fExcesMasse;         // Exces de masse (keV)
   Double_t fExcitationEnergy;   // Excitation energy in MeV
   TString  fDemiVie;            // Demi-vie (sec)
   Double_t fSpin;               // Spin
   TString  fParite;             // Parite
   Int_t    fChart[119][294];	 // Isotope chart
   // knematic properties
   Double_t fEnergy;             // in MeV
   Double_t fMomentum;           // in MeV/c
   Double_t fBeta;
   Double_t fGamma;
   Double_t fBrho;               // in T.m.
   Double_t fTimeOfFlight;       // in ns/m

 protected:
   vector<Double_t>  QuadraticSolver(Double_t a, Double_t b, Double_t c) const;

 protected:
   void        EnergyToBrho();
   void        EnergyToMomentum();
   void        EnergyToTof();
   void        BrhoToEnergy();
   void        BrhoToTof()    {BrhoToEnergy(); EnergyToTof();}
   void        TofToEnergy();
   void        TofToBrho()    {TofToEnergy(); EnergyToBrho();}

 public:
   TNoyau();
   TNoyau(Int_t Z, Int_t A);
   TNoyau(const char* name);
   virtual ~TNoyau();
   
   const char  *GetNom() const { return fNom.Data(); }
   Int_t        GetA() const { return fMasse; }
   Int_t        GetZ() const { return fCharge; }
   Double_t 	GetExcesMasse() const { return fExcesMasse; }
   Double_t 	GetExcitationEnergy() const { return fExcitationEnergy; }
   const char  *GetDemiVie() const { return fDemiVie.Data(); }
   Double_t     GetSpin() const { return fSpin; }
   const char  *GetParite() const { return fParite.Data(); }
   Double_t    GetEnergy()       const {return fEnergy;}
   Double_t    GetMomentum()       const {return fMomentum;}
   Double_t    GetBrho()         const {return fBrho;}
   Double_t    GetTimeOfFlight() const {return fTimeOfFlight;}
   void         SetNom(const char *name) { fNom = name; }
   void         SetA(Int_t A) { fMasse = A; }
   void         SetZ(Int_t Z) { fCharge = Z; }
   void         SetExcesMasse(Int_t exc) { fExcesMasse = exc; }
   void         SetExcitationEnergy(Int_t exc) { fExcitationEnergy = exc; }
   void         SetDemiVie(char *vie) { fDemiVie = vie; }
   void         SetSpin(Double_t spin) { fSpin = spin; }
   void         SetParite(char *parite) { fParite = parite; }
   void        SetEnergy(Double_t energy)    {fEnergy       = energy; EnergyToBrho(); EnergyToMomentum(); //EnergyToTof();
   }
   void        SetBrho(Double_t brho)        {fBrho         = brho;  BrhoToEnergy(); EnergyToMomentum();}
   void        SetTimeOfFlight(Double_t tof) {fTimeOfFlight = tof;}
   
   bool		IsKnown(Int_t charge, Int_t mass) const;
   Int_t	FirstKnown(Int_t charge) const;
   Int_t	LastKnown(Int_t charge) const;
   bool		IsStable() const;
   void		Extract(char *line);
   Double_t	Mass() const;

   void		Print() const;
   void     Print(Option_t*) const {};
   
   ClassDef(TNoyau,1)    // Une Classe TNoyau
};

#endif
