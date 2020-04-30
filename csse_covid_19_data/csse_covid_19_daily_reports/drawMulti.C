void setHistStyle(TH1F *hist, Color_t color = kBlack, int linestyle = 1) {
   hist->GetXaxis()->SetTimeDisplay(1);
   hist->GetXaxis()->SetTimeFormat("%m/%d");
   hist->GetXaxis()->SetTitle("Date [month/day]");
   hist->GetYaxis()->SetTitle("Counts");
   hist->SetMarkerColor(color);
   hist->SetLineColor(color);
   hist->SetLineStyle(linestyle);
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

   // start date is the first day you want, minus 12 hours
   TDatime dstart(2020,1,21,12,0,0);
   // end date is the last day you want, as 12:00:00
   TDatime dend(2020,4,29,12,0,0); 
   int ndays = TDatime::GetLegalGlobalDayFromDate(dend.GetDate()) - TDatime::GetLegalGlobalDayFromDate(dstart.GetDate());

   TCanvas *c1 = new TCanvas("coronavirus","coronavirus",1280,800);
   c1->Divide(2,2);

   ///////////////////////
   // WORLD TOTAL CASES //
   ///////////////////////

   c1->cd(1);
   auto hCasesWorld = new TH1F("hCasesWorld","Cases (World)",ndays,dstart.Convert(),dend.Convert());
   auto hDeathsWorld = new TH1F("hDeathsWorld","Deaths (World)",ndays,dstart.Convert(),dend.Convert());
   auto hRecoveredWorld = new TH1F("hRecoveredWorld","Recovered (World)",ndays,dstart.Convert(),dend.Convert());
   auto hCasesActiveWorld = new TH1F("hCasesActiveWorld","CasesActive (World)",ndays,dstart.Convert(),dend.Convert());

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
   auto hDeltaCasesWorld = new TH1F("hDeltaCasesWorld","DeltaCases (World)",ndays,dstart.Convert(),dend.Convert());
   auto hDeltaDeathsWorld = new TH1F("hDeltaDeathsWorld","DeltaDeaths (World)",ndays,dstart.Convert(),dend.Convert());
   auto hDeltaRecoveredWorld = new TH1F("hDeltaRecoveredWorld","DeltaRecovered (World)",ndays,dstart.Convert(),dend.Convert());
   auto hDeltaCasesActiveWorld = new TH1F("hDeltaCasesActiveWorld","DeltaCasesActive (World)",ndays,dstart.Convert(),dend.Convert());

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
   

   TLegend *tlegWorld2 = new TLegend(0.18,0.6,0.48,0.9);
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
   auto hCasesChina = new TH1F("hCasesChina","Cases (China)",ndays,dstart.Convert(),dend.Convert());
   auto hCasesKorea = new TH1F("hCasesKorea","Cases (Korea)",ndays,dstart.Convert(),dend.Convert());
   auto hCasesJapan = new TH1F("hCasesJapan","Cases (Japan)",ndays,dstart.Convert(),dend.Convert());
   auto hCasesIran = new TH1F("hCasesIran","Cases (Iran)",ndays,dstart.Convert(),dend.Convert());
   auto hCasesItaly = new TH1F("hCasesItaly","Cases (Italy)",ndays,dstart.Convert(),dend.Convert());
   auto hCasesFrance = new TH1F("hCasesFrance","Cases (France)",ndays,dstart.Convert(),dend.Convert());
   auto hCasesGermany = new TH1F("hCasesGermany","Cases (Germany)",ndays,dstart.Convert(),dend.Convert());
   auto hCasesSwitzerland = new TH1F("hCasesSwitzerland","Cases (Switzerland)",ndays,dstart.Convert(),dend.Convert());
   auto hCasesSpain = new TH1F("hCasesSpain","Cases (Spain)",ndays,dstart.Convert(),dend.Convert());
   auto hCasesUS = new TH1F("hCasesUS","Cases (US)",ndays,dstart.Convert(),dend.Convert());
   auto hCasesUK = new TH1F("hCasesUK","Cases (UK)",ndays,dstart.Convert(),dend.Convert());

   setHistStyle(hCasesChina, kBlack);
   setHistStyle(hCasesKorea, kRed);
   setHistStyle(hCasesJapan, kBlue);
   setHistStyle(hCasesIran, kGreen+1);
   setHistStyle(hCasesItaly, kMagenta);
   setHistStyle(hCasesFrance, kCyan);
   setHistStyle(hCasesGermany, kYellow+1);
   setHistStyle(hCasesSwitzerland, kRed+1,2);
   setHistStyle(hCasesSpain, kBlue+1,2);
   setHistStyle(hCasesUS, kGreen+2,2);
   setHistStyle(hCasesUK, kYellow+2,2);

   tr->Draw("date.Convert()>>hCasesUS","Sum$(Cases*(Country==\"US\"))","hist L");
   tr->Draw("date.Convert()>>hCasesChina","Sum$(Cases*(Country==\"China\"))","hist L same");
   tr->Draw("date.Convert()>>hCasesKorea","Sum$(Cases*(Country==\"South Korea\"))","hist L same");
   tr->Draw("date.Convert()>>hCasesJapan","Sum$(Cases*(Country==\"Japan\"))","hist L same");
   tr->Draw("date.Convert()>>hCasesIran","Sum$(Cases*(Country==\"Iran\"))","hist L same");
   tr->Draw("date.Convert()>>hCasesItaly","Sum$(Cases*(Country==\"Italy\"))","hist L same");
   tr->Draw("date.Convert()>>hCasesFrance","Sum$(Cases*(Country==\"France\"))","hist L same");
   tr->Draw("date.Convert()>>hCasesGermany","Sum$(Cases*(Country==\"Germany\"))","hist L same");
   tr->Draw("date.Convert()>>hCasesSwitzerland","Sum$(Cases*(Country==\"Switzerland\"))","hist L same");
   tr->Draw("date.Convert()>>hCasesSpain","Sum$(Cases*(Country==\"Spain\"))","hist L same");
   tr->Draw("date.Convert()>>hCasesUK","Sum$(Cases*(Country==\"UK\"))","hist L same");

   TLegend *tleg = new TLegend(0.22,.13,.43,.67);
   tleg->SetBorderSize(0);
   tleg->AddEntry(hCasesChina,"China cases","L");
   tleg->AddEntry(hCasesKorea,"Korea cases","L");
   tleg->AddEntry(hCasesJapan,"Japan cases","L");
   tleg->AddEntry(hCasesIran,"Iran cases","L");
   tleg->AddEntry(hCasesItaly,"Italy cases","L");
   tleg->AddEntry(hCasesFrance,"France cases","L");
   tleg->AddEntry(hCasesGermany,"Germany cases","L");
   tleg->AddEntry(hCasesSwitzerland,"Switzerland cases","L");
   tleg->AddEntry(hCasesSpain,"Spain cases","L");
   tleg->AddEntry(hCasesUS,"US cases","L");
   tleg->AddEntry(hCasesUK,"UK cases","L");
   tleg->Draw();
   

   ///////////////////////////////////////////
   // focus on a few countries: Delta cases //
   ///////////////////////////////////////////

   c1->cd(4);
   // gPad->SetLogy();
   auto hDeltaCasesChina = new TH1F("hDeltaCasesChina","DeltaCases (China)",ndays,dstart.Convert(),dend.Convert());
   auto hDeltaCasesKorea = new TH1F("hDeltaCasesKorea","DeltaCases (Korea)",ndays,dstart.Convert(),dend.Convert());
   auto hDeltaCasesJapan = new TH1F("hDeltaCasesJapan","DeltaCases (Japan)",ndays,dstart.Convert(),dend.Convert());
   auto hDeltaCasesIran = new TH1F("hDeltaCasesIran","DeltaCases (Iran)",ndays,dstart.Convert(),dend.Convert());
   auto hDeltaCasesItaly = new TH1F("hDeltaCasesItaly","DeltaCases (Italy)",ndays,dstart.Convert(),dend.Convert());
   auto hDeltaCasesFrance = new TH1F("hDeltaCasesFrance","DeltaCases (France)",ndays,dstart.Convert(),dend.Convert());
   auto hDeltaCasesGermany = new TH1F("hDeltaCasesGermany","DeltaCases (Germany)",ndays,dstart.Convert(),dend.Convert());
   auto hDeltaCasesSwitzerland = new TH1F("hDeltaCasesSwitzerland","DeltaCases (Switzerland)",ndays,dstart.Convert(),dend.Convert());
   auto hDeltaCasesSpain = new TH1F("hDeltaCasesSpain","DeltaCases (Spain)",ndays,dstart.Convert(),dend.Convert());
   auto hDeltaCasesUS = new TH1F("hDeltaCasesUS","DeltaCases (US)",ndays,dstart.Convert(),dend.Convert());
   auto hDeltaCasesUK = new TH1F("hDeltaCasesUK","DeltaCases (UK)",ndays,dstart.Convert(),dend.Convert());

   setHistStyle(hDeltaCasesChina, kBlack);
   setHistStyle(hDeltaCasesKorea, kRed);
   setHistStyle(hDeltaCasesJapan, kBlue);
   setHistStyle(hDeltaCasesIran, kGreen+1);
   setHistStyle(hDeltaCasesItaly, kMagenta);
   setHistStyle(hDeltaCasesFrance, kCyan);
   setHistStyle(hDeltaCasesGermany, kYellow+1);
   setHistStyle(hDeltaCasesSwitzerland, kRed+1,2);
   setHistStyle(hDeltaCasesSpain, kBlue+1,2);
   setHistStyle(hDeltaCasesUS, kGreen+2,2);
   setHistStyle(hDeltaCasesUK, kYellow+2,2);

   makeDelta(hCasesUS,hDeltaCasesUS); hDeltaCasesUS->Draw("hist L");
   makeDelta(hCasesItaly,hDeltaCasesItaly); hDeltaCasesItaly->Draw("hist L same");
   makeDelta(hCasesChina,hDeltaCasesChina); hDeltaCasesChina->Draw("hist L same");
   makeDelta(hCasesKorea,hDeltaCasesKorea); hDeltaCasesKorea->Draw("hist L same");
   makeDelta(hCasesJapan,hDeltaCasesJapan); hDeltaCasesJapan->Draw("hist L same");
   makeDelta(hCasesIran,hDeltaCasesIran); hDeltaCasesIran->Draw("hist L same");
   makeDelta(hCasesFrance,hDeltaCasesFrance); hDeltaCasesFrance->Draw("hist L same");
   makeDelta(hCasesGermany,hDeltaCasesGermany); hDeltaCasesGermany->Draw("hist L same");
   makeDelta(hCasesSwitzerland,hDeltaCasesSwitzerland); hDeltaCasesSwitzerland->Draw("hist L same");
   makeDelta(hCasesSpain,hDeltaCasesSpain); hDeltaCasesSpain->Draw("hist L same");
   makeDelta(hCasesUK,hDeltaCasesUK); hDeltaCasesUK->Draw("hist L same");

   TLegend *tleg2 = new TLegend(0.44,0.32,0.65,0.85);
   tleg2->SetBorderSize(0);
   tleg2->AddEntry(hDeltaCasesChina,"China #Delta cases","L");
   tleg2->AddEntry(hDeltaCasesKorea,"Korea #Delta cases","L");
   tleg2->AddEntry(hDeltaCasesJapan,"Japan #Delta cases","L");
   tleg2->AddEntry(hDeltaCasesIran,"Iran #Delta cases","L");
   tleg2->AddEntry(hDeltaCasesItaly,"Italy #Delta cases","L");
   tleg2->AddEntry(hDeltaCasesFrance,"France #Delta cases","L");
   tleg2->AddEntry(hDeltaCasesGermany,"Germany #Delta cases","L");
   tleg2->AddEntry(hDeltaCasesSwitzerland,"Switzerland #Delta cases","L");
   tleg2->AddEntry(hDeltaCasesSpain,"Spain #Delta cases","L");
   tleg2->AddEntry(hDeltaCasesUS,"US #Delta cases","L");
   tleg2->AddEntry(hDeltaCasesUK,"UK #Delta cases","L");
   tleg2->Draw();

   ////////////////////////////////////////////////////////////////////////////

   ////////////////////////////////////////////////////////////////////////////
   
   ////////////////////////////////////////////////////////////////////////////


   /////////////////////////////////////
   //         MORE GRAPHS: DEATHS     //
   /////////////////////////////////////

   TCanvas *c2 = new TCanvas("coronavirus_deaths","coronavirus deaths",1280,400);
   c2->Divide(2,1);
   

   /////////////////////////////////////
   // focus on a few countries: death //
   /////////////////////////////////////

   c2->cd(1);
   gPad->SetLogy();
   auto hDeathsChina = new TH1F("hDeathsChina","Deaths (China)",ndays,dstart.Convert(),dend.Convert());
   auto hDeathsKorea = new TH1F("hDeathsKorea","Deaths (Korea)",ndays,dstart.Convert(),dend.Convert());
   auto hDeathsJapan = new TH1F("hDeathsJapan","Deaths (Japan)",ndays,dstart.Convert(),dend.Convert());
   auto hDeathsIran = new TH1F("hDeathsIran","Deaths (Iran)",ndays,dstart.Convert(),dend.Convert());
   auto hDeathsItaly = new TH1F("hDeathsItaly","Deaths (Italy)",ndays,dstart.Convert(),dend.Convert());
   auto hDeathsFrance = new TH1F("hDeathsFrance","Deaths (France)",ndays,dstart.Convert(),dend.Convert());
   auto hDeathsGermany = new TH1F("hDeathsGermany","Deaths (Germany)",ndays,dstart.Convert(),dend.Convert());
   auto hDeathsSwitzerland = new TH1F("hDeathsSwitzerland","Deaths (Switzerland)",ndays,dstart.Convert(),dend.Convert());
   auto hDeathsSpain = new TH1F("hDeathsSpain","Deaths (Spain)",ndays,dstart.Convert(),dend.Convert());
   auto hDeathsUS = new TH1F("hDeathsUS","Deaths (US)",ndays,dstart.Convert(),dend.Convert());
   auto hDeathsUK = new TH1F("hDeathsUK","Deaths (UK)",ndays,dstart.Convert(),dend.Convert());

   setHistStyle(hDeathsChina, kBlack);
   setHistStyle(hDeathsKorea, kRed);
   setHistStyle(hDeathsJapan, kBlue);
   setHistStyle(hDeathsIran, kGreen+1);
   setHistStyle(hDeathsItaly, kMagenta);
   setHistStyle(hDeathsFrance, kCyan);
   setHistStyle(hDeathsGermany, kYellow+1);
   setHistStyle(hDeathsSwitzerland, kRed+1,2);
   setHistStyle(hDeathsSpain, kBlue+1,2);
   setHistStyle(hDeathsUS, kGreen+2,2);
   setHistStyle(hDeathsUK, kYellow+2,2);

   tr->Draw("date.Convert()>>hDeathsUS","Sum$(Deaths*(Country==\"US\"))","hist L");
   tr->Draw("date.Convert()>>hDeathsItaly","Sum$(Deaths*(Country==\"Italy\"))","hist L same");
   tr->Draw("date.Convert()>>hDeathsChina","Sum$(Deaths*(Country==\"China\"))","hist L same");
   tr->Draw("date.Convert()>>hDeathsKorea","Sum$(Deaths*(Country==\"South Korea\"))","hist L same");
   tr->Draw("date.Convert()>>hDeathsJapan","Sum$(Deaths*(Country==\"Japan\"))","hist L same");
   tr->Draw("date.Convert()>>hDeathsIran","Sum$(Deaths*(Country==\"Iran\"))","hist L same");
   tr->Draw("date.Convert()>>hDeathsFrance","Sum$(Deaths*(Country==\"France\"))","hist L same");
   tr->Draw("date.Convert()>>hDeathsGermany","Sum$(Deaths*(Country==\"Germany\"))","hist L same");
   tr->Draw("date.Convert()>>hDeathsSwitzerland","Sum$(Deaths*(Country==\"Switzerland\"))","hist L same");
   tr->Draw("date.Convert()>>hDeathsSpain","Sum$(Deaths*(Country==\"Spain\"))","hist L same");
   tr->Draw("date.Convert()>>hDeathsUK","Sum$(Deaths*(Country==\"UK\"))","hist L same");

   TLegend *tlegd = new TLegend(0.349211,0.236871,0.567651,0.771267);
   tlegd->SetBorderSize(0);
   tlegd->AddEntry(hDeathsChina,"China deaths","L");
   tlegd->AddEntry(hDeathsKorea,"Korea deaths","L");
   tlegd->AddEntry(hDeathsJapan,"Japan deaths","L");
   tlegd->AddEntry(hDeathsIran,"Iran deaths","L");
   tlegd->AddEntry(hDeathsItaly,"Italy deaths","L");
   tlegd->AddEntry(hDeathsFrance,"France deaths","L");
   tlegd->AddEntry(hDeathsGermany,"Germany deaths","L");
   tlegd->AddEntry(hDeathsSwitzerland,"Switzerland deaths","L");
   tlegd->AddEntry(hDeathsSpain,"Spain deaths","L");
   tlegd->AddEntry(hDeathsUS,"US deaths","L");
   tlegd->AddEntry(hDeathsUK,"UK deaths","L");
   tlegd->Draw();
   

   ///////////////////////////////////////////
   // focus on a few countries: Delta deaths//
   ///////////////////////////////////////////

   c2->cd(2);
   // gPad->SetLogy();
   auto hDeltaDeathsChina = new TH1F("hDeltaDeathsChina","DeltaDeaths (China)",ndays,dstart.Convert(),dend.Convert());
   auto hDeltaDeathsKorea = new TH1F("hDeltaDeathsKorea","DeltaDeaths (Korea)",ndays,dstart.Convert(),dend.Convert());
   auto hDeltaDeathsJapan = new TH1F("hDeltaDeathsJapan","DeltaDeaths (Japan)",ndays,dstart.Convert(),dend.Convert());
   auto hDeltaDeathsIran = new TH1F("hDeltaDeathsIran","DeltaDeaths (Iran)",ndays,dstart.Convert(),dend.Convert());
   auto hDeltaDeathsItaly = new TH1F("hDeltaDeathsItaly","DeltaDeaths (Italy)",ndays,dstart.Convert(),dend.Convert());
   auto hDeltaDeathsFrance = new TH1F("hDeltaDeathsFrance","DeltaDeaths (France)",ndays,dstart.Convert(),dend.Convert());
   auto hDeltaDeathsGermany = new TH1F("hDeltaDeathsGermany","DeltaDeaths (Germany)",ndays,dstart.Convert(),dend.Convert());
   auto hDeltaDeathsSwitzerland = new TH1F("hDeltaDeathsSwitzerland","DeltaDeaths (Switzerland)",ndays,dstart.Convert(),dend.Convert());
   auto hDeltaDeathsSpain = new TH1F("hDeltaDeathsSpain","DeltaDeaths (Spain)",ndays,dstart.Convert(),dend.Convert());
   auto hDeltaDeathsUS = new TH1F("hDeltaDeathsUS","DeltaDeaths (US)",ndays,dstart.Convert(),dend.Convert());
   auto hDeltaDeathsUK = new TH1F("hDeltaDeathsUK","DeltaDeaths (UK)",ndays,dstart.Convert(),dend.Convert());

   setHistStyle(hDeltaDeathsChina, kBlack);
   setHistStyle(hDeltaDeathsKorea, kRed);
   setHistStyle(hDeltaDeathsJapan, kBlue);
   setHistStyle(hDeltaDeathsIran, kGreen+1);
   setHistStyle(hDeltaDeathsItaly, kMagenta);
   setHistStyle(hDeltaDeathsFrance, kCyan);
   setHistStyle(hDeltaDeathsGermany, kYellow+1);
   setHistStyle(hDeltaDeathsSwitzerland, kRed+1,2);
   setHistStyle(hDeltaDeathsSpain, kBlue+1,2);
   setHistStyle(hDeltaDeathsUS, kGreen+2,2);
   setHistStyle(hDeltaDeathsUK, kYellow+2,2);

   makeDelta(hDeathsUS,hDeltaDeathsUS); hDeltaDeathsUS->Draw("hist L");
   makeDelta(hDeathsFrance,hDeltaDeathsFrance); hDeltaDeathsFrance->Draw("hist L same");
   makeDelta(hDeathsItaly,hDeltaDeathsItaly); hDeltaDeathsItaly->Draw("hist L same");
   makeDelta(hDeathsChina,hDeltaDeathsChina); hDeltaDeathsChina->Draw("hist L same");
   makeDelta(hDeathsKorea,hDeltaDeathsKorea); hDeltaDeathsKorea->Draw("hist L same");
   makeDelta(hDeathsJapan,hDeltaDeathsJapan); hDeltaDeathsJapan->Draw("hist L same");
   makeDelta(hDeathsIran,hDeltaDeathsIran); hDeltaDeathsIran->Draw("hist L same");
   makeDelta(hDeathsGermany,hDeltaDeathsGermany); hDeltaDeathsGermany->Draw("hist L same");
   makeDelta(hDeathsSwitzerland,hDeltaDeathsSwitzerland); hDeltaDeathsSwitzerland->Draw("hist L same");
   makeDelta(hDeathsSpain,hDeltaDeathsSpain); hDeltaDeathsSpain->Draw("hist L same");
   makeDelta(hDeathsUK,hDeltaDeathsUK); hDeltaDeathsUK->Draw("hist L same");

   TLegend *tlegd2 = new TLegend(0.19,0.362665,0.40,0.894349);
   tlegd2->SetBorderSize(0);
   tlegd2->AddEntry(hDeltaDeathsChina,"China #Delta deaths","L");
   tlegd2->AddEntry(hDeltaDeathsKorea,"Korea #Delta deaths","L");
   tlegd2->AddEntry(hDeltaDeathsJapan,"Japan #Delta deaths","L");
   tlegd2->AddEntry(hDeltaDeathsIran,"Iran #Delta deaths","L");
   tlegd2->AddEntry(hDeltaDeathsItaly,"Italy #Delta deaths","L");
   tlegd2->AddEntry(hDeltaDeathsFrance,"France #Delta deaths","L");
   tlegd2->AddEntry(hDeltaDeathsGermany,"Germany #Delta deaths","L");
   tlegd2->AddEntry(hDeltaDeathsSwitzerland,"Switzerland #Delta deaths","L");
   tlegd2->AddEntry(hDeltaDeathsSpain,"Spain #Delta deaths","L");
   tlegd2->AddEntry(hDeltaDeathsUS,"US #Delta deaths","L");
   tlegd2->AddEntry(hDeltaDeathsUK,"UK #Delta deaths","L");
   tlegd2->Draw();
}
