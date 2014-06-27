#ifndef __Reaction__
#define __Reaction__

#include "TObject.h"
#include "TString.h"

#ifndef __Noyau__
#include "TNoyau.h"
#endif

#ifndef __Beam__
#include "TBeam.h"
#endif

//class TNoyau;
//class TBeam;


class TReaction : public TObject {
 private:
   // Reaction definition members
   TBeam        *fNoy1;             // Noyau 1 (faisceau)
   TNoyau       *fNoy2;             // Noyau 2
   TNoyau       *fNoy3;             // Noyau 3
   TNoyau       *fNoy4;             // Noyau 4
   Double_t 	 fBeamEnergy;        // Energie du faisceau en MeV
   Double_t     fExcitationLight;   // Energie d'excitation en MeV
   Double_t     fExcitationHeavy;   // Energie d'excitation en MeV
   
   // Reaction kinematics members
   Double_t fThetaCM;		// c.m. angle in rad
   Double_t fThetaLab;		// lab angle in rad
   Double_t fQValue;		// Q-value in MeV
   Double_t fMass1;		// Atomic mass in MeV/c2
   Double_t fMass2;		// Atomic mass in MeV/c2
   Double_t fMass3;		// Atomic mass in MeV/c2
   Double_t fMass4;		// Atomic mass in MeV/c2
   Double_t fLabEnergy3;		// in MeV
   Double_t fLabEnergy4;		// in MeV
   Double_t fLabAngle3;		// in rad
   Double_t fLabAngle4;		// in rad
   Double_t fLabBeta3;		// in v/c
   Double_t fLabBeta4;		// in v/c
   Double_t fLabGamma3;
   Double_t fLabGamma4;
   Double_t fBrho3;		// in T.m
   Double_t fBrho4;		// in T.m
   Double_t fTimeOfFlight3;	// in ns/m
   Double_t fTimeOfFlight4;	// in ns/m
   Double_t fJacobian;        // dW_lab / dW_cm

   // check is valid kinematics
   bool     isValid;
   bool     isThetaCM;

 public:
   // constructeurs
   TReaction();
   TReaction(Int_t Z1, Int_t A1, Int_t Z2, Int_t A2, Int_t Z3, Int_t A3, Int_t Z4, Int_t A4);
   TReaction(Int_t Z1, Int_t A1, Int_t Z2, Int_t A2, Int_t Z3, Int_t A3, Int_t Z4, Int_t A4, Double_t efais);
   TReaction(const char* name1, const char* name2, const char* name3, const char* name4);
   TReaction(const char* name1, const char* name2, const char* name3, const char* name4, Double_t efais);
   TReaction(TBeam* noy1, TNoyau* noy2, TNoyau* noy3, TNoyau* noy4);
   ~TReaction();

   // getters et setters
   void     SetThetaCM(Double_t angle)		      {fThetaCM    = angle;}
   void     SetThetaLab(Double_t angle)		   {fThetaLab   = angle;}
   void     SetExcitationLight(Double_t exci)   {fNoy3->SetExcitationEnergy(exci); fExcitationLight = exci;}
   void     SetExcitationHeavy(Double_t exci)   {fNoy4->SetExcitationEnergy(exci); fExcitationHeavy = exci;}
   void     SetBeamEnergy(Double_t ener)		   {fBeamEnergy = ener; fNoy1->SetEnergie(ener);}
   void     SetLabEnergy3(Double_t ener)        {fLabEnergy3 = ener;}
   void     SetLabEnergy4(Double_t ener)        {fLabEnergy4 = ener;}
   Double_t GetThetaCM() const 			         {return fThetaCM;}
   Double_t GetThetaLab() const 			         {return fThetaLab;}
   Double_t GetExcitationLight() const			   {return fExcitationLight;}
   Double_t GetExcitationHeavy() const			   {return fExcitationHeavy;}
   Double_t GetBeamEnergy() const			      {return fBeamEnergy;}
   Double_t GetQValue() const 			         {return fQValue;}
   TBeam*   GetNoy1() const			            {return fNoy1;}
   TNoyau*  GetNoy2() const 			            {return fNoy2;}
   TNoyau*  GetNoy3() const 			            {return fNoy3;}
   TNoyau*  GetNoy4() const 			            {return fNoy4;}
   Double_t GetLabEnergy3() const			      {return fLabEnergy3;}
   Double_t GetLabAngle3() const			         {return fLabAngle3;}
   Double_t GetBrho3() const			            {return fBrho3;}
   Double_t GetTimeOfFlight3() const		      {return fTimeOfFlight3;}
   Double_t GetLabBeta3() const			         {return fLabBeta3;}
   Double_t GetLabGamma3() const			         {return fLabGamma3;}
   Double_t GetLabEnergy4() const			      {return fLabEnergy4;}
   Double_t GetLabAngle4() const			         {return fLabAngle4;}
   Double_t GetBrho4() const			            {return fBrho4;}
   Double_t GetTimeOfFlight4() const		      {return fTimeOfFlight4;}
   Double_t GetLabBeta4() const			         {return fLabBeta4;}
   Double_t GetLabGamma4() const			         {return fLabGamma4;}
   Double_t GetJacobian() const                 {return fJacobian;}
   TString  GetReactionName();
   
  protected:
   vector<Double_t>  QuadraticSolver(Double_t a, Double_t b, Double_t c) const;

  public:
   // divers
//   Double_t       CoulombBarrier(Option_t *option="in") const;

   // Kinematics
   void	Initialize();
   void	NonRelativisticKinematics();
   void	NonRelativisticLabKinematics();  // direct kinematics only
   void	NonRelativisticBrho();
   void	NonRelativisticTimeOfFlight();
   void 	RelativisticKinematics();
   void	RelativisticLabKinematics();     // direct kinematics only
   void	RelativisticBrho();
   void	RelativisticTimeOfFlight();
   Double_t RelativisticKinematicFactor(Double_t Bfield, Double_t LabAngle, Double_t DeltaLabAngle = 0.1);
   void	SameBrho(const char* ion, Double_t brho) const;
   void  Cinematique(Double_t &ThetaLab3, Double_t &EnergieLab3,
			      Double_t &ThetaLab4, Double_t &EnergieLab4);
   void KinRelat(Double_t &ThetaLab3, Double_t &EnergieLab3,
			   Double_t &ThetaLab4, Double_t &EnergieLab4);
   Double_t ReconstructExcitation(Double_t EnergieLab, Double_t ThetaLab) const;
   Double_t ReconstructExcitationRelat(Double_t EnergieLab, Double_t ThetaLab) const;
   Double_t ReconstructExcitationRelatBrho(Double_t BrhoLab, Double_t ThetaLab) const;
   Double_t Rutherford() const;
   Double_t Rutherford(Double_t theta1, Double_t theta2) const;
   Double_t LimitingAngle() const;
   Double_t WaveVector(UInt_t channel = 0) const;  // k in fm^-1
   Double_t QTransfered() const;
   Double_t LMatching() const;

   bool     IsValid() const {return isValid;}
   void		Dump() const;
   void		Print() const;
   void     Print(Option_t*) const {};
   
   ClassDef(TReaction,1)     // Calculs de cinematique
};

#endif
