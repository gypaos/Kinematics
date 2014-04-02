////////////////////////////////////////////////////////////////
// This function reads a ENSDF file corresponding to a given  //
// isotope and returns a vector containing the excited states //
// energy.                                                    //
// Important: example of ENSDF file (AR_27Al.ens)             //
//                                                            //
// N. de Sereville (June 2011)                                //
////////////////////////////////////////////////////////////////
#include "TSpectroPlot.h"

#include "TROOT.h"
#include "TObject.h"
#include "TCanvas.h"
#include "TBox.h"
#include "TString.h"
#include "TLine.h"
#include "TLatex.h"
#include "TMath.h"

#include <iostream>
#include <cmath>
using namespace std;

ClassImp(TSpectroPlot)


TSpectroPlot::TSpectroPlot()
   : fBeamEnergy(0),
     fLabAngle(0),
     fMagneticField(0),
     fRhoMin(63),    // SP value
     fRhoMax(84.1),  // SP value
     fBoxX1(0.15),
     fBoxY1(0.08),
     fBoxX2(0.90),
     fBoxY2(0.88),
     fSplitPole(1)
{
}




TSpectroPlot::~TSpectroPlot()
{
}




void TSpectroPlot::AddReaction(TReaction* reaction)
{
   fReactionList.push_back(reaction);
}




void TSpectroPlot::AddReaction(const char* beam, const char* target, const char* light, const char* heavy)
{
   // add reaction
   TReaction *reaction = new TReaction(beam, target, light, heavy);
   fReactionList.push_back(reaction);

   // add levels
   TEnsdf *ensdf = new TEnsdf(heavy);
   fEnsdfList.push_back(ensdf);
}




void TSpectroPlot::SetBeamEnergy(Double_t energy)
{
   fBeamEnergy = energy;

   for (UInt_t i = 0; i < fReactionList.size(); ++i) {   // loop on reaction list
      fReactionList[i]->SetBeamEnergy(energy);
   } // end loop on reaction list
}




void TSpectroPlot::SetLaboratoryAngle(Double_t angle)
{
   fLabAngle = angle;

   for (UInt_t i = 0; i < fReactionList.size(); ++i) {   // loop on reaction list
      fReactionList[i]->SetThetaLab(angle * TMath::Pi()/180);
   } // end loop on reaction list
}




void TSpectroPlot::Display()
{
   // search for existing canvas
   TCanvas *c = (TCanvas*) gROOT->GetListOfCanvases()->FindObject("SpectroPlot");

   if (c) {
      c->Clear();
   }
   else {
      c = new TCanvas("SpectroPlot", "Spectrograph Plotter", 1200, (GetNumberOfReactions())*70 + 50);
   }
   c->Draw();

   // kinematics informations
   TString info = Form("E = %3.3f MeV    Angle = %3.1f #circ    B = %1.3f T", fBeamEnergy, fLabAngle, fMagneticField);
   TLatex *infos = new TLatex((fBoxX1+fBoxX2)/2, (fBoxY2+1)/2, info.Data());
   infos->SetTextSizePixels(25);
//   infos->SetTextFont(6);
   infos->SetTextAlign(22);
   infos->Draw();

   // box hosting levels
   TBox *box = new TBox(fBoxX1, fBoxY1, fBoxX2, fBoxY2);
   box->Draw("l");

   // display reactions and levels
   for (UInt_t i = 0; i < GetNumberOfReactions(); ++i) {   // loop on reactions
      DisplayReaction(i);
      DisplayKinematicFactor(i);
   } // end loop on reactions

   // display focal plane position
   Double_t height   = (fBoxY2-fBoxY1) / GetNumberOfReactions();
   for (UShort_t i = 0; i < 3; ++i) {   // number of positions to be displayed
      Double_t xpos = fBoxX1 + (fBoxX2-fBoxX1)/2*i;
      // draw tick
      TLine *line = new TLine(xpos, fBoxY1, xpos, fBoxY1 + 0.2*height);
      line->Draw();
      // print focal plane position
      TString  rpos = Form("%3.1f cm", fRhoMin + (fRhoMax-fRhoMin)/2*i);
      TLatex *tex = new TLatex(xpos, fBoxY1 - 0.2*height, rpos.Data());
      tex->SetTextSizePixels(15);
      tex->SetTextAlign(22);
      tex->Draw();
   } // end number of positions to be displayed

   // update canvas
   c->Update();
}




