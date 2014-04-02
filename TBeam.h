#ifndef __Beam__
#define __Beam__

#ifndef __Noyau__
#include "TNoyau.h"
#endif

class TNoyau;

class TBeam : public TNoyau {
 private :
   Double_t 	fEnergie;	// energie du faisceau en MeV
   Double_t 	fPosX;		// decalage selon x en mm
   Double_t 	fPosY;		// decalage selon y en mm
   Double_t 	fResolEnergie;	// resolution du faisceau en keV (FWNM)
   Double_t	fResolSpatial;	// resolution du faisceau en mm  (FWHM)
   
 public :
   TBeam();
   TBeam(const char* name);
   TBeam(const char* name, Double_t efais);
   TBeam(const char* name, Double_t efais, Double_t x, Double_t y, Double_t defais, 
	 Double_t drfais);
   TBeam(Int_t Z, Int_t A);
   TBeam(Int_t Z, Int_t A, Double_t efais);
   TBeam(Int_t Z, Int_t A, Double_t efais, Double_t x, Double_t y, 
	 Double_t defais, Double_t drfais);
   virtual ~TBeam();

   Double_t	GetEnergie() const { return fEnergie; }
   Double_t	GetPosX() const { return fPosX; }
   Double_t	GetPosY() const { return fPosY; }
   Double_t	GetResolEnergie() const { return fResolEnergie; }
   Double_t	GetResolSpatial() const { return fResolSpatial; }
   void		SetEnergie(Double_t efais) { fEnergie = efais; }
   void		SetPos(Double_t x, Double_t y) { fPosX = x; fPosY = y; }
   void		SetPosX(Double_t x) { fPosX = x; }
   void		SetPosY(Double_t y) { fPosY = y; }
   void		SetResolEnergie(Double_t defais) { fResolEnergie = defais; }
   void		SetResolSpatial(Double_t drfais) { fResolSpatial = drfais; }
   
   Double_t	RealEnergie();
   Double_t*	RealPosition();
   void		Print() const;
   void     Print(Option_t*) const {};
   
   ClassDef(TBeam,1)         // Une classe pour un Faisceau
};   

#endif
