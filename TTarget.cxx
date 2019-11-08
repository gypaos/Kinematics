////////////////////////////////////////////////////////////////
// This function reads a ENSDF file corresponding to a given  //
// isotope and returns a vector containing the excited states //
// energy.                                                    //
// Important: example of ENSDF file (AR_27Al.ens)             //
//                                                            //
// N. de Sereville (June 2011)                                //
////////////////////////////////////////////////////////////////
#include "TTarget.h"
#include "TMath.h"

#include "TNoyau.h"

#include <algorithm>
#include <cmath>
using namespace std;

ClassImp(TTarget)


TTarget::TTarget()
   : fTargetAngle(0),
     fInteractionLayer(0),       // current layer
     fInteractionPosition(1),    // end of target
     fIncidentIon(""),
     fEnergyIon(0),
     fReaction(),
     fEmittedIon(""),
     fEnergyBeforeReaction(0),
     fEnergyAfterReaction(0),
     fEnergyLossBeforeReaction(0),
     fEnergyLossAfterReaction(0),
     fLightParticle(1)
{
}



TTarget::TTarget(TString name, Double_t thickness)
   : fTargetAngle(0),
     fInteractionLayer(0),       // current layer
     fInteractionPosition(1),    // end of target
     fIncidentIon(""),
     fEnergyIon(0),
     fReaction(),
     fEmittedIon(""),
     fEnergyBeforeReaction(0),
     fEnergyAfterReaction(0),
     fEnergyLossBeforeReaction(0),
     fEnergyLossAfterReaction(0),
     fLightParticle(1)
{
   AddLayer(name, thickness);
}




TTarget::~TTarget()
{
   if (fReaction) delete fReaction;
}



void TTarget::Clear()
{
   fLayerEnergyLoss.clear();          // energy loss in MeV
   fLayerEnergyEnd.clear();           // energy in MeV at end of layer
   fLayerBrhoEnd.clear();             // brho in T.m. at end of layer
   fInteractingLayerName.clear();     // name of interacting layers
   fEnergyBeforeReaction     = 0;     // in MeV
   fEnergyAfterReaction      = 0;     // in MeV
   fEnergyLossBeforeReaction = 0;     // in keV
   fEnergyLossAfterReaction  = 0;     // in keV

}



void TTarget::AddLayer(TString name, Double_t thickness)
{
   fLayerName.push_back(name);
   fLayerNominalThickness.push_back(thickness);
   fLayerEffectiveThickness.push_back(thickness);
   SetInteraction(name, 1);
}



void TTarget::SetThickness(UShort_t index, Double_t thickness)
{
   // check if a corresponding layer exist
   if (index >= fLayerName.size()) {
      cout << "WARNING: index does not correspond to any existing layer," << endl;
      cout << "WARNING: index set to last defined layer (" << fLayerName[fLayerName.size()-1] << ")" << endl;
      index = fLayerName.size()-1;
   }

   // get iterator
   vector<Double_t>::iterator it;
   it = fLayerNominalThickness.begin() + index;

   // if layer exist, remove corresponding entry in vector
   fLayerNominalThickness.erase(it);
   // and then put at the position new thickness
   fLayerNominalThickness.insert(it, thickness);

   // calculate effective thickness
   CalculateEffectiveThickness();
}



void TTarget::SetInteraction(TString name, Double_t position)
{
   // find index of 'name' element in vector
   fInteractionLayer = find(fLayerName.begin(), fLayerName.end(), name) - fLayerName.begin();
   fInteractionPosition = position;
}



void TTarget::SetInteraction(UShort_t index, Double_t position)
{
   fInteractionLayer    = index;
   if (fInteractionLayer >= fLayerName.size()) {
      cout << "WARNING: index does not correspond to any existing layer," << endl;
      cout << "WARNING: index set to last defined layer (" << fLayerName[fLayerName.size()-1] << ")" << endl;
      fInteractionLayer = fLayerName.size()-1;
   }
   fInteractionPosition = position;
}



void TTarget::BuildEnergyLossIncident()
{
   for (UShort_t i = 0; i < fLayerName.size(); ++i) {   // loop on layers
      // build SRIM eloss file name
      TString name = fIncidentIon + "_" + fLayerName[i] + ".txt";
      // instantiate TRal object
      TRal *eloss = new TRal(name);
      fEnergyLossIncidentIon.push_back(eloss);
   } // end loop on layers
}



