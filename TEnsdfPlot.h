////////////////////////////////////////////////////////////////
// This function reads a ENSDF file corresponding to a given  //
// isotope and returns a vector containing the excited states //
// energy.                                                    //
// Important: example of ENSDF file (AR_27Al.ens)             //
//                                                            //
// N. de Sereville (June 2011)                                //
////////////////////////////////////////////////////////////////
#ifndef __TENSDFPLOT__
#define __TENSDFPLOT_

// ROOT headers_
#include "TObject.h"

// perso headers
#include "TEnsdf.h"

// c++ headers
#include <vector>
using namespace std;


class TEnsdfPlot : public TObject
{
   private:
      std::vector<TEnsdf*>    fEnsdfList;
      std::vector<Double_t>   fOffsetLevels;    // offset in keV
      Double_t                fEnergyMin;
      Double_t                fEnergyMax;
      Double_t                fBoxX1;
      Double_t                fBoxY1;
      Double_t                fBoxX2;
      Double_t                fBoxY2;


   public:
      TEnsdfPlot();
      virtual ~TEnsdfPlot();

      // setters
      void     SetEnergyMin(Double_t emin)      {fEnergyMin = emin;}
      void     SetEnergyMax(Double_t emax)      {fEnergyMax = emax;}

      // getters
      UInt_t   GetNumberOfEnsdfFiles() const    {return fEnsdfList.size();}


      void     AddEnsdfFile(TEnsdf* ensdf, Double_t offset = 0);
      void     AddEnsdfFile(const char* isotope, Double_t offset = 0);
      void     Display();
      void     DisplayEnsdfFile(UInt_t index);

      ClassDef(TEnsdfPlot,1);
};


#endif
