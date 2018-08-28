#define CaenBgoSelector_cxx
#include "CaenBgoSelector.h"
#include "TMath.h" 
#include "TH1.h"

void CaenBgoSelector::CreateHistograms() {
  //Define Histograms, x Vs y
  fH1["runTime"] = new TH1D("runTime", "run time", 1000,0,0.001);
  fH1["labrTs"] = new TH1D("labrTs","Time stamps",10000,0,1);
  fH2["hitPattern"] = new TH2D("hitPattern","hit Pattern",4,0,4.0,4,0,4.0);

  //Energy and Timing Histos
  fH2["energyVsDetUnsup"] = new TH2D("energyVsDetUnsup","Raw #gamma Singles",3000,0,3000,8,0,8); 
  fH2["energyVsDetSup"] = new TH2D("energyVsDetSup", "compton suppresed labr spectrum", 3000, 0, 3000, 8, 0, 8);
  fH2["TDVsBGOEnergy1Unsup"] = new TH2D("TDVsBGOEnergy1Unsup","Time Diff Vs. Unsuppressed BGO Energy",400, -200.0, 200.0, 650,0,65000); 
  fH2["TDVsBGOEnergy1Sup"] = new TH2D("TDVsBGOEnergy1Sup","Time Diff Vs. BGO Energy",400, -200.0, 200.0, 650,0,65000); 
  fH1["TDVsBGOEnergy1Proj"] = new TH1D("TDVsBGOEnergy1Proj","Time Diff Vs. BGO Energy",300, -3000.0, 3000.0); 
  fH2["labrVsBGOEnergy1"] = new TH2D("labrVsBGOEnergy1","labr energy  Vs. BGO Energy",3000, 0, 3000, 650,0,65000); 
  fH2["TDVsEnergy"] = new TH2D("TDVsEnergy","#gamma Time Diff using GetTime() Vs. Energy",3000,0,6000,1000,-100,100);
  fH2["TDVsDet"] = new TH2D("TDVsDet","#gamma Time Diff using GetTime()",100,-5,5,8,0,8);
  
  /* //Big plots
  fH2["gammaGammaE"] = new TH2D("gammaGammaE","#gamma gamma E matrix",3000,0,6000,3000,0,6000);
  fH2["gammaGammaBkgd"] = new TH2D("gammaGammaBkgd","#gamma gamma E matrix background",3000,0,6000,3000,0,6000);
  Int_t nbins[3] = {1000,1000,60000};
  const Double_t xmin[3] = {0.,0.,0.};
  const Double_t xmax[3] = {4000.,4000.,500.};
  fHSparse["gammaGammaEvT"] = new THnSparseF("gammaGammaEvT","#gamma gamma E matrix vs Time",3, nbins,xmin,xmax);
  */

  for(auto detnum=0; detnum<2; detnum++){
    fH2[Form("energyVsTime%d",detnum)] = new TH2D(Form("energyVsTime%d",detnum),"#gamma energy vs time short term",60000,0,500,1000,0,4000); // bgo times
  }

  TF1 *f1 = new TF1("f1", "[0] + [1]*x", 0, 60000);

  //Send histograms to Output list to be added and written.
  for(auto it : fH1) {GetOutputList()->Add(it.second);}
  for(auto it : fH2) {GetOutputList()->Add(it.second);} 
  for(auto it : fHSparse) {GetOutputList()->Add(it.second);}
}