void TTarget::BuildEnergyLossEmitted()
{
   for (UShort_t i = 0; i < fLayerName.size(); ++i) {   // loop on layers
      // build SRIM eloss file name
      TString name = fEmittedIon + "_" + fLayerName[i] + ".txt";
      // instantiate TRal object
      TRal *eloss = new TRal(name);
      fEnergyLossEmittedIon.push_back(eloss);
   } // end loop on layers
}



void TTarget::CalculateEffectiveThickness()
{
   for (UShort_t i = 0; i < fLayerName.size(); ++i) {   // loop on layers
      Double_t effThickness = fLayerNominalThickness[i] / TMath::Cos(fTargetAngle * TMath::Pi()/180);
      fLayerEffectiveThickness[i] = effThickness;
   } // end loop on layers
}



void TTarget::SetReaction(TReaction *reac, Bool_t kLight)
{
   fReaction = reac;
   fLightParticle = kLight;

   SetIncidentIon(reac->GetNoy1()->GetNom(), reac->GetBeamEnergy());
   if (fLightParticle) {
      SetEmittedIon(reac->GetNoy3()->GetNom());
   }
   else {
      SetEmittedIon(reac->GetNoy4()->GetNom());
   }
}



Double_t TTarget::EnergyToBrho(TString ion, Double_t energy) const
{
   TNoyau nucleus(ion);
   nucleus.SetEnergy(energy);

   return nucleus.GetBrho();
}



