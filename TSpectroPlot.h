////////////////////////////////////////////////////////////////
// This function reads a ENSDF file corresponding to a given  //
// isotope and returns a vector containing the excited states //
// energy.                                                    //
// Important: example of ENSDF file (AR_27Al.ens)             //
//                                                            //
// N. de Sereville (June 2011)                                //
////////////////////////////////////////////////////////////////
#ifndef __SPECTROPLOT__
#define __SPECTROPLOT__

// ROOT headers_
#include "TObject.h"

// perso headers
#include "TReaction.h"
#include "TEnsdf.h"

// c++ headers
#include <vector>
using namespace std;


class TSpectroPlot : public TObject
{
   private:
      vector<TReaction*>      fReactionList;
      vector<TEnsdf*>         fEnsdfList;
      Int_t                   fRefReaction;
      Double_t                fBeamEnergy;      // in MeV
      Double_t                fLabAngle;        // in degrees
      Double_t                fMagneticField;   // in Tesla
      Double_t                fRhoMin;          // in cm
      Double_t                fRhoMax;          // in cm
      Double_t                fBoxX1;
      Double_t                fBoxY1;
      Double_t                fBoxX2;
      Double_t                fBoxY2;
      Bool_t                  fSplitPole;       //


   public:
      TSpectroPlot();
      virtual ~TSpectroPlot();

      void SetBeamEnergy(Double_t);
      void SetLaboratoryAngle(Double_t);
      void SetRefReaction(Int_t reaction)       {fRefReaction   = reaction;}
      void SetMagneticField(Double_t field)     {fMagneticField = field;}
      void SetRhoMin(Double_t rmin)             {fRhoMin        = rmin;}
      void SetRhoMax(Double_t rmax)             {fRhoMax        = rmax;}
      void SetIsSplitPole(Bool_t flag)          {fSplitPole     = flag;}

      Double_t GetLaboratoryAngle()    const    {return fLabAngle;}
      Double_t GetBeamEnergy()         const    {return fBeamEnergy;}
      Double_t GetMagneticField()      const    {return fMagneticField;}
      UInt_t   GetNumberOfReactions()  const    {return fReactionList.size();}

      void     AddReaction(TReaction* reaction);
      void     AddReaction(const char*, const char*, const char*, const char*);
      void     Display();
      void     DisplayReaction(UInt_t index);
      void     DisplayKinematicFactor(UInt_t index);
      void     SetRefExcitationEnergy(Double_t);

      ClassDef(TSpectroPlot,1);
};


#endif
