///////////////////////////////////////////////////////////////////
// This class calculates energy loss in a target made of several //
// layers.                                                       //
//                                                               //
// Simple ions and two-body kinematics are supported.            //
//                                                               //
// N. de Sereville (September 2011)                              //
///////////////////////////////////////////////////////////////////
#ifndef __TTARGET__
#define __TTARGET_

// ROOT headers_
#include "TObject.h"
#include "TString.h"

// Perso headers
#include "TRal.h"
#include "TReaction.h"

// c++ headers
#include <iostream>
#include <vector>
using namespace std;


class TTarget : public TObject
{
   private:
      vector<TString>   fLayerName;                // list of layer's name
      vector<TString>   fInteractingLayerName;     // list of interacting layer's name
      vector<Double_t>  fLayerNominalThickness;    // in ug/cm2
      vector<Double_t>  fLayerEffectiveThickness;  // in ug/cm2
      vector<Double_t>  fLayerEnergyLoss;          // energy loss in MeV
      vector<Double_t>  fLayerEnergyEnd;           // energy in MeV at end of layer
      vector<Double_t>  fLayerBrhoEnd;             // brho in T.m. at end of layer
      vector<TRal*>     fEnergyLossIncidentIon;
      vector<TRal*>     fEnergyLossEmittedIon;
      Double_t          fTargetAngle;              // in deg
      UShort_t          fInteractionLayer;         // layer number (starting from 0)
      Double_t          fInteractionPosition;      // between 0 and 1
      TString           fIncidentIon;              // incident particle
      Double_t          fEnergyIon;                // energy of incident particle (MeV)
      TReaction*        fReaction;
      TString           fEmittedIon;               // emitted particle after reaction
      Double_t          fEnergyBeforeReaction;     // in MeV
      Double_t          fEnergyAfterReaction;      // in MeV
      Double_t          fEnergyLossBeforeReaction; // in keV
      Double_t          fEnergyLossAfterReaction;  // in keV


   protected:
      void     BuildEnergyLossIncident();
      void     BuildEnergyLossEmitted();
      void     CalculateEffectiveThickness();

   public:
      TTarget();
      TTarget(TString name, Double_t thickness);
      virtual ~TTarget();

      // getters
      UShort_t   GetNumberOfLayers()                  const {return fLayerName.size();};
      Double_t   GetLayerEnergyEnd(UShort_t index)    const {return fLayerEnergyEnd[index];};
      Double_t   GetLayerEnergyLoss(UShort_t index)   const {return fLayerEnergyLoss[index];};
      Double_t   GetLayerBrhoEnd(UShort_t index)      const {return fLayerBrhoEnd[index];};
      Double_t   GetEnergyBeforeReaction()            const {return fEnergyBeforeReaction;};
      Double_t   GetEnergyAfterReaction()             const {return fEnergyAfterReaction;};
      TReaction* GetReaction()                        const {return fReaction;};

      // setters
      void  SetTargetAngle(Double_t ang)  {fTargetAngle = ang; CalculateEffectiveThickness();};
      void  SetThickness(UShort_t index,   Double_t thickness);
      void  SetInteraction(TString name,   Double_t position);
      void  SetInteraction(UShort_t index, Double_t position);
      void  SetInteractionPosition(Double_t pos)   {fInteractionPosition = pos;}
      void  SetIncidentIon(TString ion, Double_t ener) {
               fIncidentIon = ion; BuildEnergyLossIncident();
               fEnergyIon   = ener;
      }
      void  SetIncidentIon(TString ion)   {fIncidentIon = ion; BuildEnergyLossIncident();};
      void  SetEnergyIon(Double_t ener)   {fEnergyIon = ener;}
      void  SetReaction(TReaction *reac);
      void  SetEmittedIon(TString ion)    {fEmittedIon = ion; BuildEnergyLossEmitted();};

      // other methods
      void     AddLayer(TString name, Double_t thickness);
      Int_t    EnergyLoss();
      Double_t BrhoAtInteractionPosition(Double_t brho) const;
      Double_t BrhoToExcitationEnergy(Double_t brho) const;
      Double_t EnergyToBrho(TString ion, Double_t energy) const;
      void     Dump() const;
      void     Clear();
      void     Clear(const Option_t*) {};
      void     PrintEnergies() const;

      ClassDef(TTarget,1);
};


#endif
