void drawhist(TString var="DeltaCases", TString country="China") {
   gStyle->SetTimeOffset(0);
   TFile *f = TFile::Open("data_covid19.root");
   auto tr = (TTree*) f->Get("tree");

   TDatime dstart(2020,1,22,0,0,0);
   TDatime dend(2020,3,15,0,0,0);
   auto h = new TH1F("h","",(dend.Convert()-dstart.Convert())/(3600*24)+1,dstart.Convert(),dend.Convert());
   h->GetXaxis()->SetTimeDisplay(1);
   h->GetXaxis()->SetTimeFormat("%m/%d");
   h->GetXaxis()->SetTitle("Date [month/day]");
   
   tr->Draw("date.Convert()>>h","Sum$(" + var + "*(Country==\"" + country + "\"))","hist");
   // tr->Draw("date.Convert()>>h","Sum$(" + var + ")","hist");

   // auto htemp = (TH1F*) gDirectory->Get("htemp");
   // htemp->GetXaxis()->SetTimeDisplay(1);
   // htemp->GetXaxis()->SetTimeFormat("%m/%d");
   // htemp->GetXaxis()->SetTitle("Date [month/day]");
   // htemp->Draw("hist");
}
