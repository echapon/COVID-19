#include "TFile.h"
#include "TTree.h"
#include "TString.h"
#include "TDatime.h"

#include <fstream>
#include <algorithm>
#include <iostream>
#include <map>

using namespace std;

void csv2root(TString filenames) {
   TFile f("data_covd19.root","RECREATE");
   TTree tr("tree","tree");
   // tr.ReadFile(filename, "Province/C:Country/C:LastUpdate/C:Confirmed/I:Deaths/I:Recovered/I",',');

   char Province[99];
   char Country[99];
   char LastUpdated[99];
   int Cases(0), Deaths(0), Recovered(0), DeltaCases(0), DeltaDeaths(0), DeltaRecovered(0);
   TDatime date;

   map<TString> prevCases, prevDeaths, prevRecovered;

   tr.Branch("Province",Province,"Province/C");
   tr.Branch("Country",Country,"Country/C");
   tr.Branch("LastUpdated",LastUpdated,"LastUpdated/C");
   tr.Branch("Cases",&Cases,"Cases/I");
   tr.Branch("Deaths",&Deaths,"Deaths/I");
   tr.Branch("Recovered",&Recovered,"Recovered/I");
   tr.Branch("DeltaCases",&DeltaCases,"DeltaCases/I");
   tr.Branch("DeltaDeaths",&DeltaDeaths,"DeltaDeaths/I");
   tr.Branch("DeltaRecovered",&DeltaRecovered,"DeltaRecovered/I");
   tr.Branch("date",&date);

   TString tok;
   Ssiz_t from = 0;
   while (filenames.Tokenize(tok, from, " ")) {
      ifstream ifs(tok.Data());

      // parse file name, make it a date
      TString parsedate(tok);
      parsedate.ReplaceAll(".csv","");
      TString tokdate;
      Ssiz_t fromdate = 0;
      int day,month,year;
      int cntdate(0);
      cout << parsedate.Data() << endl;
      while (parsedate.Tokenize(tokdate, fromdate, "-")) {
         if (cntdate==0) month = atoi(tokdate.Data());
         else if (cntdate==1) day = atoi(tokdate.Data());
         else if (cntdate==2) year = atoi(tokdate.Data());

         cntdate++;
      }
      date.Set(year,month,day,0,0,0);

      char theline[1024];
      while(ifs.is_open()) {
         ifs.getline(theline,1024);
         TString thelineT(theline);
         if (thelineT=="") break;
         // cout << theline << "EOL" << endl;

         TString tok2;
         Ssiz_t from2 = 0;
         int cnt=0;
         while (thelineT.Tokenize(tok2, from2, ",")) {
            if (cnt==0) sprintf(Province,tok2.Data());
            else if (cnt==1) sprintf(Country,tok2.Data());
            else if (cnt==2) sprintf(LastUpdated,tok2.Data());
            else if (cnt==3) {
               int CasesToday = atoi(tok2);
               DeltaCases = CasesToday - prevCases[TString(Province)+TString(Country)];
               Cases = CasesToday;
               prevCases[TString(Province)+TString(Country)] = CasesToday;
            } else if (cnt==5) {
               int DeathsToday = atoi(tok2);
               DeltaDeaths = DeathsToday - prevDeaths[TString(Province)+TString(Country)];
               Deaths = DeathsToday;
               prevDeaths[TString(Province)+TString(Country)] = DeathsToday;
            } else if (cnt==6) {
               int RecoveredToday = atoi(tok2);
               DeltaRecovered = RecoveredToday - prevRecovered[TString(Province)+TString(Country)];
               Recovered = RecoveredToday;
               prevCases[TString(Province)+TString(Country)] = CasesToday;
            }
            cnt++;
         }
         // cout << Province << " " << Country << " " << LastUpdated << " " << Cases << " " << Deaths << " " << Recovered << " " << DeltaCases << " " << DeltaDeaths << " " << DeltaRecovered << endl;

         tr.Fill();
      }

      ifs.close();
   }

   f.Write();
   f.Close();
}
