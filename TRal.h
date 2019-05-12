#ifndef __Ral__
#define __Ral__

#include "TObject.h"
#include "TString.h"
#include "TGraph.h"

class TRal : public TObject {
 private :
   Int_t	      fDim;
   Double_t* 	fEnergie;      //[fDim] Energie en MeV
   Double_t* 	fDedx_Nuc;     //[fDim] Perte d'energie nucleaire en MeV / (mg/cm2)
   Double_t* 	fDedx_Elec;    //[fDim] Perte d'energie electronique en MeV / (mg/cm2)
   Double_t* 	fDedx_Tot;     //[fDim] Perte d'energie totale en MeV / (mg/cm2)
   TString 	   fUnit;         // MeV / (mg/cm2)
   Double_t* 	fRange;        //[fDim] Range en um
   TString 	   fUnitRange;    // um
   
 public :
   TRal();
   TRal(TString name);
   ~TRal();

   const char	*GetUnit() const { return fUnit.Data(); }
   Int_t	GetDim() const { return fDim; }
   TGraph*  GetEnergyLossGraph() const;
   Double_t	EvalDedxNuc(Double_t ener) const;
   Double_t	EvalDedxElec(Double_t ener) const;
   Double_t	EvalDedxTot(Double_t ener) const;
   Double_t EvalRange(Double_t ener) const;
   Double_t	Slow(Double_t ener, Double_t ecib, Bool_t kOpt=1) const;
   Double_t	ThicknessMax(Double_t ener, Double_t thickness_step) const;
   Double_t EnergyMax(Double_t range) const;

   void         Print() const;
   void         Print(Option_t*) const {};
   void         Draw() const;
   void         Draw(Option_t* option = "") {if (option) {};};

   ClassDef(TRal,1)   // Une Classe TRal
};

#endif