Int_t TTarget::EnergyLoss()
{
   // clear maps
   Clear();

   // get energy for calculations
   Double_t energy = fEnergyIon;
   if (fReaction) energy = fReaction->GetBeamEnergy();
   Double_t eloss  = 0;

   // check whether an ion is defined when case of no reaction
   if (!fReaction && !fIncidentIon.CompareTo("")) {
      cout << "WARNING: you should define an incident ion" << endl;
      cout << "WARNING: aborting calculation" << endl;
      return 0;
   }

   for (UShort_t i = 0; i < fInteractionLayer; ++i) {   // loop on layers before interacting layer
      eloss = fEnergyLossIncidentIon[i]->Slow(energy, fLayerEffectiveThickness[i]*1e-3);
      energy -= eloss;
      fLayerEnergyLoss.push_back(eloss*1e3);
      fLayerEnergyEnd.push_back(energy);
      fLayerBrhoEnd.push_back(EnergyToBrho(fIncidentIon, energy));
      fInteractingLayerName.push_back(fLayerName[i]);
   } // end loop on layers before interacting layers

   // energy loss in interacting layer
   eloss = fEnergyLossIncidentIon[fInteractionLayer]->Slow(energy, fInteractionPosition*fLayerEffectiveThickness[fInteractionLayer]*1e-3);
   energy -= eloss;
   fEnergyLossBeforeReaction = eloss*1e3;
   fEnergyBeforeReaction     = energy;

   if (fReaction) {
      Double_t pathAfter;
      Double_t reactionAngle = fReaction->GetThetaLab(); // end

      // kinematics
      fReaction->SetBeamEnergy(energy);
//      fReaction->RelativisticLabKinematics();
      // for VAMOS at zero degree
      fReaction->RelativisticKinematics();
      energy = fReaction->GetLabEnergy3();
      if (!fLightParticle) energy = fReaction->GetLabEnergy4();
      fEnergyAfterReaction = energy;

      if (reactionAngle < TMath::Pi()/2) { // transmission case (i.e. magnetic spectro)
         // energy loss in interacting layer after reaction
         pathAfter = (1-fInteractionPosition)*fLayerNominalThickness[fInteractionLayer]*1e-3 / TMath::Cos(reactionAngle - fTargetAngle*TMath::Pi()/180);
         eloss   = fEnergyLossEmittedIon[fInteractionLayer]->Slow(energy, pathAfter);
         energy -= eloss;
         fEnergyLossAfterReaction = eloss*1e3;
         fLayerEnergyLoss.push_back(fEnergyLossBeforeReaction + fEnergyLossAfterReaction);
         fLayerEnergyEnd.push_back(energy);
         fLayerBrhoEnd.push_back(EnergyToBrho(fEmittedIon, energy));
         fInteractingLayerName.push_back(fLayerName[fInteractionLayer]);

         for (UShort_t i = fInteractionLayer+1; i < fLayerName.size(); ++i) {   // loop on layers after interacting layer
            pathAfter = fLayerNominalThickness[i]*1e-3 / TMath::Cos(reactionAngle - fTargetAngle*TMath::Pi()/180);
            eloss     = fEnergyLossEmittedIon[i]->Slow(energy, pathAfter);
            energy   -= eloss;
            fLayerEnergyLoss.push_back(eloss*1e3);
            fLayerEnergyEnd.push_back(energy);
            fLayerBrhoEnd.push_back(EnergyToBrho(fEmittedIon, energy));
            fInteractingLayerName.push_back(fLayerName[i]);
         } // end loop on layers after interacting layer
      }
      else { // backward case (i.e. RBS)
         // energy loss in interacting layer after reaction
         pathAfter = fInteractionPosition*fLayerNominalThickness[fInteractionLayer]*1e-3 / TMath::Abs(TMath::Cos(reactionAngle - fTargetAngle*TMath::Pi()/180));
         eloss   = fEnergyLossEmittedIon[fInteractionLayer]->Slow(energy, pathAfter);
         energy -= eloss;
         fEnergyLossAfterReaction = eloss*1e3;
         fLayerEnergyLoss.push_back(fEnergyLossBeforeReaction + fEnergyLossAfterReaction);
         fLayerEnergyEnd.push_back(energy);
         fLayerBrhoEnd.push_back(EnergyToBrho(fEmittedIon, energy));
         fInteractingLayerName.push_back(fLayerName[fInteractionLayer]);

         if (fInteractionLayer > 0) { // case where interaction layer is not first layer
            for (Short_t i = fInteractionLayer-1; i > -1; i--) {   // loop on layers after interacting layer
               pathAfter = fLayerNominalThickness[i]*1e-3 / TMath::Abs(TMath::Cos(reactionAngle - fTargetAngle*TMath::Pi()/180));
               eloss     = fEnergyLossEmittedIon[i]->Slow(energy, pathAfter);
               energy   -= eloss;
               fLayerEnergyLoss.push_back(eloss*1e3);
               fLayerEnergyEnd.push_back(energy);
               fLayerBrhoEnd.push_back(EnergyToBrho(fEmittedIon, energy));
               fInteractingLayerName.push_back(fLayerName[i]);
            } // end loop on layers after interacting layer
         }
      }

      // set beam energy to initial value for next calculation
      fReaction->SetBeamEnergy(fEnergyIon);
   }
   else {   // store energy loss & energy in case of simple target without reaction
      fLayerEnergyLoss.push_back(eloss*1e3);
      fLayerEnergyEnd.push_back(energy);
      fLayerBrhoEnd.push_back(EnergyToBrho(fIncidentIon, energy));
      fInteractingLayerName.push_back(fLayerName[fInteractionLayer]);
   }

   return 1;
}



// only work in transmission mode
Double_t TTarget::BrhoAtInteractionPosition(Double_t brho) const
{
   Double_t brhoAtInteractionPosition = 0;

   if (fReaction) {
      // initialisations
      Double_t pathAfter;
      Double_t eloss = 0;

      // transform brho in energy
      TNoyau ion(fEmittedIon);
      ion.SetBrho(brho);
      Double_t energy = ion.GetEnergy();

      // detector angle
      Double_t reactionAngle = fReaction->GetThetaLab();

      // energy loss in layers after interacting layers
      for (UShort_t i = fLayerName.size()-1; i > fInteractionLayer; --i) {   // loop on layers after interacting layer
         pathAfter = fLayerNominalThickness[i]*1e-3 / TMath::Cos(reactionAngle - fTargetAngle*TMath::Pi()/180);
         eloss     = fEnergyLossEmittedIon[i]->Slow(energy, pathAfter, 1);
         energy   += eloss;
      } // end loop on layers after interacting layer

      // energy loss in interacting layer
      pathAfter = (1-fInteractionPosition)*fLayerNominalThickness[fInteractionLayer]*1e-3 / TMath::Cos(reactionAngle - fTargetAngle*TMath::Pi()/180);
      eloss   = fEnergyLossEmittedIon[fInteractionLayer]->Slow(energy, pathAfter, 1);
      energy += eloss;

      // transform energy in brho
      ion.SetEnergy(energy);
      brhoAtInteractionPosition = ion.GetBrho();
//      brhoAtInteractionPosition = energy;
   }

   return brhoAtInteractionPosition; 
}



