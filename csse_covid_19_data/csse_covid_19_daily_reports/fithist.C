void fithist(TString var="DeltaCases", TString country="China") {
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

   // fitting part
   TString thefunc;
   TF1 *tf = NULL;

   if (var.Contains("Delta")) { // try fitting with a Gaus function
      thefunc = "gaus";
      tf = new TF1("tf",thefunc,dstart.Convert(),dend.Convert());
      tf->SetParameters(6.68e2,1.583e9,4e5);
      tf->SetParLimits(0,0,3e5);
      tf->SetParLimits(1,dstart.Convert()-100,dend.Convert()+400);
      tf->SetParLimits(2,1e4,1e7);
   } else { // try fitting with a logistic function
      thefunc = "[0]/(1+exp(-[1]*(x-[2])))";
      tf = new TF1("tf",thefunc,dstart.Convert(),dend.Convert());
      tf->SetParameters(8000,4.037e-6,1.58e9);
      tf->SetParLimits(0,0,3e5);
      tf->SetParLimits(2,dstart.Convert()-100,dend.Convert()+400);
      tf->SetParLimits(1,0,1e-4);
   }

   auto res = h->Fit(tf,"WW E R S");

   // plot the result of the fit
   tf->Draw("same");
}
