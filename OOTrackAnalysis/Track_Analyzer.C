#include <TRandom2.h>
#include <TRandom3.h>
#include <vector>
#include <TLorentzVector.h>
#include "THnSparse.h"
#include <cstring>
#include <ctime>
#include <iostream>
#include <cmath>
#include <math.h>
#include <fstream>
#include <vector>
#include <map>
#include "TFrame.h"
#include "TBenchmark.h"
#include "TSystem.h"
#include "TGraph.h"
#include "TGraphErrors.h"
#include "TProfile2D.h"
#include "TDatime.h"
#include <stdlib.h>
#include <algorithm>
#include "TApplication.h"
#include "Rtypes.h"
#include "TObject.h"
#include "TDirectoryFile.h"
using namespace std;

void RebinLogx(TH1* hist, Int_t nbinsLog, Double_t minPt, Double_t maxPt) {

  TH1::SetDefaultSumw2();
  Double_t binEdges[nbinsLog + 1];
  for (int i = 0; i <= nbinsLog; i++) {
    binEdges[i] = TMath::Power(10, TMath::Log10(minPt) + i * (TMath::Log10(maxPt) - TMath::Log10(minPt)) / nbinsLog);
    cout<<binEdges[i]<<endl;
  }

  hist->SetBins(nbinsLog, binEdges);
}

