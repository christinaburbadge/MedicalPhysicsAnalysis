#include "TMath.h" 
#include "TH1.h"
#include "TMath.h"
#include "TF1.h"
#include "TLegend.h"

void EnergyCalibration() {  
  gStyle->SetMarkerStyle(20);   //  g->SetMarkerStyle(20);
  gStyle->SetMarkerSize(1);

  Double_t det[]={2013.8, 4055.6, 5694.9, 8431.4, 12799.0, 15818.8, 18034.0, 21389.9, 23175.9};
  Double_t det2[]={2109.5, 4319.4, 6096.2, 9056.5, 13787.6, 17050.2, 19443.3, 23023.6, 25039.2}; 
  Double_t energy[]={122., 245. ,344. ,511. ,778., 963., 1084., 1299., 1408.};
  Double_t par[6];

  TF1 *f = new TF1("f", "[1] * x + [0]");
  f->SetLineWidth(3);

  TCanvas *c1 = new TCanvas("c1");
  c1->cd();

  TLegend *legend=new TLegend(0.68,0.7,0.86,0.88);
  legend->SetTextSize(0.03);

  TGraph *g = new TGraph((sizeof(det) / sizeof(Double_t)), det, energy);
 
  g->SetTitle("Energy Calibration 152Eu");
  f->SetLineColor(kMagenta);
  g->Fit(f);
  f->GetParameters(par);   // writes the fit results into the par array  
  legend->AddEntry(g,"Data","p");
  legend->AddEntry(f,"fit","l");
  legend->Draw();
  g->Draw("AP");
  
  c1->Print("Ecal0.pdf");

  TCanvas *c2 = new TCanvas("c2");
  c2->cd();

  TLegend *legend2=new TLegend(0.68,0.7,0.86,0.88);
  legend2->SetTextSize(0.03);

  TGraph *g2 = new TGraph((sizeof(det2) / sizeof(Double_t)), det2, energy);
  
  g2->SetTitle("Energy Calibration 152Eu det2");
  f->SetLineColor(kGreen);
  g2->Fit(f);
  f->GetParameters(par);   // writes the fit results into the par array  
  legend2->AddEntry(g2,"Data","p");
  legend2->AddEntry(f,"fit","l");
  legend2->Draw();
  g2->Draw("AP");

  c2->Print("Ecal1.pdf");

}
