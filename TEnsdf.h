////////////////////////////////////////////////////////////////
// This function reads a ENSDF file corresponding to a given  //
// isotope and returns a vector containing the excited states //
// energy.                                                    //
// Important: example of ENSDF file (AR_27Al.ens)             //
//                                                            //
// N. de Sereville (June 2011)                                //
////////////////////////////////////////////////////////////////
#ifndef __TENSDF__
#define __TENSDF_

// ROOT headers_
#include "TObject.h"
#include "TString.h"
#include "TGraph.h"
#include "TH1.h"

// c++ headers
#include <iostream>
#include <fstream>
using namespace std;


class TEnsdf : public TObject
{
   private:
      TString           fFileName;                 // isotope name
      vector<Double_t>  fLevelEnergy;              // list of level energies
      vector<Double_t>  fLevelEnergyUncertainty;   // list of level energies uncertainty

   public:
      TEnsdf();
      TEnsdf(const char* isotope);
      virtual ~TEnsdf();

      // getters
      const char*       GetFileName()                       {return fFileName;}
      UInt_t            GetNumberOfLevels()                 {return fLevelEnergy.size();}
      vector<Double_t>  GetLevelEnergyList()                {return fLevelEnergy;}
      vector<Double_t>  GetLevelEnergyUncertaintyList()     {return fLevelEnergyUncertainty;}

      // setters
      void              SetFileName(const char* isotope)    {fFileName = isotope; ReadEnsdfFile();}

      // other methods to access raw information
      void              ReadEnsdfFile();
      Double_t          GetLevelEnergy(Int_t i)             {return fLevelEnergy.at(i);}
      Double_t          GetLevelEnergyUncertainty(Int_t i)  {return fLevelEnergyUncertainty.at(i);}

      // other methods to treat raw information
      TGraph*           GetCumulativeLevels();
      TH1F*             GetLevelSpacing(Double_t exmin, Double_t exmax, Double_t exrange = 1000);

      // display method
      void  PrintLevels();

      void  Clear();
      void  Clear(const Option_t*) {};

      ClassDef(TEnsdf,1);
};


#endif