void Track_Analyzer(TString input_file, TString outputFileName)
{

  TH1::SetDefaultSumw2();
  TH2::SetDefaultSumw2();
  TH3::SetDefaultSumw2();

  TFile *f = TFile::Open(input_file,"READ");


  TChain *trk_tree = new TChain("anaTrack/trackTree");
  trk_tree->Add(input_file);
  
  TH1D* hEvents = new TH1D("hEvents", "", 10, 0, 10);
  TH1D* hZvtx = new TH1D("hZvtx", "", 80, -20, 20);


  // 1-d and 2-d histograms to store trk info
  TH1D* hNtrk     = new TH1D("hNtrk","hNtrk",1000,0,1000);
  TH1D* htrkpt    = new TH1D("htrkpt","htrkpt",1000,0,200);
  TH1D* htrketa   = new TH1D("htrketa","htrketa",1000,-3.,3.);
  TH1D* htrkphi   = new TH1D("htrkphi","htrkphi",1000,-3.14,3.14);
  double binEdges[]= {0.5,1.5,2.5,3.5,4.5,5.5,6.5,7.5,8.5};
  TH1D* hnVtx     = new TH1D("hnVtx","hnVtx",8,binEdges);

  TH1D* htrkDcaz= new TH1D("htrkDcaz","htrkDcaz",100,0.,3.);
  TH1D* htrkDcaxy=new TH1D("htrkDcaxy","htrkDcaxy",100,0.,3.);
  
  TH1D* hhiHF     = new TH1D("hhiHF","hhiHF",200,0.,700.);
  TH2D* hNtrkHF   = new TH2D("hNtrkHF","hNtrkHF",500,0.,500,100.,0.,700);

  RebinLogx(htrkpt,30,0.1,200.);
  RebinLogx(hNtrk,40,0.1,1000);

  int nevents = trk_tree->GetEntries(); // number of events
  cout << "Total number of events in those files: "<< nevents << endl;
  cout << endl;
  cout << "-------------------------------------------------" << endl;

  // Start loop over events ****************************************************

  for(int i = 0; i < nevents; i++)
    {

      if (i%100==0) cout<<i<<" events passed "<<endl;


      //Event information******************************************************

      trk_tree->SetBranchStatus("*", 0);
      Float_t vz;
      Float_t hiHF;
      Int_t nVtx;

      trk_tree->SetBranchStatus("zVtx", 1);
      trk_tree->SetBranchAddress("zVtx", &vz);

      trk_tree->SetBranchStatus("hiHF",1);
      trk_tree->SetBranchAddress("hiHF",&hiHF);
      
      trk_tree->SetBranchStatus("nVtx",1);
      trk_tree->SetBranchAddress("nVtx",&nVtx);
      
      Int_t nTrk=0;
      //float trkPt[10000];
      Float_t    trkPt[10000];
      Float_t    trkEta[10000];      
      Float_t    trkPhi[10000];      
      Bool_t    highPurity[10000];

      Float_t    trkDz1[10000];
      Float_t    trkDzError1[10000];
      Float_t    trkDxy1[10000];
      Float_t    trkDxyError1[10000];

      Float_t    trkPtError[10000];

      trk_tree->SetBranchStatus("trkPt", 1);
      trk_tree->SetBranchAddress("trkPt", &trkPt);

      trk_tree->SetBranchStatus("trkEta", 1);
      trk_tree->SetBranchAddress("trkEta", &trkEta);

      trk_tree->SetBranchStatus("trkPhi", 1);
      trk_tree->SetBranchAddress("trkPhi", &trkPhi);

      trk_tree->SetBranchStatus("nTrk", 1);
      trk_tree->SetBranchAddress("nTrk", &nTrk);


      trk_tree->SetBranchStatus("highPurity", 1);
      trk_tree->SetBranchAddress("highPurity", &highPurity);

      trk_tree->SetBranchStatus("trkDxy1",1);
      trk_tree->SetBranchAddress("trkDxy1",&trkDxy1);

      trk_tree->SetBranchStatus("trkDz1",1);
      trk_tree->SetBranchAddress("trkDz1",&trkDz1);

      trk_tree->SetBranchStatus("trkDzError1",1);
      trk_tree->SetBranchAddress("trkDzError1",&trkDzError1);

      trk_tree->SetBranchStatus("trkDxyError1",1);
      trk_tree->SetBranchAddress("trkDxyError1",&trkDxyError1);

      trk_tree->SetBranchStatus("trkPtError",1);
      trk_tree->SetBranchAddress("trkPtError",&trkPtError);
      
      trk_tree->GetEntry(i);
      //return;
      //Event Cuts ***************************************************************
      if(vz <= -15. || vz >= 15.) continue; // vertex cut

      if(nTrk <= 0) continue; // if there are no tracks in the event

      //Fill Event histograms ***************************************************
      Float_t ptHatw=1;
      hNtrk->Fill(nTrk,ptHatw);
      hZvtx->Fill(vz,ptHatw);
      hEvents->Fill(1,ptHatw);
      hNtrkHF->Fill(nTrk,hiHF);
      hhiHF->Fill(hiHF);
      hnVtx->Fill(nVtx);

      //Start Analyzing tracks *************************************************
      for (int j = 0; j < nTrk; j++)  // Track loop start
	{
	  //return;
	  Float_t trk_pt  =  trkPt[j];
	  Float_t trk_eta =  trkEta[j];
	  Float_t trk_phi =  trkPhi[j];
	  //Int_t   trk_NHits=  trkNHits->at(j);
	  Bool_t  isHighPurity= highPurity[j];

	  Float_t trk_pt_error=trkPtError[j];

	  Float_t trk_dz=trkDz1[j];
	  Float_t trk_dxy=trkDxy1[j];
	  
	  Float_t trk_dzerror= trkDzError1[j];
	  Float_t trk_dxyerror= trkDxyError1[j];

	  Float_t trk_dcaxy=abs(trk_dxy/trk_dxyerror);
	  Float_t trk_dcaz=abs(trk_dz/trk_dzerror);


	  //    return;
	  // Apply track cuts***********************************
	  if(!isHighPurity) continue;
	  if(abs(trk_eta) > 1.0) continue;
	  if(trk_pt<0.1) continue;
	  
	  if(trk_dcaxy >= 3.0) continue;
	  if(trk_dcaz  >= 3.0) continue;
	  
	  if(trk_pt>10 && abs(trk_pt_error/trk_pt)>=0.1) continue;

	  //cout<<trk_pt<<"  "<<trk_eta<<" "<<endl;

	  htrkpt->Fill(trk_pt);
	  htrketa->Fill(trk_eta);
	  htrkphi->Fill(trk_phi);
	  htrkDcaz->Fill(trk_dcaz);
	  htrkDcaxy->Fill(trk_dcaxy);
	  
	  
	} 

    }//events loop end

  delete trk_tree;


  TFile *fout = new TFile(outputFileName, "RECREATE");

  hEvents->Write();
  hNtrk->Write();
  hZvtx->Write();
  htrkpt->Write();
  htrketa->Write();
  htrkphi->Write();
  hNtrkHF->Write();
  htrkDcaz->Write();
  htrkDcaxy->Write();
  hhiHF->Write();
  hnVtx->Write();
  
  fout->Write();
  fout->Close();
  delete fout;
  cout<<"finished";





} // program end

int main(int argc, char** argv){
  TString firstArgument(argv[1]);
  TString outfile(argv[2]);
  

  Track_Analyzer(firstArgument,outfile);
}