void TSpectroPlot::DisplayReaction(UInt_t index)
{
   // horizontal line and reaction name
   Double_t height   = (fBoxY2-fBoxY1) / GetNumberOfReactions();
   Double_t position = fBoxY2 - height*index;
   TLine *line = new TLine(fBoxX1, position, fBoxX2, position);
   line->Draw();
   TLatex *latex = new TLatex(0.03, position - 0.5*height, fReactionList[index]->GetReactionName());
   latex->SetTextSizePixels(15);
   latex->Draw();

   // draw levels
   Double_t previousPosRho = 1;
   Double_t lastPosRho     = 1;
   vector<Double_t> ExcitationEnergy = fEnsdfList[index]->GetLevelEnergyList();
   for (UInt_t i = 0; i < ExcitationEnergy.size(); ++i) {   // loop on excited states
      // calculate brho
      fReactionList[index]->SetExcitationHeavy(ExcitationEnergy[i]);
      fReactionList[index]->RelativisticLabKinematics();
      fReactionList[index]->RelativisticBrho();
      Double_t brho = fReactionList[index]->GetBrho3();;
      Double_t rho = brho / fMagneticField;
      rho *= 100; // in cm
      Double_t posRho = (fBoxX2 - fBoxX1)/(fRhoMax - fRhoMin)*(rho - fRhoMin) + fBoxX1;
      // if level in focal plane, then draw
      if (rho > fRhoMin  &&  rho < fRhoMax) {
         // draw level
         TLine *lineLevel = new TLine(posRho, position, posRho, position - 0.5*height);
         lineLevel->Draw();
         // label level displayed in such a way that there is no overlap between labels
         if ((fabs(posRho-previousPosRho) > 0.05 * (fBoxX2-fBoxX1)) ||  // percent of posRho scale 
             (fabs(posRho-lastPosRho)     > 0.05 * (fBoxX2-fBoxX1))) {
            TString labelLevel = Form("%2.3f", ExcitationEnergy[i]);
            TLatex *texLevel = new TLatex(posRho, position - 0.95*height, labelLevel.Data());
            texLevel->SetTextSizePixels(12);
            texLevel->Draw();
            lastPosRho = posRho;
         }
      }
      previousPosRho = posRho;
   } // end loop on excited states
}



void TSpectroPlot::DisplayKinematicFactor(UInt_t index)
{
   // calculate brho (T.m.) in middle of focal plane
//   Double_t bRhoMid = fMagneticField * (fRhoMin+fRhoMax)/2 * 1e-2;
   Double_t bRhoMid = fMagneticField * (fRhoMin + 1500./2500.*(fRhoMax-fRhoMin)) * 1e-2;

   // calculate associated excitation energy
   Double_t exMid = fReactionList[index]->ReconstructExcitationRelatBrho(bRhoMid, TMath::Pi()/180*fLabAngle);
   fReactionList[index]->SetExcitationHeavy(exMid);

   // calculate Split-Pole focal plane position
   Double_t FPpos = 295 + fReactionList[index]->RelativisticKinematicFactor(fMagneticField, fLabAngle);

   // display focal plane position
   Double_t height   = (fBoxY2-fBoxY1) / GetNumberOfReactions();
   Double_t position = fBoxY2 - height*index;
   if (fSplitPole) {
      TString labelFP = Form("%3.1f", FPpos);
      TLatex *texFP = new TLatex(0.94, position - 0.5*height, labelFP.Data());
      texFP->SetTextSizePixels(15);
      texFP->Draw();
   }
   else {
      TString labelFP = Form("%5.3f", exMid);
      TLatex *texFP = new TLatex(0.94, position - 0.5*height, labelFP.Data());
      texFP->SetTextSizePixels(15);
      texFP->Draw();
   }
}
