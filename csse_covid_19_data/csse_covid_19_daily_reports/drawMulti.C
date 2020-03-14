void setHistStyle(TH1F *hist, Color_t color = kBlack) {
   hist->GetXaxis()->SetTimeDisplay(1);
   hist->GetXaxis()->SetTimeFormat("%m/%d");
   hist->GetXaxis()->SetTitle("Date [month/day]");
   hist->GetYaxis()->SetTitle("Counts");
   hist->SetMarkerColor(color);
   hist->SetLineColor(color);
};

void makeDelta(TH1F *horig, TH1F *htarget) {
   for (int i=1; i<=horig->GetNbinsX(); i++) {
      htarget->SetBinContent(i, horig->GetBinContent(i)-horig->GetBinContent(i-1));
   }
}

void drawMulti() {
   gStyle->SetTimeOffset(0);
   TFile *f = TFile::Open("data_covid19.root");
   auto tr = (TTree*) f->Get("tree");

   // start date is the first day you want
   TDatime dstart(2020,1,22,0,0,0);
   // TDatime dstart(2020,3,8,0,0,0);
   // end date is the last day you want, plus one day
   TDatime dend(2020,3,14,0,0,0); 

   TCanvas *c1 = new TCanvas("coronavirus","coronavirus",1280,800);
   c1->Divide(2,2);

   ///////////////////////
   // WORLD TOTAL CASES //
   ///////////////////////

   c1->cd(1);
   auto hCasesWorld = new TH1F("hCasesWorld","Cases (World)",(dend.Convert()-dstart.Convert())/(3600*24),dstart.Convert(),dend.Convert());
   auto hDeathsWorld = new TH1F("hDeathsWorld","Deaths (World)",(dend.Convert()-dstart.Convert())/(3600*24),dstart.Convert(),dend.Convert());
   auto hRecoveredWorld = new TH1F("hRecoveredWorld","Recovered (World)",(dend.Convert()-dstart.Convert())/(3600*24),dstart.Convert(),dend.Convert());
   auto hCasesActiveWorld = new TH1F("hCasesActiveWorld","CasesActive (World)",(dend.Convert()-dstart.Convert())/(3600*24),dstart.Convert(),dend.Convert());

   setHistStyle(hCasesWorld, kBlack);
   setHistStyle(hDeathsWorld, kRed);
   setHistStyle(hRecoveredWorld, kBlue);
   setHistStyle(hCasesActiveWorld, kGreen+2);
   
   tr->Draw("date.Convert()>>hCasesWorld","Sum$(Cases)","hist L");
   tr->Draw("date.Convert()>>hDeathsWorld","Sum$(Deaths)","hist L same");
   tr->Draw("date.Convert()>>hRecoveredWorld","Sum$(Recovered)","hist L same");
   tr->Draw("date.Convert()>>hCasesActiveWorld","Sum$(Cases)","goff");
   hCasesActiveWorld->Add(hRecoveredWorld,-1);
   hCasesActiveWorld->Draw("hist L same");


   TLegend *tlegWorld = new TLegend(0.2,0.6,0.5,0.9);
   tlegWorld->SetBorderSize(0);
   tlegWorld->AddEntry(hCasesWorld,"Total cases","L");
   tlegWorld->AddEntry(hDeathsWorld,"Total deaths","L");
   tlegWorld->AddEntry(hRecoveredWorld,"Total recovered","L");
   tlegWorld->AddEntry(hCasesActiveWorld,"Total active cases","L");
   tlegWorld->Draw();


   ///////////////////////
   // WORLD d/dt  CASES //
   ///////////////////////

   c1->cd(2);
   auto hDeltaCasesWorld = new TH1F("hDeltaCasesWorld","DeltaCases (World)",(dend.Convert()-dstart.Convert())/(3600*24),dstart.Convert(),dend.Convert());
   auto hDeltaDeathsWorld = new TH1F("hDeltaDeathsWorld","DeltaDeaths (World)",(dend.Convert()-dstart.Convert())/(3600*24),dstart.Convert(),dend.Convert());
   auto hDeltaRecoveredWorld = new TH1F("hDeltaRecoveredWorld","DeltaRecovered (World)",(dend.Convert()-dstart.Convert())/(3600*24),dstart.Convert(),dend.Convert());
   auto hDeltaCasesActiveWorld = new TH1F("hDeltaCasesActiveWorld","DeltaCasesActive (World)",(dend.Convert()-dstart.Convert())/(3600*24),dstart.Convert(),dend.Convert());

   setHistStyle(hDeltaCasesWorld, kBlack);
   setHistStyle(hDeltaDeathsWorld, kRed);
   setHistStyle(hDeltaRecoveredWorld, kBlue);
   setHistStyle(hDeltaCasesActiveWorld, kGreen+2);
   
   // tr->Draw("date.Convert()>>hDeltaCasesWorld","Sum$(DeltaCases)","hist L");
   // tr->Draw("date.Convert()>>hDeltaDeathsWorld","Sum$(DeltaDeaths)","hist L same");
   // tr->Draw("date.Convert()>>hDeltaRecoveredWorld","Sum$(DeltaRecovered)","hist L same");
   // tr->Draw("date.Convert()>>hDeltaCasesActiveWorld","Sum$(DeltaCases)","goff");
   // hDeltaCasesActiveWorld->Add(hDeltaRecoveredWorld,-1);
   // hDeltaCasesActiveWorld->Draw("hist L same");
   makeDelta(hCasesWorld,hDeltaCasesWorld); hDeltaCasesWorld->Draw("hist L");
   makeDelta(hDeathsWorld,hDeltaDeathsWorld); hDeltaDeathsWorld->Draw("hist L same");
   makeDelta(hRecoveredWorld,hDeltaRecoveredWorld); hDeltaRecoveredWorld->Draw("hist L same");
   makeDelta(hCasesActiveWorld,hDeltaCasesActiveWorld); hDeltaCasesActiveWorld->Draw("hist L same");
   

   TLegend *tlegWorld2 = new TLegend(0.2,0.6,0.5,0.9);
   tlegWorld2->SetBorderSize(0);
   tlegWorld2->AddEntry(hDeltaCasesWorld,"#Delta cases","L");
   tlegWorld2->AddEntry(hDeltaDeathsWorld,"#Delta deaths","L");
   tlegWorld2->AddEntry(hDeltaRecoveredWorld,"#Delta recovered","L");
   tlegWorld2->AddEntry(hDeltaCasesActiveWorld,"#Delta active cases","L");
   tlegWorld2->Draw();
   

   /////////////////////////////////////
   // focus on a few countries: cases //
   /////////////////////////////////////

   c1->cd(3);
   gPad->SetLogy();
   auto hCasesChina = new TH1F("hCasesChina","Cases (China)",(dend.Convert()-dstart.Convert())/(3600*24),dstart.Convert(),dend.Convert());
   auto hCasesKorea = new TH1F("hCasesKorea","Cases (Korea)",(dend.Convert()-dstart.Convert())/(3600*24),dstart.Convert(),dend.Convert());
   auto hCasesJapan = new TH1F("hCasesJapan","Cases (Japan)",(dend.Convert()-dstart.Convert())/(3600*24),dstart.Convert(),dend.Convert());
   auto hCasesIran = new TH1F("hCasesIran","Cases (Iran)",(dend.Convert()-dstart.Convert())/(3600*24),dstart.Convert(),dend.Convert());
   auto hCasesItaly = new TH1F("hCasesItaly","Cases (Italy)",(dend.Convert()-dstart.Convert())/(3600*24),dstart.Convert(),dend.Convert());
   auto hCasesFrance = new TH1F("hCasesFrance","Cases (France)",(dend.Convert()-dstart.Convert())/(3600*24),dstart.Convert(),dend.Convert());
   auto hCasesGermany = new TH1F("hCasesGermany","Cases (Germany)",(dend.Convert()-dstart.Convert())/(3600*24),dstart.Convert(),dend.Convert());
   auto hCasesSwitzerland = new TH1F("hCasesSwitzerland","Cases (Switzerland)",(dend.Convert()-dstart.Convert())/(3600*24),dstart.Convert(),dend.Convert());
   auto hCasesUK = new TH1F("hCasesUK","Cases (UK)",(dend.Convert()-dstart.Convert())/(3600*24),dstart.Convert(),dend.Convert());
   auto hCasesUS = new TH1F("hCasesUS","Cases (US)",(dend.Convert()-dstart.Convert())/(3600*24),dstart.Convert(),dend.Convert());

   setHistStyle(hCasesChina, kBlack);
   setHistStyle(hCasesKorea, kRed);
   setHistStyle(hCasesJapan, kBlue);
   setHistStyle(hCasesIran, kGreen+2);
   setHistStyle(hCasesItaly, kYellow+2);
   setHistStyle(hCasesFrance, kViolet);
   setHistStyle(hCasesGermany, kOrange);
   setHistStyle(hCasesSwitzerland, kTeal);
   setHistStyle(hCasesUK, kPink);
   setHistStyle(hCasesUS, kAzure);

   tr->Draw("date.Convert()>>hCasesChina","Sum$(Cases*(Country==\"China\"))","hist L");
   tr->Draw("date.Convert()>>hCasesKorea","Sum$(Cases*(Country==\"South Korea\"))","hist L same");
   tr->Draw("date.Convert()>>hCasesJapan","Sum$(Cases*(Country==\"Japan\"))","hist L same");
   tr->Draw("date.Convert()>>hCasesIran","Sum$(Cases*(Country==\"Iran\"))","hist L same");
   tr->Draw("date.Convert()>>hCasesItaly","Sum$(Cases*(Country==\"Italy\"))","hist L same");
   tr->Draw("date.Convert()>>hCasesFrance","Sum$(Cases*(Country==\"France\"))","hist L same");
   tr->Draw("date.Convert()>>hCasesGermany","Sum$(Cases*(Country==\"Germany\"))","hist L same");
   tr->Draw("date.Convert()>>hCasesSwitzerland","Sum$(Cases*(Country==\"Switzerland\"))","hist L same");
   tr->Draw("date.Convert()>>hCasesUK","Sum$(Cases*(Country==\"UK\"))","hist L same");
   tr->Draw("date.Convert()>>hCasesUS","Sum$(Cases*(Country==\"US\"))","hist L same");

   TLegend *tleg = new TLegend(0.349211,0.236871,0.567651,0.771267);
   tleg->SetBorderSize(0);
   tleg->AddEntry(hCasesChina,"China cases","L");
   tleg->AddEntry(hCasesKorea,"Korea cases","L");
   tleg->AddEntry(hCasesJapan,"Japan cases","L");
   tleg->AddEntry(hCasesIran,"Iran cases","L");
   tleg->AddEntry(hCasesItaly,"Italy cases","L");
   tleg->AddEntry(hCasesFrance,"France cases","L");
   tleg->AddEntry(hCasesGermany,"Germany cases","L");
   tleg->AddEntry(hCasesSwitzerland,"Switzerland cases","L");
   tleg->AddEntry(hCasesUK,"UK cases","L");
   tleg->AddEntry(hCasesUS,"US cases","L");
   tleg->Draw();
   

   ///////////////////////////////////////////
   // focus on a few countries: Delta cases //
   ///////////////////////////////////////////

   c1->cd(4);
   // gPad->SetLogy();
   auto hDeltaCasesChina = new TH1F("hDeltaCasesChina","DeltaCases (China)",(dend.Convert()-dstart.Convert())/(3600*24),dstart.Convert(),dend.Convert());
   auto hDeltaCasesKorea = new TH1F("hDeltaCasesKorea","DeltaCases (Korea)",(dend.Convert()-dstart.Convert())/(3600*24),dstart.Convert(),dend.Convert());
   auto hDeltaCasesJapan = new TH1F("hDeltaCasesJapan","DeltaCases (Japan)",(dend.Convert()-dstart.Convert())/(3600*24),dstart.Convert(),dend.Convert());
   auto hDeltaCasesIran = new TH1F("hDeltaCasesIran","DeltaCases (Iran)",(dend.Convert()-dstart.Convert())/(3600*24),dstart.Convert(),dend.Convert());
   auto hDeltaCasesItaly = new TH1F("hDeltaCasesItaly","DeltaCases (Italy)",(dend.Convert()-dstart.Convert())/(3600*24),dstart.Convert(),dend.Convert());
   auto hDeltaCasesFrance = new TH1F("hDeltaCasesFrance","DeltaCases (France)",(dend.Convert()-dstart.Convert())/(3600*24),dstart.Convert(),dend.Convert());
   auto hDeltaCasesGermany = new TH1F("hDeltaCasesGermany","DeltaCases (Germany)",(dend.Convert()-dstart.Convert())/(3600*24),dstart.Convert(),dend.Convert());
   auto hDeltaCasesSwitzerland = new TH1F("hDeltaCasesSwitzerland","DeltaCases (Switzerland)",(dend.Convert()-dstart.Convert())/(3600*24),dstart.Convert(),dend.Convert());
   auto hDeltaCasesUK = new TH1F("hDeltaCasesUK","DeltaCases (UK)",(dend.Convert()-dstart.Convert())/(3600*24),dstart.Convert(),dend.Convert());
   auto hDeltaCasesUS = new TH1F("hDeltaCasesUS","DeltaCases (US)",(dend.Convert()-dstart.Convert())/(3600*24),dstart.Convert(),dend.Convert());

   setHistStyle(hDeltaCasesChina, kBlack);
   setHistStyle(hDeltaCasesKorea, kRed);
   setHistStyle(hDeltaCasesJapan, kBlue);
   setHistStyle(hDeltaCasesIran, kGreen+2);
   setHistStyle(hDeltaCasesItaly, kYellow+2);
   setHistStyle(hDeltaCasesFrance, kViolet);
   setHistStyle(hDeltaCasesGermany, kOrange);
   setHistStyle(hDeltaCasesSwitzerland, kTeal);
   setHistStyle(hDeltaCasesUK, kPink);
   setHistStyle(hDeltaCasesUS, kAzure);

   // tr->Draw("date.Convert()>>hDeltaCasesChina","Sum$(DeltaCases*(Country==\"China\"))","hist L");
   // tr->Draw("date.Convert()>>hDeltaCasesKorea","Sum$(DeltaCases*(Country==\"South Korea\"))","hist L same");
   // tr->Draw("date.Convert()>>hDeltaCasesJapan","Sum$(DeltaCases*(Country==\"Japan\"))","hist L same");
   // tr->Draw("date.Convert()>>hDeltaCasesIran","Sum$(DeltaCases*(Country==\"Iran\"))","hist L same");
   // tr->Draw("date.Convert()>>hDeltaCasesItaly","Sum$(DeltaCases*(Country==\"Italy\"))","hist L same");
   // tr->Draw("date.Convert()>>hDeltaCasesFrance","Sum$(DeltaCases*(Country==\"France\"))","hist L same");
   // tr->Draw("date.Convert()>>hDeltaCasesGermany","Sum$(DeltaCases*(Country==\"Germany\"))","hist L same");
   // tr->Draw("date.Convert()>>hDeltaCasesSwitzerland","Sum$(DeltaCases*(Country==\"Switzerland\"))","hist L same");
   // tr->Draw("date.Convert()>>hDeltaCasesUK","Sum$(DeltaCases*(Country==\"UK\"))","hist L same");
   // tr->Draw("date.Convert()>>hDeltaCasesUS","Sum$(DeltaCases*(Country==\"US\"))","hist L same");
   makeDelta(hCasesChina,hDeltaCasesChina); hDeltaCasesChina->Draw("hist L");
   // fix the "bug" in the China delta
   hDeltaCasesChina->GetYaxis()->SetRangeUser(0,5000); 
   hDeltaCasesChina->Draw("hist L");
   makeDelta(hCasesKorea,hDeltaCasesKorea); hDeltaCasesKorea->Draw("hist L same");
   makeDelta(hCasesJapan,hDeltaCasesJapan); hDeltaCasesJapan->Draw("hist L same");
   makeDelta(hCasesIran,hDeltaCasesIran); hDeltaCasesIran->Draw("hist L same");
   makeDelta(hCasesItaly,hDeltaCasesItaly); hDeltaCasesItaly->Draw("hist L same");
   makeDelta(hCasesFrance,hDeltaCasesFrance); hDeltaCasesFrance->Draw("hist L same");
   makeDelta(hCasesGermany,hDeltaCasesGermany); hDeltaCasesGermany->Draw("hist L same");
   makeDelta(hCasesSwitzerland,hDeltaCasesSwitzerland); hDeltaCasesSwitzerland->Draw("hist L same");
   makeDelta(hCasesUK,hDeltaCasesUK); hDeltaCasesUK->Draw("hist L same");
   makeDelta(hCasesUS,hDeltaCasesUS); hDeltaCasesUS->Draw("hist L same");

   TLegend *tleg2 = new TLegend(0.590912,0.362665,0.807722,0.894349);
   tleg2->SetBorderSize(0);
   tleg2->AddEntry(hDeltaCasesChina,"China #Delta cases","L");
   tleg2->AddEntry(hDeltaCasesKorea,"Korea #Delta cases","L");
   tleg2->AddEntry(hDeltaCasesJapan,"Japan #Delta cases","L");
   tleg2->AddEntry(hDeltaCasesIran,"Iran #Delta cases","L");
   tleg2->AddEntry(hDeltaCasesItaly,"Italy #Delta cases","L");
   tleg2->AddEntry(hDeltaCasesFrance,"France #Delta cases","L");
   tleg2->AddEntry(hDeltaCasesGermany,"Germany #Delta cases","L");
   tleg2->AddEntry(hDeltaCasesSwitzerland,"Switzerland #Delta cases","L");
   tleg2->AddEntry(hDeltaCasesUK,"UK #Delta cases","L");
   tleg2->AddEntry(hDeltaCasesUS,"US #Delta cases","L");
   tleg2->Draw();
}
