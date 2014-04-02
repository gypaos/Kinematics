////////////////////////////////////////////////////////////////
// This function reads a ENSDF file corresponding to a given  //
// isotope and returns a vector containing the excited states //
// energy.                                                    //
// Important: example of ENSDF file (AR_27Al.ens)             //
//                                                            //
// N. de Sereville (June 2011)                                //
////////////////////////////////////////////////////////////////
#include "TEnsdfPlot.h"

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

ClassImp(TEnsdfPlot)


TEnsdfPlot::TEnsdfPlot()
   : fEnergyMin(-0.2),  // g.s.
     fEnergyMax(8),  // arbitrary value
     fBoxX1(0.15),
     fBoxY1(0.08),
     fBoxX2(0.90),
     fBoxY2(0.88)
{
}




TEnsdfPlot::~TEnsdfPlot()
{
}




void TEnsdfPlot::AddEnsdfFile(TEnsdf* ensdf, Double_t offset)
{
   fEnsdfList.push_back(ensdf);
   fOffsetLevels.push_back(offset);
}




void TEnsdfPlot::AddEnsdfFile(const char* isotope, Double_t offset)
{
   // add ensdf file
   TEnsdf *ensdf = new TEnsdf(isotope);
   fEnsdfList.push_back(ensdf);
   fOffsetLevels.push_back(offset);
}




void TEnsdfPlot::Display()
{
   // search for existing canvas
   TCanvas *c = (TCanvas*) gROOT->GetListOfCanvases()->FindObject("EnsdfPlot");

   if (c) {
      c->Clear();
   }
   else {
      c = new TCanvas("EnsdfPlot", "Ensdf Plotter", 1200, (GetNumberOfEnsdfFiles())*70 + 50);
   }
   c->Draw();

   // box hosting levels
   TBox *box = new TBox(fBoxX1, fBoxY1, fBoxX2, fBoxY2);
   box->Draw("l");

   // display reactions and levels
   for (UInt_t i = 0; i < GetNumberOfEnsdfFiles(); ++i) {   // loop on ensdf files
      DisplayEnsdfFile(i);
   } // end loop on ensdf files

   // update canvas
   c->Update();
}




void TEnsdfPlot::DisplayEnsdfFile(UInt_t index)
{
   // horizontal line and reaction name
   Double_t height   = (fBoxY2-fBoxY1) / GetNumberOfEnsdfFiles();
   Double_t position = fBoxY2 - height*index;
   TLine *line = new TLine(fBoxX1, position, fBoxX2, position);
   line->Draw();
   TLatex *latex = new TLatex(0.03, position - 0.5*height, fEnsdfList[index]->GetFileName());
   latex->SetTextSizePixels(15);
   latex->Draw();

   // draw levels
   Double_t previousPosEnergy = 1;
   Double_t lastPosEnergy     = 1;
   vector<Double_t> ExcitationEnergy = fEnsdfList[index]->GetLevelEnergyList();
   for (UInt_t i = 0; i < ExcitationEnergy.size(); ++i) {   // loop on excited states
      // get energy
      Double_t energy = ExcitationEnergy[i];
      Double_t posEnergy = (fBoxX2 - fBoxX1)/(fEnergyMax - fEnergyMin)*(energy - fEnergyMin) + fBoxX1;
      // if level in focal plane, then draw
      if (energy > fEnergyMin  &&  energy < fEnergyMax) {
         // draw level
         TLine *lineLevel = new TLine(posEnergy, position, posEnergy, position - 0.5*height);
         lineLevel->Draw();
         // label level displayed in such a way that there is no overlap between labels
         if ((fabs(posEnergy-previousPosEnergy) > 0.05 * (fBoxX2-fBoxX1)) ||  // percent of posEnergy scale 
             (fabs(posEnergy-lastPosEnergy)     > 0.05 * (fBoxX2-fBoxX1))) {
            TString labelLevel = Form("%2.3f", ExcitationEnergy[i]);
            TLatex *texLevel = new TLatex(posEnergy, position - 0.95*height, labelLevel.Data());
            texLevel->SetTextSizePixels(12);
            texLevel->Draw();
            lastPosEnergy = posEnergy;
         }
      }
      previousPosEnergy = posEnergy;
   } // end loop on excited states

}
