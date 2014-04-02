////////////////////////////////////////////////////////////////
// This class is basically an interface to TTarget. It is in  //
// charge of calculating and plotting brhos on existing       //
// histograms.                                                //
//                                                            //
// N. de Sereville (October 2012)                             //
////////////////////////////////////////////////////////////////
#include "TDisplayBrho.h"

// ROOT headers
#include "TObject.h"
#include "TDirectory.h"
#include "TString.h"
#include "TBox.h"
#include "TLatex.h"

// headers perso
#include "TNoyau.h"

#include <iostream>
using namespace std;

//#define DEBUG

ClassImp(TDisplayBrho)


TDisplayBrho::TDisplayBrho()
   : fTarget(0),
     fEnsdf(0),
     fHist(0),
     fCurrentPad((TPad*) gPad),
     fBrhoOffset(0),
     fBrho(0),
     fBrhoUncertainty(2e-4),
     fLine(new TLine()),
     fLineMaxHeight(0.9),
     fLineMinHeight(0),
     fLevelStart(0),
     fLevelPeriodicity(1),
     fLevelHeight(fLineMaxHeight),
     fBox(new TBox()),
     fVerbose(0),
     fDrawUncertainty(0),
     fDrawLevelNumber(1)
{
   SetLineColor(kGreen);
}




TDisplayBrho::~TDisplayBrho()
{
   delete fTarget;
   delete fEnsdf;
   delete fLine;
   delete fBox;
}



Bool_t TDisplayBrho::CheckExistingHistogram()
// if not already found, check for a defined histogram
// where the brhos will be displayed
{
   Bool_t present;

   if (fHist) {      // already existing histogram
      present = 1;
   }
   else {
      present = 0;   // not sure an histogram will be found
      TIter next(gDirectory->GetList());
      TObject *obj;
      while ((obj = next())) {
         cout << obj->GetName() << endl;
         if (obj->InheritsFrom("TH1")) {
            present = 1;
            fHist = (TH1F*) obj;
         }
      }
   }

#ifdef DEBUG
   cout << "CheckingExistingHistogram()" << endl;
   cout << "histogram found:\t"     << present << endl;
   cout << "histogram address:\t"   << fHist << endl;
   cout << endl;
#endif

   return present;
}




void TDisplayBrho::InitializeEnsdf()
// Get Ensdf object from heavy recoil nucleus if fTarget
// is defined.
{
   if (fTarget) {
      TNoyau *heavy = fTarget->GetReaction()->GetNoy4();
      fEnsdf = new TEnsdf(heavy->GetNom());
   }
}



void TDisplayBrho::InitializeLineAttribute()
// Set some default colors for the TLine object representing
// the brhos
{
   if (fTarget) {
      TString target = fTarget->GetReaction()->GetNoy2()->GetNom();
      if (!target.CompareTo("12C")) {
         SetLineColor(kRed);
      }
      else if (!target.CompareTo("16O")) {
         SetLineColor(kBlue);
      }
   }
}



void TDisplayBrho::Calculate()
// Calculate brho if fTarget is defined
// Excitation energy is directly defined in the TReaction object
{
   if (fTarget) {
      // calculate energy losses
      fTarget->EnergyLoss();
      // bet brho at end of target
      UShort_t nlayers = fTarget->GetNumberOfLayers();
      fBrho = fTarget->GetLayerBrhoEnd(nlayers-1);
      // print infos
      if (fVerbose) cout << fBrho << " T.m." << endl;
   }
}



void TDisplayBrho::Calculate(Int_t level, Bool_t flag)
// Calculate brho for a given level number
// if flag = 0 (default), calculate brho for the energy level
// if flag = 1, make the calculation for the uncertainty
{     
   // set excitation energy
   Double_t energy   = fEnsdf->GetLevelEnergy(level);
   if (flag) energy += fEnsdf->GetLevelEnergyUncertainty(level);
   fTarget->GetReaction()->SetExcitationHeavy(energy);

   // print infos
   if (fVerbose) cout << "(" << level << ")\tEx = " << energy << " MeV\t\t";

   // calculate brho
   Calculate();
}