void CaenBgoSelector::FillHistograms() {
    Double_t firstTs=0.0, lastTs=0.0;
    
  // BGO Selectors (baseline channels)
  for(auto b = 0; b < fLaBrBgo->GetMultiplicity(); ++b){
    auto bgo = fLaBrBgo->GetLaBrBgoHit(b);          
    if(bgo->GetDetector()>=8 || bgo->GetDetector()<0){ return; } //GetDetector should return a channel 0 -7
    fH2.at("energyVsDetUnsup")->Fill(bgo->GetEnergy(),bgo->GetDetector()+2);
  }
  
  //LaBr selectors 
  for(auto i = 0; i < fLaBr->GetMultiplicity(); ++i){
    auto labr = fLaBr->GetLaBrHit(i);          
    if(labr->GetDetector()>=8 || labr->GetDetector()<0){ return; } //GetDetector should return a channel 0 -7
    fH2.at("energyVsDetUnsup")->Fill(labr->GetEnergy(),labr->GetDetector());
    fH2.at(Form("energyVsTime%d", labr->GetDetector()))->Fill(labr->GetTime()/1e9,labr->GetEnergy());
    fH1.at("labrTs")->Fill(labr->GetTimeStamp());	

    for(auto j=i+1; j < fLaBr->GetMultiplicity(); ++j){
      auto labr2 = fLaBr->GetLaBrHit(j);
      if(labr2->GetDetector()>=8 || labr2->GetDetector()<0){ return; } //GetDetector should return a channel 0 -7
      fH2.at("hitPattern")->Fill(labr->GetDetector(), labr2->GetDetector());
      if(labr->GetAddress()!=labr2->GetAddress()){
	fH2.at("TDVsDet")->Fill(labr->GetTime() - labr2->GetTime(), labr->GetDetector());
	fH2.at("TDVsEnergy")->Fill(labr->GetEnergy(), labr->GetTime() - labr2->GetTime());
	if(abs(labr->GetTime() - labr2->GetTime()) > 6. && abs(labr->GetTime()-labr2->GetTime()) < 9. ){
	  Double_t eet[3] = {labr->GetEnergy(), labr2->GetEnergy(), labr->GetTime()/1e9};
	  //fH2.at("gammaGammaE")->Fill(labr->GetEnergy(), labr2->GetEnergy())
	  // fHSparse.at("gammaGammaEvT")->Fill(eet);
	}
	else{
	  //fH2.at("gammaGammaBkgd")->Fill(labr->GetEnergy(), labr2->GetEnergy());
	}
	    
	if(labr2->GetTime() < labr->GetTime()){firstTs = labr2->GetTime();}
	else{firstTs = labr->GetTime();}
	if(labr2->GetTime()>labr->GetTime()){lastTs = labr2->GetTime();}
	else{lastTs = labr->GetTime();}
      }   
    }
    for(auto c = 0; c < fLaBrBgo->GetMultiplicity(); ++c){
      auto bgolabr = fLaBrBgo->GetHit(c);          
      if(bgolabr->GetDetector()>=8 || bgolabr->GetDetector()<0){ return; } //GetDetector should return a channel 0 -7
      fH2.at("hitPattern")->Fill(labr->GetDetector(), bgolabr->GetDetector()+2);
      if(labr->GetDetector()==1 && bgolabr->GetDetector()==1){
	fH2.at("TDVsBGOEnergy1Unsup")->Fill(labr->GetTime() - bgolabr->GetTime(), bgolabr->GetEnergy());
	fH2.at("labrVsBGOEnergy1")->Fill(labr->GetEnergy(), bgolabr->GetEnergy());
	fH1.at("TDVsBGOEnergy1Proj")->Fill(labr->GetTime() - bgolabr->GetTime());	
      }
    }
  }

  //Suppressed LaBr Selector
  for(auto b = 0; b < fLaBr->GetSuppressedMultiplicity(fLaBrBgo); ++b){
    auto labr = fLaBr->GetSuppressedHit(b);          
    fH2.at("energyVsDetSup")->Fill(labr->GetEnergy(), labr->GetDetector());

    for(auto c = 0; c < fLaBrBgo->GetMultiplicity(); ++c){
      auto bgolabr = fLaBrBgo->GetHit(c);          
      if(labr->GetDetector()==1 && bgolabr->GetDetector()==1){
        fH2.at("TDVsBGOEnergy1Sup")->Fill(labr->GetTime() - bgolabr->GetTime(), bgolabr->GetEnergy());
      }
    }
  }
 fH1.at("runTime")->Fill((lastTs - firstTs)/1e9);
  
}
