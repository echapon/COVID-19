# CSSE COVID-19 Dataset: Émilien Chapon's ROOT transcript

## available macros
* csv2root.C: convert a list of CSV files (daily reports) to a ROOT file. See second line of list.txt for how to run it.
* drawhist.C: example macro to draw an histogram.
* drawMulti.C: standard suite of plots.

## organisation of the ROOT file: data_covid19.root
    tree->Show(0)
    ======> EVENT:0
    Province        = (vector<TString>*)0x7f500c001880
    Country         = (vector<TString>*)0x7f5014001a60
    LastUpdated     = (vector<TString>*)0x2ce8170
    Cases           = (vector<int>*)0x7f5014012240
    Deaths          = (vector<int>*)0x7f5014012740
    Recovered       = (vector<int>*)0x7f5014032c70
    DeltaCases      = (vector<int>*)0x7f501001da50
    DeltaDeaths     = (vector<int>*)0x7f501001e6b0
    DeltaRecovered  = (vector<int>*)0x7f5010030eb0
    date            = (TDatime*)0x7f5010063fe0

* one entry per day. The date for each entry is in TDatime format.
* data are structured in std::vectors, all of the same size. One element per Province/Country.
* for each (Province, Country) couple, the following numbers are available:
  * Cases: total confirmed cases since the start
  * Deaths
  * Recovered
  * DeltaCases, DeltaDeaths, DeltaRecovered: defined as Cases(day N) - Cases(day N-1). Note: these numbers are not always reliable, especially as the (Province, Country) division can change from one day to the other.

