////////////////////////////////////////////////////////////////
// This class is basically an interface to TTarget. It is in  //
// charge of calculating and plotting brhos on existing       //
// histograms.                                                //
//                                                            //
// N. de Sereville (October 2012)                             //
////////////////////////////////////////////////////////////////
#ifndef __TDISPLAYBRHO__
#define __TDISPLAYBRHO_

// ROOT headers_
#include "TObject.h"
#include "TH1F.h"
#include "TPad.h"
#include "TLine.h"
#include "TBox.h"

// perso headers
#include "TTarget.h"
#include "TEnsdf.h"


class TDisplayBrho : public TObject
{
   private:
      TTarget *fTarget;
      TEnsdf  *fEnsdf;
      TH1F    *fHist;
      TPad    *fCurrentPad;
      Double_t fBrhoOffset;         // in T.m.
      Double_t fBrho;               // in T.m.
      Double_t fBrhoUncertainty;    // in T.m.
      TLine   *fLine;               // line for display brho
      Double_t fLineMaxHeight;      // in percent from 0 to 1
      Double_t fLineMinHeight;      // in percent from 0 to 1
      Int_t    fLevelStart;         //
      Int_t    fLevelPeriodicity;   // 
      Double_t fLevelHeight;        // in percent from 0 to 1
      TBox    *fBox;                // box for uncertainty
      Bool_t   fVerbose;            //
      Bool_t   fDrawUncertainty;
      Bool_t   fDrawLevelNumber;
      Bool_t   fIsEnergy;

   protected:
      Bool_t   CheckExistingHistogram();
      void     InitializeEnsdf();
      void     InitializeLineAttribute();

   public:
      TDisplayBrho();
      virtual ~TDisplayBrho();

      // setters
      void     SetTarget(TTarget *target)       {fTarget = target; InitializeEnsdf(); InitializeLineAttribute();}
      void     SetHistogram(TH1F* histogram)    {fHist = histogram;}
      void     SetCurrentPad(TPad* pad)         {fCurrentPad = pad; pad->cd(); cout << pad << endl;}
      void     SetBrhoOffset(Double_t offset)   {fBrhoOffset = offset;}
      void     SetLineColor(Color_t color)      {fLine->SetLineColor(color);}
      void     SetLineStyle(Style_t style)      {fLine->SetLineStyle(style);}
      void     SetLineMaxHeight(Double_t height){fLineMaxHeight = height;}
      void     SetLineMinHeight(Double_t height){fLineMinHeight = height;}
      void     SetLevelStart(Int_t start)       {fLevelStart = start;}
      void     SetLevelPeriodicity(Int_t period){fLevelPeriodicity = period;}
      void     SetLevelHeight(Double_t height)  {fLevelHeight = height;}
      void     SetVerbose(Bool_t verbose)       {fVerbose = verbose;}
      void     SetDrawUncertainty(Bool_t error) {fDrawUncertainty = error;}
      void     SetDrawLevelNumber(Bool_t levnb) {fDrawLevelNumber = levnb;}
      void     SetIsEnergy(Bool_t nener)        {fIsEnergy = nener;}

      // getters
      TTarget* GetTarget()       const          {return fTarget;}
      TEnsdf*  GetEnsdf()        const          {return fEnsdf;}
      Double_t GetBrhoOffset()   const          {return fBrhoOffset;}
      Double_t GetBrho()         const          {return fBrho;}


      // other methods
      void     Calculate();
      void     Calculate(Int_t level, Bool_t flag = 0);
      void     Calculate(Int_t lmin, Int_t lmax);
      void     Draw(Int_t level);
      void     Draw(Int_t lmin, Int_t lmax);
      void     Draw(Double_t brhomin, Double_t brhomax);
      void     Draw();

      ClassDef(TDisplayBrho,1);
};


#endif
