#include "TFile.h"
#include "TTree.h"
#include "TString.h"
#include "TDatime.h"

#include <fstream>
#include <algorithm>
#include <iostream>
#include <map>
#include <vector>

using namespace std;

void csv2root(TString filenames) {
   TFile f("data_covd19.root","RECREATE");
   TTree tr("tree","tree");
   // tr.ReadFile(filename, "Province/C:Country/C:LastUpdate/C:Confirmed/I:Deaths/I:Recovered/I",',');

   // char Province[99];
   // char Country[99];
   // char LastUpdated[99];
   // int Cases(0), Deaths(0), Recovered(0), DeltaCases(0), DeltaDeaths(0), DeltaRecovered(0);
   vector<TString> Province, Country, LastUpdated;
   vector<int> Cases, Deaths, Recovered, DeltaCases, DeltaDeaths, DeltaRecovered;
   TDatime date;

   map<TString,int> prevCases;
   map<TString,int> prevDeaths;
   map<TString,int> prevRecovered;

   tr.Branch("Province",&Province);
   tr.Branch("Country",&Country);
   tr.Branch("LastUpdated",&LastUpdated);
   tr.Branch("Cases",&Cases);
   tr.Branch("Deaths",&Deaths);
   tr.Branch("Recovered",&Recovered);
   tr.Branch("DeltaCases",&DeltaCases);
   tr.Branch("DeltaDeaths",&DeltaDeaths);
   tr.Branch("DeltaRecovered",&DeltaRecovered);
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
            // fix for China
            if (cnt==0) Province.push_back(tok2);
            else if (cnt==1) {
               Country.push_back(tok2);
               // workaround for France: if Province is empty, then copy Country into it
               if (Province.back()=="") Province.back() = Country.back();
               // workaround for China
               if (Country.back()=="Mainland China") Country.back() = "China";
            } else if (cnt==2) LastUpdated.push_back(tok2);
            else if (cnt==3) {
               int CasesToday = atoi(tok2);
               DeltaCases.push_back(CasesToday - prevCases[Province.back()+Country.back()]);
               Cases.push_back(CasesToday);
               prevCases[Province.back()+Country.back()] = CasesToday;
            } else if (cnt==5) {
               int DeathsToday = atoi(tok2);
               DeltaDeaths.push_back(DeathsToday - prevDeaths[Province.back()+Country.back()]);
               Deaths.push_back(DeathsToday);
               prevDeaths[Province.back()+Country.back()] = DeathsToday;
            } else if (cnt==6) {
               int RecoveredToday = atoi(tok2);
               DeltaRecovered.push_back(RecoveredToday - prevRecovered[Province.back()+Country.back()]);
               Recovered.push_back(RecoveredToday);
               prevRecovered[Province.back()+Country.back()] = RecoveredToday;
            }
            cnt++;
         }
         // cout << Province << " " << Country << " " << LastUpdated << " " << Cases << " " << Deaths << " " << Recovered << " " << DeltaCases << " " << DeltaDeaths << " " << DeltaRecovered << endl;

      }

      tr.Fill();

      Province.clear();
      Country.clear();
      LastUpdated.clear();
      Cases.clear();
      Deaths.clear();
      Recovered.clear();
      DeltaCases.clear();
      DeltaDeaths.clear();
      DeltaRecovered.clear();

      ifs.close();
   }

   f.Write();
   f.Close();
}
