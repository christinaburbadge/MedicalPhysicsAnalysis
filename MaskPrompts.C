#include "TMath.h"
#include "TH1.h"
#include "TH2.h"
#include "THnSparse.h"

void MaskPrompts(TFile* file, int threshold){
	bool sumDetectors = false;
	bool maskGammaGamma = false;	

	//Creating base histograms from file

	TH2D* rawEvT0 = (TH2D*)file->Get("energyVsTime0");
	TH2D* rawEvT1 = (TH2D*)file->Get("energyVsTime1");
	
	TH2D* delayEvT0 = (TH2D*)rawEvT0->Clone("delayEvT0");
	TH2D* delayEvT1 = (TH2D*)rawEvT1->Clone("delayEvT1");

	TH2D* promptEvT0 = (TH2D*)rawEvT0->Clone("promptEvT0");
	TH2D* promptEvT1 = (TH2D*)rawEvT1->Clone("promptEvT1");

	TH1D* projT0 = (TH1D*)rawEvT0->ProjectionX("projT0");
	TH1D* projT1 = (TH1D*)rawEvT1->ProjectionX("projT1");

	const int nbinsX = delayEvT0->GetXaxis()->GetNbins();
	const int nbinsY = delayEvT1->GetYaxis()->GetNbins();
	int binContent0;
	int binContent1;
	int i;
	int j;
	int beamOnBins = 0;
	int beamOffBins = 0;
	int* beamOn 	= new int[nbinsX];
	int* beamOff 	= new int[nbinsX];
	

	//Splitting up Energy Vs Time Plots into separate prompt and delayed plots, flagging beam on/off bins.
	
	std::cout << "Masking..." <<std::endl;

	for(i = 0; i <= nbinsX; i++){
		binContent0 = projT0->GetBinContent(i);
		binContent1 = projT1->GetBinContent(i);
		if(binContent0 >= threshold){
			for(j = -1; j <= nbinsY+1; j++){
				delayEvT0->SetBinContent(i,j,0);
				delayEvT1->SetBinContent(i,j,0);
			}
			beamOffBins=0;
			beamOnBins++;
	}
		else{
			for(j = -1; j <= nbinsY+1; j++){
				promptEvT0->SetBinContent(i,j,0);
				promptEvT1->SetBinContent(i,j,0);
			}
			beamOffBins++;
			beamOnBins=0;
		}
		beamOff[i] = beamOffBins;
		beamOn[i] = beamOnBins;
		if(i%int(nbinsX/100) == 0){
			std::cout << 100*i/nbinsX << "%\r";	
			std::cout.flush();
		}	
	}

	double binWidth = projT0->GetXaxis()->GetBinWidth(50);
	double period = 20;
	int newXbins = period/projT0->GetXaxis()->GetXmax()*nbinsX;

	//Creating Superimposed histograms.

	TH2D* beamOnEvT0  = new TH2D("beamOnEvT0",  "Superimposed EvT for beam on  windows for det 0", newXbins, 0, period, nbinsY, 0, rawEvT0->GetYaxis()->GetXmax());
	TH2D* beamOnEvT1  = new TH2D("beamOnEvT1",  "Superimposed EvT for beam on  windows for det 1", newXbins, 0, period, nbinsY, 0, rawEvT1->GetYaxis()->GetXmax());
	TH2D* beamOffEvT0 = new TH2D("beamOffEvT0", "Superimposed EvT for beam off windows for det 0", newXbins, 0, period, nbinsY, 0, rawEvT0->GetYaxis()->GetXmax());
	TH2D* beamOffEvT1 = new TH2D("beamOffEvT1", "Superimposed EvT for beam off windows for det 1", newXbins, 0, period, nbinsY, 0, rawEvT1->GetYaxis()->GetXmax());

	//Superimposing beam on and beam off windows to increase statistics

	std::cout << "Superimposing..." <<std::endl;

	for(i = 0; i <= nbinsX;i++){
		for(j=0; j<=nbinsY; j++){
			beamOnEvT0->SetBinContent(beamOn[i],j,beamOnEvT0->GetBinContent(beamOn[i],j)+promptEvT0->GetBinContent(i,j));
			beamOnEvT1->SetBinContent(beamOn[i],j,beamOnEvT1->GetBinContent(beamOn[i],j)+promptEvT1->GetBinContent(i,j));
			beamOffEvT0->SetBinContent(beamOff[i],j,beamOffEvT0->GetBinContent(beamOff[i],j)+delayEvT0->GetBinContent(i,j));
			beamOffEvT1->SetBinContent(beamOff[i],j,beamOffEvT1->GetBinContent(beamOff[i],j)+delayEvT1->GetBinContent(i,j));

		}	
		if(i%int(nbinsX/100) == 0){
			std::cout << 100*i/nbinsX << "%\r";	
			std::cout.flush();
		}
	}

	THnSparse* rawGGT = (THnSparse*)file->Get("gammaGammaEvT");
	int GGbinsE1 = rawGGT->GetAxis(0)->GetNbins();
	int GGbinsE2 = rawGGT->GetAxis(1)->GetNbins();
	int GGbinsT = rawGGT->GetAxis(2)->GetNbins();
	int GGbinsLin = rawGGT->GetNbins();
	int GGbinContent;
	if(maskGammaGamma && GGbinsT == nbinsX){
		Int_t* coord[3];
		THnSparse* promptGGT = (THnSparse*)rawGGT->Clone("promptGGT");
		THnSparse* delayGGT = (THnSparse*)rawGGT->Clone("delayGGT");
		for(i = 0; i< GGbinsLin ; i++){
			GGbinContent = rawGGT->GetBinContent(i,coord);
			if(beamOn[coord[2]] == 0){
				promptGGT->SetBinContent(i,0);
			}
			else{
				delayGGT->SetBinContent(i,0);
			}	
		}	
		TH2D* delayGammaGamma = (TH2D*)delayGGT->Projection(0,1);
		TH2D* promptGammaGamma = (TH2D*)promptGGT->Projection(0,1);
	}
	else{std::cout << "Skipping Gamma Gamma masking" << std::endl;

	//If energy calibrations good, can sum data from detectors for higher statistics. Boolean set at top of file.

	if(sumDetectors){
		TH2D* delayEvT = (TH2D*)delayEvT0->Clone("delayEvT");
		delayEvT->Add(delayEvT1,1);
		TH2D* promptEvT = (TH2D*)promptEvT0->Clone("promptEvT");
		promptEvT->Add(promptEvT1,1);
		TH2D* beamOnEvT = (TH2D*)beamOnEvT0->Clone("beamOnEvT");
		beamOnEvT->Add(beamOnEvT1,1);
		TH2D* beamOffEvT = (TH2D*)beamOffEvT0->Clone("beamOffEvT");
		beamOffEvT->Add(beamOffEvT1,1);

	}
}
//need to implement gamma gamma
