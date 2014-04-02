////////////////////////////////////////////////////////////////
// This function reads a ENSDF file corresponding to a given  //
// isotope and returns a vector containing the excited states //
// energy.                                                    //
// Important: example of ENSDF file (AR_27Al.ens)             //
//                                                            //
// N. de Sereville (June 2011)                                //
////////////////////////////////////////////////////////////////
#include "TEnsdf.h"
#include "TMath.h"
#include "TPRegexp.h"

#include <sstream>
#include <algorithm>
using namespace std;

//#define DEBUG


ClassImp(TEnsdf)


TEnsdf::TEnsdf()
   : fFileName("")
{
}



TEnsdf::TEnsdf(const char* fileName)
   : fFileName(fileName)
{
   ReadEnsdfFile();
}




TEnsdf::~TEnsdf()
{
}




void TEnsdf::Clear()
{
   fLevelEnergy.clear();
   fLevelEnergyUncertainty.clear();
}




void TEnsdf::ReadEnsdfFile()
{
   // clear vectors. needed in case we use the SetFileName() method
   Clear();

   // open file to read
   TString path = getenv("DATANUC");
   TString fileName = Form("%s/Ensdf/AR_%s.ens", path.Data(), fFileName.Data());
   ifstream inputFile(fileName.Data());
   if (!inputFile) cout << "Problem opening file " << fileName << endl;

   // read file
   TString lineFile;
   TString isComment, isWhitespace, isLevel;
   TString energy, uncertainty;
   TString tempIsotopeName = fFileName;
   // calculate size of file name
   Int_t   size = tempIsotopeName.Length();
   // get position of first '_' in file name
   Int_t   pos  = tempIsotopeName.First("_");
   if (pos != -1) {
      // remove everything after first '_'
      tempIsotopeName.Remove(pos, size-pos);
      // size now corresponds to the length of the isotope name
      size = tempIsotopeName.Length(); 
   }
   // find length of mass number
//   TString tempIsotopeName = fFileName;
   TPRegexp("\\D").Substitute(tempIsotopeName, "", "g"); // suppress all alpha characters
   Int_t   nbDigit = tempIsotopeName.Length();
#ifdef DEBUG
   cout << "size, nbDigit:\t" << size << "\t" << nbDigit << endl;
#endif
   while (!inputFile.eof()) {
      lineFile.ReadLine(inputFile);
      isWhitespace = lineFile(size,1);
      isComment    = lineFile(size+1,1);
      isLevel      = lineFile(size+2,1);
      if (size == 3  &&  nbDigit == 2) {     // case of a few isotopes, e.g. 12C, 10B, ...
         isComment    = lineFile(size+2,1);
         isLevel      = lineFile(size+3,1);
      }
      if (size == 2  &&  nbDigit == 1) {     // case of a few isotopes, e.g. 1H ...
         isComment    = lineFile(size+2,1);
         isLevel      = lineFile(size+3,1);
      }
#ifdef DEBUG
      cout << "isComment, isLevel:\t" << isComment << "\t" << isLevel << endl;
#endif
      if (isWhitespace.IsWhitespace() && isComment.CompareTo("c") && !isLevel.CompareTo("L")) {
         energy      = lineFile(size+4,10); 
         energy      = energy.Strip();      // remove trailing white spaces
         uncertainty = lineFile(size+14,2);
         if (size == 3  &&  nbDigit == 2) {
            energy      = lineFile(size+5,10); 
            energy      = energy.Strip();      // remove trailing white spaces
            uncertainty = lineFile(size+15,2);
         }
         if (size == 2  &&  nbDigit == 1) {     // case of a few isotopes, e.g. 1H ...
            energy      = lineFile(size+6,10); 
            energy      = energy.Strip();      // remove trailing white spaces
            uncertainty = lineFile(size+16,2);
         }
         if (energy.IsFloat()) {
            // energy set to MeV units
            fLevelEnergy.push_back(energy.Atof()*1e-3);
            // transform uncertainty to MeV units
            UInt_t nElementsAfterComma = 0;
            if (energy.First(".") > 0) nElementsAfterComma = energy.Length() - energy.First(".") - 1;
            Double_t uncertain = uncertainty.Atof();
            uncertain /= TMath::Power(10, (Int_t)nElementsAfterComma);
            fLevelEnergyUncertainty.push_back(uncertain*1e-3);
         }
      }
   }

   // close input file
   inputFile.close();

   // sort level energies
   // !! use with caution since if uncertainty are defined they are not sorted accordingly to the 
   // energies
   sort(fLevelEnergy.begin(), fLevelEnergy.end());
}



TGraph* TEnsdf::GetCumulativeLevels()
{
   // Number of levels
   UInt_t nbLevels = GetNumberOfLevels();

   // instantiate TGraph
   TGraph *gr = new TGraph(nbLevels);
   gr->SetNameTitle("grCumulative", "Cumulated histogram for discrete levels");

   for (UInt_t i = 0; i < nbLevels; ++i) {   // loop on levels
      gr->SetPoint(i, GetLevelEnergy(i), i);
   } // end loop on levels

   return gr;
}


TH1F* TEnsdf::GetLevelSpacing(Double_t exmin, Double_t exmax, Double_t exrange)
{
   // instantiate TH1F
   TH1F *hist = new TH1F("hist", "Level spacing", (Int_t)exrange, 0, exrange);
   hist->SetXTitle("Level spacing [keV]");

   for (UInt_t i = 0; i < GetNumberOfLevels()-1; ++i) {   // loop on levels
      Double_t ex1 = GetLevelEnergy(i);
      Double_t ex2 = GetLevelEnergy(i+1);
      if (ex1>=exmin && ex2<exmax) {
         hist->Fill((ex2 - ex1)*1e3);
      }
   } // end loop on levels

   return hist;
}



void TEnsdf::PrintLevels()
{
   for (UInt_t i = 0; i < fLevelEnergy.size(); ++i) {   // loop on levels
      cout << i << "\t" << fLevelEnergy[i] << endl;
   } // end loop on levels
}
