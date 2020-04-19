void drawhist(TString var="DeltaCases", TString country="China") {
   gStyle->SetTimeOffset(0);
   TFile *f = TFile::Open("data_covid19.root");
   auto tr = (TTree*) f->Get("tree");

   // start date is the first day you want, minus 12 hours
   TDatime dstart(2020,1,21,12,0,0);
   // end date is the last day you want, as 12:00:00
   TDatime dend(2020,4,18,12,0,0); 
   int ndays = TDatime::GetLegalGlobalDayFromDate(dend.GetDate()) - TDatime::GetLegalGlobalDayFromDate(dstart.GetDate());

   auto h = new TH1F("h","",ndays,dstart.Convert(),dend.Convert());
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
