//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Tue Oct 25 13:18:27 2016 by ROOT version 5.34/24
// from TTree FragmentTree/FragmentTree
// found on file: fragment07844_000.root
//////////////////////////////////////////////////////////

#ifndef CaenWaveSelector_h
#define CaenWaveSelector_h

#include "TChain.h"
#include "TFile.h"

// Header file for the classes stored in the TTree if any.
#include "TFragment.h"
#include "TGRSISelector.h"

// Fixed size dimensions of array or collections stored in the TTree if any.

class CaenWaveSelector : public TGRSISelector {

public:
   TFragment* fFragment;

   CaenWaveSelector(TTree* /*tree*/ = 0) : TGRSISelector(), fFragment(0) { SetOutputPrefix("CaenWave"); }
   virtual ~CaenWaveSelector() {}
   virtual Int_t Version() const { return 2; }
   void          CreateHistograms();
   void          FillHistograms();
   void InitializeBranches(TTree* tree);

   ClassDef(CaenWaveSelector, 2);
};

#endif

#ifdef CaenWaveSelector_cxx
void CaenWaveSelector::InitializeBranches(TTree* tree)
{
   if(!tree) return;
   tree->SetBranchAddress("TFragment", &fFragment);
}

#endif // #ifdef CaenWaveSelector_cxx
