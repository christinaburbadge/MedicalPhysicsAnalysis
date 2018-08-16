#define CaenWaveSelector_cxx
// The class definition in CaenWaveSelector.h has been generated automatically
#include "CaenWaveSelector.h"

void CaenWaveSelector::CreateHistograms()
{
   fH2["descantCharge"] = new TH2D("descantCharge", "#gamma Charge vs. Channel", 128, 0., 128., 12000, 0., 12000.);
   fH2["descantEnergy"] = new TH2D("descantEnergy", "#gamma Energy vs. Channel", 128, 0., 128., 16000, 0., 8000.);
   
   for(int detnum=0; detnum<8; ++detnum){
     fH2[Form("cfd_waveform_%d", detnum)] = new TH2D(Form("cfd_waveform_%d", detnum), "descant cfd waveform charge vs. Time", 100, 0, 200, 1024, 0, 16384); 
     fH2[Form("analog_waveform_%d", detnum)] = new TH2D(Form("analog_waveform_%d", detnum), "descant analog waveform charge vs. Time", 100, 0, 200, 1024, 0, 16384); 
     fH2[Form("digital_waveform_%d", detnum)] = new TH2D(Form("digital_waveform_%d", detnum), "descant digital waveform charge vs. Time", 100, 0, 200, 4, 0,4); 
   }
   for(auto it : fH1) {
      GetOutputList()->Add(it.second);
   }
   for(auto it : fH2) {
      GetOutputList()->Add(it.second);
   }
   for(auto it : fHSparse) {
      GetOutputList()->Add(it.second);
   }
}

void CaenWaveSelector::FillHistograms()
{
  if(fFragment->GetDetectorType()!=6 && fFragment->GetDetectorType()!=9){ return; }  //Detector type should be 6 (DESCANT) or 9 (ZDS)
  if(fFragment->GetDetector()>=8 || fFragment->GetDetector()<0){ return; } //GetDetector should return channel 0-7

   fH2.at("descantCharge")->Fill(fFragment->GetChannelNumber(), fFragment->GetCharge());
   fH2.at("descantEnergy")->Fill(fFragment->GetChannelNumber(), fFragment->GetEnergy());

   for(auto i=0; i<(fFragment->GetWaveform()->size()); ++i){ 
     if(i%2==0){//even i's belong to cfd waveforms
       fH2.at(Form("cfd_waveform_%d", fFragment->GetDetector()))->Fill(i, (fFragment->GetWaveform()->at(i))&0x3fff);       //mask digital waveform bits 15 & 16
     }
     else{ //odd i's are the analog waveforms
       fH2.at(Form("analog_waveform_%d", fFragment->GetDetector()))->Fill(i, (fFragment->GetWaveform()->at(i))&0x3fff);        // mask digital waveform bits 15 & 16
     }
     fH2.at(Form("digital_waveform_%d", fFragment->GetDetector()))->Fill(i, (fFragment->GetWaveform()->at(i)>>14)&0x3);        //Write digital waveforms (bits 15&16 scaled by 2^14 bits)
   } 
}
