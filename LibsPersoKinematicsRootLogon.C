/*****************************************************************************
 * Copyright (C) 2009-2010   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $GNAIL2ROOT/Licence                           *
 * For the list of contributors see $GANIL2ROOT/README/CREDITS               *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: N. de Sereville  contact address: deserevi@ipno.in2p3.fr *
 *                                                                           *
 * Creation Date  : 07/01/11                                                 *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription: This script loads automatically the kinematics include path   *
 *             and shared libraries.                                         *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment: This script should be called in your rootlogon.C file            *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/
// C++ headers
#include <iostream>
#include <vector>
using namespace std;

// ROOT headers
#include "TSystem.h"
#include "TROOT.h"
#include "TList.h"
#include "TSystemDirectory.h"
#include "TString.h"

void LibsPersoKinematicsRootLogon(bool verbosemode = false)
{
   TString currentpath = gSystem->Getenv("PWD");
   TString path = gSystem->Getenv("LIBSPERSO");
   path += "/kinematics";
   
   // Add include path
   if (verbosemode) cout << "LibsPerso: adding include path : " << path << endl;
   gROOT->ProcessLine(Form(".include %s", path.Data()));

   // Add shared libraries
   if (verbosemode) cout << "LibsPerso: loading shared libraries ..." << endl;
   TString libpath = path.Data();
   TSystemDirectory libdir("libdir", libpath);
   TList* listfile = libdir.GetListOfFiles();

   // Since the list is ordered alphabetically and that the 
   // libVDetector.so library should be loaded before the 
   // lib*Physics.so libraries, it is then loaded manually 
   // first.
   // Test if the lib directory is empty or not
   if (listfile->GetEntries() > 2) {
         gSystem->Load(libpath+"/libRal.so");
         gSystem->Load(libpath+"/libKinematics.so");
   }
    
   // Loop on all libraries
   Int_t i = 0;
   while (listfile->At(i)) {
      TString libname = listfile->At(i++)->GetName();
      if (libname.Contains(".so") && !libname.Contains("libKinematics.so")
                                  && !libname.Contains("libRal.so")) {
         TString lib     = libpath + "/" + libname;
         gSystem->Load(lib);
      }
   }
   
   // Since the libdir.GetListOfFiles() commands cds to the
   // libidr directory, one has to return to the initial
   // directory
   gSystem->cd(currentpath);
   
   if (verbosemode) cout << "LibsPerso: Ready" << endl;
}
