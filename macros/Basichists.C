#include "TFile.h"
#include "TH1.h"
#include "TH3.h"
#include "TCanvas.h"
#include "TString.h"
#include "TLatex.h"
#include "TLegend.h"
#include "TStyle.h"
#include "THnSparse.h"






//________________
void setPadStyle() {
    gPad->SetTopMargin(0.1);
    gPad->SetBottomMargin(0.15);
    gPad->SetRightMargin(0.1);
    gPad->SetLeftMargin(0.15);
}

//________________
void set1DStyle(TH1 *h, Int_t type = 0, Bool_t doRenorm = kFALSE) {
    Int_t markerStyle = 20; // Full circle
    Double_t markerSize = 1.3;
    Int_t lineWidth = 2;
    Int_t color = 2;
    if (type == 0) {
        color = 2;        // red
        markerStyle = 20; // filled circle
    }
    else if (type == 1) {
        color = 4;        // blue
        markerStyle = 24; // open circle
    }
    else if (type == 2) {
        color = 1;        // black
        markerStyle = 20; // filled triangle
    }
    else if (type == 3) {
        color = 2;        // red
        markerStyle = 24; // open circle
    }
    else if (type == 4) {
        color = 4;        // blue
        markerStyle = 20; // filled circle
    }
    else {
        color = 6;        // magenta
        markerStyle = 30; // open star
    }

    h->SetLineWidth( lineWidth );
    h->SetLineColor( color );
    
    h->SetMarkerStyle( markerStyle );
    h->SetMarkerColor( color );
    h->SetMarkerSize( markerSize );

    h->GetYaxis()->SetTitleSize(0.06);
    h->GetYaxis()->SetLabelSize(0.06);
    h->GetXaxis()->SetTitleSize(0.06);
    h->GetXaxis()->SetLabelSize(0.06);
    h->GetXaxis()->SetNdivisions(208);
    h->GetYaxis()->SetNdivisions(208);    
    h->GetYaxis()->SetTitleOffset(1.1);

    if ( doRenorm ) {
        h->Scale( 1./h->Integral() );
    }
}


//________________
void make1DRatio(TH1F *hRat, TH1F *hDen, const Char_t *ratioName = "Ratio to default", Int_t style = 0, Bool_t isBinomial = kFALSE) {

    if ( !hDen ) {
        std::cout << "Denominator does not exist" << std::endl;
    }

    if ( isBinomial ) {
        hRat->Divide( hRat, hDen, 1., 1., "b" );
    }
    else {
        hRat->Divide( hRat, hDen, 1., 1. );
    }
    hRat->GetYaxis()->SetTitle( ratioName );
    hRat->GetYaxis()->SetRangeUser(0.6,1.2 );
    //hRat->GetXaxis()->SetRangeUser(-2.915, 3.);
    set1DStyle(hRat, style);
}

//________________
void plotComparison(TCanvas *c, TH1F *h1, TH1F *h2,TH1F *hRatio,
                    const char *h1Type, const char *h2Type, float xmin, float xmax,const char *xtitle )  {

    TLatex t;
    t.SetTextFont(42);
    t.SetTextSize(0.05);

    set1DStyle(h1, 0);
    set1DStyle(h2, 1);
    
    set1DStyle(hRatio, 0);
    
    

    c->cd(1);
    gPad->SetLogy();
    setPadStyle();
    h1->Draw();
    h2->Draw("same");
    //h1->GetYaxis()->SetRangeUser(0.0, 0.12);
    h1->GetXaxis()->SetRangeUser(xmin, xmax);
    h1->GetYaxis()->SetTitle("NEvents");
    h1->GetXaxis()->SetTitle(xtitle);
    
   

    TLegend *leg = new TLegend(0.65, 0.65, 0.9, 0.85);
    leg->SetTextSize(0.06);
    leg->AddEntry(h1, Form("%s", h1Type), "p");
    leg->AddEntry(h2, Form("%s", h2Type), "p");
    leg->SetBorderSize(0);
    leg->Draw();

    c->cd(2);
    setPadStyle();
    hRatio->GetXaxis()->SetRangeUser(xmin,xmax);
    hRatio->GetXaxis()->SetTitle(xtitle);
    hRatio->Draw();
    TLine *l = new TLine(xmin,1,xmax,1);
    l->SetLineStyle(2);
    l->SetLineColor(kMagenta);
    l->Draw();

}

//________________
void Basichists() {

    // Base style
    gStyle->SetOptStat(0);
    gStyle->SetOptTitle(0);
    gStyle->SetPalette(kBird);


    
    TFile *f = TFile::Open("/Users/vipulpant/cernbox/newana/TestFile_UPCReco_Vipul_og.root","READ");
    TDirectoryFile *g = (TDirectoryFile*) f->Get("defaultCPDC");
    TDirectoryFile *g1 = (TDirectoryFile*) g->Get("QAplots");

    TH1F *hpt_gen_inc = (TH1F*) g1->Get("hpt_gen_inc");
    TH1F *heta_gen_inc = (TH1F*) g1->Get("heta_gen_inc");
    TH1F *hphi_gen_inc = (TH1F*) g1->Get("hphi_gen_inc");

    TH1F *hpt_inc = (TH1F*) g1->Get("hpt_inc");
    TH1F *heta_inc = (TH1F*) g1->Get("heta_inc");
    TH1F *hphi_inc = (TH1F*) g1->Get("hphi_inc");

    set1DStyle(hpt_gen_inc, 0);
    set1DStyle(heta_gen_inc, 0);
    set1DStyle(hphi_gen_inc, 0);
    set1DStyle(hpt_inc, 0);
    set1DStyle(heta_inc, 0);
    set1DStyle(hphi_inc, 0);


    
    TH1F *hptRatio = dynamic_cast<TH1F*>( hpt_inc->Clone("hpt_inc") );
    TH1F *hetaRatio = dynamic_cast<TH1F*>( heta_inc->Clone("heta_inc") );
    TH1F *hphiRatio = dynamic_cast<TH1F*>( hphi_inc->Clone("hphi_inc") );
    
    
    make1DRatio(hptRatio, hpt_gen_inc, "gen/reco", 2);
    make1DRatio(hetaRatio, heta_gen_inc, "gen/reco", 2);
    make1DRatio(hphiRatio, hphi_gen_inc, "gen/reco", 2);
    

    TCanvas *c1 = new TCanvas("c1", "c1", 800, 1200);
    TCanvas *c2 = new TCanvas("c2", "c2", 800, 1200);
    TCanvas *c3 = new TCanvas("c3", "c3", 800, 1200);

    c1->Divide(1, 2,0,0);
    
    plotComparison(c1, hpt_gen_inc,hpt_inc,hptRatio, "Gen","Reco",0.,10.,"Track pT");

    c2->Divide(1, 2,0,0);
    plotComparison(c2, heta_gen_inc,heta_inc,hetaRatio, "Gen","Reco",-3.,3.,"Track #eta");

    c3->Divide(1, 2,0,0);
    plotComparison(c3, hphi_gen_inc,hphi_inc,hphiRatio, "Gen","Reco",-3.14,3.14,"Track #phi");

    c1->SaveAs("/Users/vipulpant/OOanalysis/Plots/pTdist.pdf");
    c2->SaveAs("/Users/vipulpant/OOanalysis/Plots/etadist.pdf");
    c3->SaveAs("/Users/vipulpant/OOanalysis/Plots/phidist.pdf");
    
    

    
    

    
    
   
}

