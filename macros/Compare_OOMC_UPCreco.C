#include <TCanvas.h>
#include <TFile.h>
#include <TH1.h>
#include <TLegend.h>



void RebinLogx(TH1* hist, Int_t nbinsLog, Double_t minPt, Double_t maxPt) {
    
  TH1::SetDefaultSumw2();
  Double_t binEdges[nbinsLog + 1];
  for (int i = 0; i <= nbinsLog; i++) {
    binEdges[i] = TMath::Power(10, TMath::Log10(minPt) + i * (TMath::Log10(maxPt) - TMath::Log10(minPt)) / nbinsLog);
  }

  hist->SetBins(nbinsLog, binEdges);
}

void DivideBinwidth(TH1* h)
{
for (Int_t iBin=1; iBin<=h->GetNbinsX(); iBin++) {
        Double_t val = h->GetBinContent( iBin );
        Double_t valErr = h->GetBinError( iBin );
        Double_t binWidth = h->GetBinWidth( iBin );
        h->SetBinContent( iBin, val / binWidth );
        h->SetBinError( iBin, valErr / binWidth );
    } // for (Int_t iBin=1; iBin<=h->GetNbinsX(); iBin++)                                           
//h->Scale( 1. / h->Integral() );
}


void Compare_OOMC_UPCreco() {
  // List of file names
  TH1::SetDefaultSumw2();



  
  
  std::vector<TString> fileNames = {

    
    "/Users/vipulpant/OOanalysis/OOTrackAnalysis/Histograms_AMPT_UPCreco_5p36.root",
    
    "/Users/vipulpant/OOanalysis/OOTrackAnalysis/Histograms_Hijing_UPCreco_5p36.root",
    
    "/Users/vipulpant/OOanalysis/OOTrackAnalysis/Histograms_Hydjet_UPCreco_5p36.root"      


  };

  // List of histogram names
  std::vector<TString> histNames = {
    "hNtrk", "htrkpt", "htrketa", "htrkphi"
  };
  std::vector<TString> sampleNames= {"AMPT UPCreco","Hijing UPCreco", "Hydjet UPCreco"};
  std::vector<TString> xaxislabels={"Ntrk","Track pT","Track Eta","Track Phi"};

    


  TLatex t;
  // Colors for different histograms
  int colors[] = {kRed, kBlue, kGreen};

  // Loop through histogram names and create a canvas for each type

  int j=-1;
  for (const auto& histName : histNames) {
    j++;
    TCanvas *canvas = new TCanvas(histName, histName, 800, 600);
    TLegend *legend = new TLegend(0.65, 0.4, 0.9, 0.6);
    if (histName=="hNtrk"||histName=="htrkpt") {
      canvas->SetLogy(1);
      canvas->SetLogx(1);
    }
    
    bool firstHist = true;
    TH1 *hist = nullptr;

    for (size_t i = 0; i < fileNames.size(); ++i) {
      TFile *file = TFile::Open(fileNames[i]);
      if (!file || file->IsZombie()) {
	std::cerr << "Failed to open file: " << fileNames[i] << std::endl;
	continue;
      }

      // Extract the histogram
      hist = dynamic_cast<TH1*>(file->Get(histName));
      //if (histName=="htrketa"||histName=="htrkphi"||histName=="hNtrk")
	//hist->Rebin(20);
      if (histName=="hNtrk") {//hist->Rebin(5);
	hist->GetXaxis()->SetRangeUser(8,1000);
      }
      //if (histName=="htrkpt") RebinLogx(hist,20,0.1,10000);
      //if (histName=="hNtrk") RebinLogx(hist,20,10,1000);
      //DivideBinwidth(hist);

      
      if (!hist) {
	std::cerr << "Histogram " << histName << " not found in " << fileNames[i] << std::endl;
	continue;
      }

      hist->SetLineColor(colors[i]);
      hist->SetLineWidth(2);
      hist->SetTitle("");
      hist->SetStats(kFALSE);
      hist->SetMarkerStyle(108);
      hist->SetMarkerSize(1.4);
      hist->SetMarkerColor(colors[i]);
      hist->GetXaxis()->SetTitle(xaxislabels[j]);
      hist->GetXaxis()->CenterTitle();
				 
      // Draw histograms on the same canvas
      if (firstHist) {
	hist->Draw("e");
	firstHist = false;
      }
      else {
	hist->Draw("e SAME");
      }
      legend->AddEntry(hist, sampleNames[i], "l");
      
      
    }

    t.DrawLatexNDC(0.15, 0.93, "#bf{CMS} #it{Simulation}");
    t.DrawLatexNDC(0.6, 0.93, "OO 5.36 TeV");
    legend->Draw();
    legend->SetBorderSize(0);
    canvas->SaveAs(Form("/Users/vipulpant/OOanalysis/Plots/OOMC_UPCreco_%s.pdf",histName.Data()));  
  }
}

