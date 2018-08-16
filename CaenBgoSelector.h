//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Tue Oct 25 13:18:27 2016 by ROOT version 5.34/24
// from TTree FragmentTree/FragmentTree
// found on file: fragment07844_000.root
//////////////////////////////////////////////////////////

#ifndef CaenBgoSelector_h
#define CaenBgoSelector_h

#include "TChain.h"
#include "TFile.h"

#include "TH1.h"
#include "TH2.h"
#include "TH3.h"
#include "THnSparse.h"

// Header file for the classes stored in the TTree if any.
#include "TGriffin.h"
#include "TZeroDegree.h"
#include "TDescant.h"
#include "TGRSISelector.h"
#include "TLaBr.h"
#include "TLaBrBgo.h"
#include "TLaBrBgoHit.h"

// Fixed size dimensions of array or collections stored in the TTree if any.

class CaenBgoSelector : public TGRSISelector { //Must be same name as .C and .h

 public :
   TGriffin* fGrif; //Pointers to spot that events will be
   TZeroDegree* fZds;
   TDescant* fDesc;
   TLaBr* fLaBr;
   TLaBrBgo* fLaBrBgo;

 CaenBgoSelector(TTree * /*tree*/ =0) : TGRSISelector(), fGrif(NULL), fZds(NULL), fDesc(NULL), fLaBr(NULL), fLaBrBgo(NULL){
      SetOutputPrefix("CaenBgo"); //Changes prefix of output file
   }
	//These functions are expected to exist
   virtual ~CaenBgoSelector() { }
   virtual Int_t   Version() const { return 2; }
   void CreateHistograms();
   void FillHistograms();
   void InitializeBranches(TTree *tree);

   ClassDef(CaenBgoSelector,2); //Makes ROOT happier
};

#endif

#ifdef CaenBgoSelector_cxx
void CaenBgoSelector::InitializeBranches(TTree* tree)
{
   if(!tree) return;
   if(tree->SetBranchAddress("TGriffin", &fGrif) == TTree::kMissingBranch) {
		fGrif = new TGriffin;
	}
   if(tree->SetBranchAddress("TZeroDegree", &fZds) == TTree::kMissingBranch) {
		fZds = new TZeroDegree;
	}
   if(tree->SetBranchAddress("TDescant", &fDesc) == TTree::kMissingBranch) {
		fDesc = new TDescant;
	}
   if(tree->SetBranchAddress("TLaBr", &fLaBr) == TTree::kMissingBranch) {
		fLaBr = new TLaBr;
	}
   if(tree->SetBranchAddress("TLaBrBgo", &fLaBrBgo) == TTree::kMissingBranch) {
		fLaBrBgo = new TLaBrBgo;
	}
}

#endif // #ifdef CaenBgoSelector_cxx