void TDisplayBrho::Calculate(Int_t lmin, Int_t lmax)
// Calculate brhos for a range of level number
{
   // check if lmin if smaller than lmax
   Int_t ltmp;
   if (lmax < lmin) {
      ltmp = lmax; lmax = lmin; lmin = ltmp;
   }

   for (Int_t i = lmin; i < lmax+1; ++i) {   // loop on levels
      Calculate(i);
   } // end loop on levels
}



void TDisplayBrho::Draw(Int_t level)
// Draw a TLine associated to the brho of a given level
{
   if (CheckExistingHistogram()) {
      // calculate brho
      Calculate(level);

      if (fBrho>fHist->GetXaxis()->GetXmin() && fBrho<fHist->GetXaxis()->GetXmax()) {
         // vertical line associated to state
         fLine->DrawLine(fBrho+fBrhoOffset, fLineMinHeight*fCurrentPad->GetUymax(), fBrho+fBrhoOffset, fLineMaxHeight*fCurrentPad->GetUymax());
         // state number
         if (fDrawLevelNumber) {
            // TLatex *latex = new TLatex(fBrho+fBrhoOffset, 2*fHist->GetBinContent(fHist->FindBin(fBrho+fBrhoOffset)), Form("%d", level));
            if (level%fLevelPeriodicity == fLevelStart) {
               TLatex *latex = new TLatex(fBrho+fBrhoOffset, fLevelHeight*fCurrentPad->GetUymax(), Form("%d", level));
               latex->SetTextSize(0.05);
               latex->SetTextAlign(12);
               latex->SetTextAngle(90);
               latex->Draw();
            }
         }
         fCurrentPad->Update();
         // error box associated to level uncertainty
         if (fDrawUncertainty) {
            // record nominal brho value
            Double_t brho_nominal = fBrho;
            // calculate uncertainty
            Calculate(level, (Bool_t)1);
            fBrhoUncertainty = fBrho - brho_nominal;
            // box associated to brho uncertainty
//            Double_t ybox = 1.20*fHist->GetBinContent(fHist->FindBin(fBrho+fBrhoOffset));
            Double_t ybox = 0.2*fLineMaxHeight*fCurrentPad->GetUymax();
            fBox = new TBox(brho_nominal+fBrhoOffset-fBrhoUncertainty, ybox,
                            brho_nominal+fBrhoOffset+fBrhoUncertainty, ybox + 0.002*(fCurrentPad->GetUymax()-fCurrentPad->GetUymin()));
            fBox->SetFillColor(fLine->GetLineColor());
            fBox->Draw();
         }
      }
   }
    
}



void TDisplayBrho::Draw(Int_t lmin, Int_t lmax)
// Draw a TLine associated to the brhos of a given level range
{
   // check if lmin if smaller than lmax
   Int_t ltmp;
   if (lmax < lmin) {
      ltmp = lmax; lmax = lmin; lmin = ltmp;
   }

   for (Int_t i = lmin; i < lmax+1; ++i) {   // loop on levels
      Draw(i);
   } // end loop on levels
}



void TDisplayBrho::Draw(Double_t brhomin, Double_t brhomax)
// Draw a TLine associated to the brhos of a given brho range
{
   UInt_t nlevels = fEnsdf->GetNumberOfLevels();

   // calculate brhos
   for (UInt_t i = 0; i < nlevels; ++i) {   // loop on excitation energies
      Calculate(i);
      if(fBrho > brhomin && fBrho < brhomax) Draw(i);
   }
}



void TDisplayBrho::Draw()
// Draw a TLine associated to brhos in the histogram range
{
   // get min & max of histogram
   Double_t brhomin = 0;
   Double_t brhomax = 0;
   Int_t binmin = 0;
   Int_t binmax = 0;
   if (CheckExistingHistogram()) {
      // bin min & max
      binmin = fHist->GetXaxis()->GetFirst();
      binmax = fHist->GetXaxis()->GetLast();
      // get brhomin & brhomax
      brhomin = fHist->GetBinCenter(binmin);
      brhomax = fHist->GetBinCenter(binmax);
   }

   #ifdef DEBUG
      cout << "Draw()" << endl;
      cout << "binmin:\t"  << binmin  << endl;
      cout << "binmax:\t"  << binmax  << endl;
      cout << "brhomin:\t" << brhomin << endl;
      cout << "brhomax:\t" << brhomax << endl;
      cout << endl;
   #endif

   Draw(brhomin, brhomax);
}

