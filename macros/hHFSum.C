void hHFSum()
{
  TFile *f = TFile::Open("/Users/vipulpant/cernbox/newana/TestFile_UPCReco_Vipul_og.root","READ");
  TDirectoryFile *g = (TDirectoryFile*) f->Get("defaultCPDC");
  TDirectoryFile *g1 = (TDirectoryFile*) g->Get("QAplots");  
  TCanvas *c2=new TCanvas("c2","c2",1200,1200);
  c2->SetLogy(1);
  TH1F *hHFSum= (TH1F*) g1->Get("hHFSum");
  hHFSum->GetXaxis()->SetTitle("HFSum");
  hHFSum->SetMarkerColor(kBlue);
  hHFSum->SetLineColor(kBlue);
  hHFSum->SetMarkerSize(4);
  hHFSum->SetLineWidth(2);
  hHFSum->Scale(1./hHFSum->Integral(""));
  
  std::vector<double> centvals;
  
  int binstart=1;
  centvals.push_back(0.);
  double area = 0.;
  for (int i=1;i<=hHFSum->GetNbinsX();i++)
    {
      area=hHFSum->Integral(binstart,i);
      cout<<"Intermediate area "<<area<<endl;
      if (area>=0.9999) {
	binstart=i+1;
	area=0.;
	double xvalue=0.5*(hHFSum->GetBinCenter(i));
	cout<<xvalue<<endl;
	centvals.push_back(xvalue);
      }
      
    }

  centvals.push_back(hHFSum->GetBinCenter(hHFSum->GetNbinsX()));
  hHFSum->Draw("same");
  TLine *l[9];
  for (int j=1; j<=9; j++)
    {
      l[j]=new TLine(centvals.at(j),0,centvals.at(j),hHFSum->GetBinContent(hHFSum->FindBin(centvals.at(j))));
      l[j]->SetLineStyle(3);
      l[j]->Draw("same");
    }
  

  std::copy(centvals.begin(), centvals.end(), std::ostream_iterator<double>(std::cout, " "));
    
  
  c2->SaveAs("/Users/vipulpant/OOanalysis/Plots/HFSum.pdf");
}  
