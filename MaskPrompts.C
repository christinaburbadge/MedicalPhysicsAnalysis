#include "TMath.h"
#include "TH1.h"
#include "TH2.h"
#include "THnSparse.h"

void MaskPrompts(TFile* file, int threshold){
	
	TH2D* energyVsTime0 = (TH2D*)file->Get("energyVsTime0");
	TH2D* energyVsTime1 = (TH2D*)file->Get("energyVsTime1");
	
	TH2D* maskedEvT = (TH2D*)energyVsTime1->Clone("maskedEvT");
	//maskedEvT->Add(energyVsTime1,1);
	TH1D* projT = (TH1D*)maskedEvT->ProjectionX("projT");

	int nbinsX = maskedEvT->GetXaxis()->GetNbins();
	int nbinsY = maskedEvT->GetYaxis()->GetNbins();
	int binContent;
	int i;
	int j;

	//think about doing this with suppressed histograms

	for(i = 0; i <= nbinsX; i++){
		binContent = projT->GetBinContent(i);
		if(binContent >= threshold){
			for(j = -1; j <= nbinsY+1; j++){
				maskedEvT->SetBinContent(i,j,0);
			}
		}
	}
}
//need to implement gamma gamma
//include threshold as function argument
