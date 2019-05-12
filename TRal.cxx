#include "TRal.h"
#include "TCanvas.h"
#include "TGraph.h"
#include "TSpline.h"
#include "TAxis.h"

#include <string>
#include <iostream>
#include <fstream>
#include <cstdlib>
using namespace std;

ClassImp(TRal)


TRal::TRal() {
}


TRal::TRal(TString name) 
{ 
   fEnergie = new Double_t[200];
   fDedx_Nuc = new Double_t[200];
   fDedx_Elec = new Double_t[200];
   fDedx_Tot = new Double_t[200];
   fRange = new Double_t[200];
   fUnit = "MeV / (mg/cm2)";
   fUnitRange = "um";

   // path to file
   TString path = getenv("DATANUC");
   path += "/SRIM/";
   path += name;
 
   // opening file
   ifstream fich;
   fich.open(path);
   if ( !fich )
     cout << "Problem opening file " << name << endl;
 
   // skeep comment lines at beginning of file
   string line;
   getline(fich, line);
   while (line.compare(0,1,"-") != 0) {
      getline(fich, line);
   }

   // reading file
   Double_t fact = 0;
   Double_t fact2 = 0;
   Double_t energie, nuc, elec, range, dum_n;
   TString unit, dum_c, unit_range;
   Int_t nlines = 0;
   while ( fich >> energie >> unit >> elec >> nuc >> range >> unit_range >> dum_n 
	        >> dum_c >> dum_n >> dum_c ) {
//      cout << energie << "\t" << elec << "\t" << nuc << "\n";
      if ( !unit.CompareTo("keV") ) fact = 1.e-3;
      if ( !unit.CompareTo("MeV") ) fact = 1.;
      if ( !unit.CompareTo("GeV") ) fact = 1.e+3;
      fEnergie[nlines] = energie * fact;
      fDedx_Nuc[nlines] = nuc;
      fDedx_Elec[nlines] = elec;
      fDedx_Tot[nlines] = fDedx_Elec[nlines] + fDedx_Nuc[nlines];
      if (!unit_range.CompareTo("A"))  fact2 = 1.e-4;
      if (!unit_range.CompareTo("um")) fact2 = 1.;
      if (!unit_range.CompareTo("mm")) fact2 = 1.e+3;
      if (!unit_range.CompareTo("xm")) fact2 = 1.e+6;
      if (!unit_range.CompareTo("km")) fact2 = 1.e+9;
//      cout << range << "\t" << unit_range << "\t" << fact2 << endl;
      fRange[nlines] = range * fact2;
      nlines++;
   }
 
   fDim = nlines;
   
   // closing file
   fich.close();
}


TRal::~TRal() 
{
   delete [] fEnergie;
   delete [] fDedx_Nuc;
   delete [] fDedx_Elec;
   delete [] fDedx_Tot;
   delete [] fRange;
}


void TRal::Draw() const
{
   TCanvas *c = new TCanvas("c", "c");
   c->Draw();

   TGraph *gr = new TGraph(fDim, fEnergie, fDedx_Tot);
   gr->Draw("A*");
   gr->GetXaxis()->SetTitle("E (MeV)");
   gr->GetYaxis()->SetTitle("dE/dx   (MeV / (mg/cm^{2})");
   gr->Draw("A");
   
   // use a cubic spline to smooth the graph
   TSpline3 *s = new TSpline3("grs",gr);
   s->SetLineColor(kRed);
   s->Draw();
//   s->Draw("same");
}


TGraph* TRal::GetEnergyLossGraph() const
{
   return (new TGraph(fDim, fEnergie, fDedx_Tot));
}



Double_t TRal::EvalDedxNuc(Double_t ener) const
{
   TSpline3 *s = new TSpline3("s", fEnergie, fDedx_Nuc, fDim);   
   Double_t val = s->Eval(ener);
   
   delete s;   
   return val;
}


Double_t TRal::EvalDedxElec(Double_t ener) const
{
   TSpline3 *s = new TSpline3("s", fEnergie, fDedx_Elec, fDim);   
   Double_t val = s->Eval(ener);
   
   delete s;   
   return val;
}


Double_t TRal::EvalDedxTot(Double_t ener) const
{
   TSpline3 *s = new TSpline3("s", fEnergie, fDedx_Tot, fDim);   
   Double_t val = s->Eval(ener);
   
   delete s;   
   return val;
}


Double_t TRal::EvalRange(Double_t ener) const
{
   TSpline3 *s = new TSpline3("s", fEnergie, fRange, fDim);   
   Double_t val = s->Eval(ener);
   
   delete s;   
   return val;
}



void TRal::Print() const
{
   cout << "Pertes d'energie : " << endl;
}


Double_t TRal::Slow(Double_t ener, Double_t ecib, Bool_t kOpt) const
{
// calcule le ralentissement dans une cible
// energie en MeV
// ecib en mg/cm2
   Int_t nb_couche = 100;
   Double_t ecouche = ecib / (Double_t)nb_couche;
	 
   TSpline3 *s = new TSpline3("s", fEnergie, fDedx_Tot, fDim);
   
   Double_t ener_initial = ener;
   
   Double_t ral = 0.;
   for (Int_t i = 0; i < nb_couche; i++) {
      Double_t de = s->Eval(ener) * ecouche;
      ral += de;
      if (kOpt) ener -= de;  // on perd de l'energie
      else ener += de;       // on gagne de l'energie
      // cas ou l'ion ne sort pas de la cible [doit etre teste]
      if (ener < 0) {ral = ener_initial; break;}
   }
   
   delete s;
   return ral;
}



Double_t TRal::ThicknessMax(Double_t ener, Double_t thickness_step) const
{
// calcule l'epaisseur maximum pour arreter une particule
// ener en Mev
// thickness_step en mg/cm2
   Double_t thick = 0;

   TSpline3 *s = new TSpline3("s", fEnergie, fDedx_Tot, fDim);

   while (ener > 0) {
      Double_t de = s->Eval(ener) * thickness_step;
      ener -= de;
      thick += thickness_step;
   }

   delete s;
   return thick;
}



Double_t TRal::EnergyMax(Double_t range) const
{
   TSpline3 sp("sp", fRange, fEnergie, fDim);

   return sp.Eval(range);
}
