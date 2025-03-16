void Centrality_bins()
{
  TFile *f = TFile::Open("/Users/vipulpant/cernbox/newana/TestFile_UPCReco_Vipul_og.root","READ");
  TDirectoryFile *g = (TDirectoryFile*) f->Get("defaultCPDC");
  TDirectoryFile *g1 = (TDirectoryFile*) g->Get("QAplots");  
  TCanvas *c2=new TCanvas("c2","c2",1200,1200);
  TLatex t;
  c2->SetLogy(1);
  TH1F *hHFSum= (TH1F*) g1->Get("hHFSum");
  hHFSum->GetXaxis()->SetTitle("HFSum");
  hHFSum->GetYaxis()->SetTitle("Fraction of events");
  hHFSum->SetStats(0);
  hHFSum->GetXaxis()->SetRangeUser(0.,300.);
  hHFSum->GetXaxis()->CenterTitle();
  hHFSum->GetYaxis()->CenterTitle();
  hHFSum->SetMarkerColor(kBlue);
  hHFSum->SetLineColor(kBlue);
  hHFSum->SetMarkerSize(6);
  hHFSum->SetLineWidth(2);
  hHFSum->Scale(1./hHFSum->Integral());
  double totalarea=hHFSum->Integral();
  double areaperbin= totalarea/10;
  std::vector<double> centvals;
  
  int binstart=1;
  centvals.push_back(0.);
  double area = 0.;
  for (int i=1;i<=hHFSum->GetNbinsX();i++)
    { 
      area+=hHFSum->GetBinContent(i);
      cout<<"Intermediate area "<<area/totalarea<<endl;
      if (area/totalarea==1) break;
      if (area >= centvals.size()*areaperbin) {
	double xvalue=hHFSum->GetXaxis()->GetBinUpEdge(i);
	cout<<xvalue<<endl;
	centvals.push_back(xvalue);
      }
      
      }
  centvals.push_back(hHFSum->GetBinCenter(hHFSum->GetNbinsX()));
  cout<<"size :"<<centvals.size()<<endl;
  hHFSum->GetXaxis()->SetTitleSize(0.04);
  hHFSum->GetYaxis()->SetTitleSize(0.04);
  hHFSum->Draw("e");
  TLine *l[11];
  for (int j=0; j<=9; j++)
    {
      l[j]=new TLine(centvals.at(j),0,centvals.at(j),hHFSum->GetBinContent(hHFSum->FindBin(centvals.at(j))));
      l[j]->SetLineStyle(2);
      l[j]->SetLineWidth(4);
      cout<<hHFSum->GetBinContent(hHFSum->FindBin(centvals.at(j)))<<endl;
      //l[j]->SetName(Form("line_%d",j));
      l[j]->Draw();
    }
  

  std::copy(centvals.begin(), centvals.end(), std::ostream_iterator<double>(std::cout, " "));
  int nBins = centvals.size() - 1;

    
  TH1D *cent_vals = new TH1D("cent_vals", "cent_vals", nBins, centvals.data());    
  cent_vals->Fill(100);
  cent_vals->Fill(50);
  TFile *fout= new TFile("cent_vals.root","RECREATE");
  cent_vals->Write();
  t.DrawLatexNDC(0.15, 0.93, "#bf{CMS} #it{Simulation}");
      t.DrawLatexNDC(0.6, 0.93, "HYDJET OO 6.8 TeV");
  c2->SaveAs("/Users/vipulpant/OOanalysis/Plots/Centrality_bins.pdf");
}  
