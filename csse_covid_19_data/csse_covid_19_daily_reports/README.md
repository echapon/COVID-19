# CSSE COVID-19 Dataset: Émilien Chapon's ROOT transcript

## available macros
* csv2root.C: convert a list of CSV files (daily reports) to a ROOT file. See second line of list.txt for how to run it.
* drawhist.C: example macro to draw an histogram.
* drawMulti.C: standard suite of plots.
* fithist.C: fitting the histogram produced with drawhist.C with a logistic (counts) or Gaussian (Delta). WORK IN PROGRESS

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

## Reminder about the data (from the repository's README)
This is the data repository for the 2019 Novel Coronavirus Visual Dashboard operated by the Johns Hopkins University Center for Systems Science and Engineering (JHU CSSE). Also, Supported by ESRI Living Atlas Team and the Johns Hopkins University Applied Physics Lab (JHU APL).

<br>

<b>Visual Dashboard (desktop):</b><br>
https://www.arcgis.com/apps/opsdashboard/index.html#/bda7594740fd40299423467b48e9ecf6
<br><br>
<b>Visual Dashboard (mobile):</b><br>
http://www.arcgis.com/apps/opsdashboard/index.html#/85320e2ea5424dfaaa75ae62e5c06e61
<br><br>
<b>Lancet Article:</b><br>
[An interactive web-based dashboard to track COVID-19 in real time](https://doi.org/10.1016/S1473-3099(20)30120-1)
<br><br>
<b>Provided by Johns Hopkins University Center for Systems Science and Engineering (JHU CSSE):</b><br>
https://systems.jhu.edu/
<br><br>
<b>Data Sources:</b><br>
* World Health Organization (WHO): https://www.who.int/ <br>
* DXY.cn. Pneumonia. 2020. http://3g.dxy.cn/newh5/view/pneumonia.  <br>
* BNO News: https://bnonews.com/index.php/2020/02/the-latest-coronavirus-cases/  <br>
* National Health Commission of the People’s Republic of China (NHC): <br>
 http://www.nhc.gov.cn/xcs/yqtb/list_gzbd.shtml <br>
* China CDC (CCDC): http://weekly.chinacdc.cn/news/TrackingtheEpidemic.htm <br>
* Hong Kong Department of Health: https://www.chp.gov.hk/en/features/102465.html <br>
* Macau Government: https://www.ssm.gov.mo/portal/ <br>
* Taiwan CDC: https://sites.google.com/cdc.gov.tw/2019ncov/taiwan?authuser=0 <br>
* US CDC: https://www.cdc.gov/coronavirus/2019-ncov/index.html <br>
* Government of Canada: https://www.canada.ca/en/public-health/services/diseases/coronavirus.html <br>
* Australia Government Department of Health: https://www.health.gov.au/news/coronavirus-update-at-a-glance <br>
* European Centre for Disease Prevention and Control (ECDC): https://www.ecdc.europa.eu/en/geographical-distribution-2019-ncov-cases 
* Ministry of Health Singapore (MOH): https://www.moh.gov.sg/covid-19
* Italy Ministry of Health: http://www.salute.gov.it/nuovocoronavirus

<br>
<b>Additional Information about the Visual Dashboard:</b><br>
https://systems.jhu.edu/research/public-health/ncov/
<br><br>

<b>Contact Us: </b><br>
* Email: jhusystems@gmail.com
<br><br>

<b>Terms of Use:</b><br>

This GitHub repo and its contents herein, including all data, mapping, and analysis, copyright 2020 Johns Hopkins University, all rights reserved, is provided to the public strictly for educational and academic research purposes.  The Website relies upon publicly available data from multiple sources, that do not always agree. The Johns Hopkins University hereby disclaims any and all representations and warranties with respect to the Website, including accuracy, fitness for use, and merchantability.  Reliance on the Website for medical guidance or use of the Website in commerce is strictly prohibited.