Double_t TTarget::BrhoToExcitationEnergy(Double_t brho) const
{
   Double_t brho_interaction = BrhoAtInteractionPosition(brho);
//   cout << brho_interaction << endl;

   // detector angle
   Double_t reactionAngle = fReaction->GetThetaLab();

   return fReaction->ReconstructExcitationRelatBrho(brho_interaction, reactionAngle);
//   return fReaction->ReconstructExcitationRelat(brho_interaction, reactionAngle);
}



void TTarget::Dump() const
{
   cout << endl;
   cout << "-------- Dump() from TTarget ---------" << endl;
   cout << "Target is made of " << fLayerName.size() << " layers" << endl;
   for (UShort_t i = 0; i < fLayerName.size(); ++i) {   // loop on layers
      cout << "\t" << fLayerName[i] 
           << "\t" << fLayerNominalThickness[i] << "\t(" << fLayerEffectiveThickness[i] << ")" 
           << "\t" << "ug/cm2" << endl;
   } // end loop on layers

   cout << endl;
   cout << "Target angle is " << fTargetAngle << " deg" << endl;
   cout << "Interaction is at " << fInteractionPosition*100 << " % of " << fLayerName[fInteractionLayer] << " layer" << endl;

   if (fReaction) {
      cout << endl;
      cout << "Reaction is " << fReaction->GetNoy2()->GetNom() << "(" << fReaction->GetNoy1()->GetNom() 
                             << "," << fReaction->GetNoy3()->GetNom() << ")" << fReaction->GetNoy4()->GetNom() << endl;
      cout << "Detection angle is " << fReaction->GetThetaLab() * 180/TMath::Pi() << " deg" << endl;
      cout << "Ex(" << fReaction->GetNoy4()->GetNom() << ") = " << fReaction->GetExcitationHeavy() << " MeV" << endl;
   }

   PrintEnergies();
}



void TTarget::PrintEnergies() const
{
   cout << endl;
   Double_t energy = fEnergyIon;
   if (fReaction) energy = fReaction->GetBeamEnergy();

   if (!fReaction) cout << "Incident ion: " << fIncidentIon << endl;
   cout << "Incident energy: " << energy << " MeV" << endl;

   for (UShort_t i = 0; i < fInteractingLayerName.size(); ++i) {   // loop on layers
      if (fInteractionLayer == i) {
         cout << "\t" << fInteractingLayerName[i];
         cout << "\t"   << fEnergyLossBeforeReaction << " keV\t\t" << fEnergyBeforeReaction << "\t" << "MeV" 
              << "\t\t" << EnergyToBrho(fIncidentIon, fEnergyBeforeReaction) << " T.m." << endl;
         if (fReaction) {
            cout << "\t\t" << "** reaction **"          << " \t\t"    << fEnergyAfterReaction  << "\t" << "MeV"
               << "\t\t" << EnergyToBrho(fEmittedIon, fEnergyAfterReaction) << " T.m." << endl;
            cout << "\t\t" << fEnergyLossAfterReaction  << " keV\t\t" << fLayerEnergyEnd[i]    << "\t" << "MeV" 
               << "\t\t" << fLayerBrhoEnd[i] << " T.m." << endl; 
         }
      }
      else {
         cout << "\t" << fInteractingLayerName[i] 
              << "\t" << fLayerEnergyLoss[i] << " keV\t\t" << fLayerEnergyEnd[i] << "\t" << "MeV"
              << "\t\t" << fLayerBrhoEnd[i] << " T.m." << endl;
      }
   } // end loop on layers
   cout << "--------------------------------------" << endl;
   cout << endl;
}
