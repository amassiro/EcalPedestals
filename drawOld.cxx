//
//---- plot pedestals
//

void draw(std::string nameInputFile = "ana_ped_2016-2017.root", int index = 0, int runNumber = -1) {
  
  std::cout << " runNumber = " << runNumber << std::endl;
  
  gStyle->SetOptStat(0);
    
  TFile* fileIn = new TFile(nameInputFile.c_str(), "READ");
  TTree* T       = (TTree*) fileIn -> Get ("T");
  TTree* PedChan = (TTree*) fileIn -> Get ("PedChan");
  
//   TGraph *gr_ped  = new TGraph();  
//   TGraph *gr_rms  = new TGraph();  
  
  Int_t           id;
  Int_t           run;
  Int_t           run_type;
  Int_t           seq_id;
  Int_t           las_id;
  Int_t           fill_num;
  Int_t           run_num_infill;
  Int_t           run_time;
  Int_t           run_time_stablebeam;
  Float_t         lumi;
  Float_t         bfield;
  Int_t           nxt;
  Int_t           time[54];
  Int_t           fed[75848];
  Int_t           chan[75848];
  Float_t         ped[75848];
  Float_t         pedrms[75848];
  
  T -> SetBranchAddress ("id", &id);
  T -> SetBranchAddress ("run", &run);
  T -> SetBranchAddress ("run_type", &run_type);
  T -> SetBranchAddress ("seq_id", &seq_id);
  T -> SetBranchAddress ("las_id", &las_id);
  T -> SetBranchAddress ("fill_num", &fill_num);
  T -> SetBranchAddress ("run_num_infill", &run_num_infill);
  T -> SetBranchAddress ("run_time", &run_time);
  T -> SetBranchAddress ("run_time_stablebeam", &run_time_stablebeam);
  T -> SetBranchAddress ("lumi", &lumi);
  T -> SetBranchAddress ("bfield", &bfield);
  T -> SetBranchAddress ("nxt", &nxt);
  T -> SetBranchAddress ("time", time);
  T -> SetBranchAddress ("fed", fed);
  T -> SetBranchAddress ("chan", chan);
  T -> SetBranchAddress ("ped", ped);
  T -> SetBranchAddress ("pedrms", pedrms);
  
    
  int nEntries = T->GetEntries();

  std::cout << " nEntries = " << nEntries << std::endl;
  
  TEntryList* myList;
  if (runNumber != -1) {
    TString cut = Form ("run == %d", runNumber);
    T -> SetEntryList(0); 
    T -> Draw(">> myList",cut.Data(),"entrylist");
    myList = (TEntryList*)gDirectory->Get("myList");
    T -> SetEntryList(myList); 
    
//     nEntries = myList->GetN();
  }
   
  std::cout << " --> nEntries = " << nEntries << std::endl;
     
  int fedNumber = -1;
  
//   for (int iEntry = 0; iEntry < nEntries; iEntry++) {
// 
//     if (!(iEntry%1000)) {
//       std::cout << " iEntry = " << iEntry << " :: " << nEntries << std::endl;
//     }
//     
// //     if (runNumber != -1) {
// //       myList -> Next();
// //     }
// //     else {    
//       T->GetEntry(iEntry);
// //     }
//     
//     //---- decide which crystal and fed we will display
//     if (fedNumber == -1) {
//       fedNumber = fed [index] - 601; //---- first fed for ECAL is 601
//     }
//     
//     gr_ped-> SetPoint (iEntry,   time [fedNumber]   ,   ped[index] ) ;           
//     gr_rms-> SetPoint (iEntry,   time [fedNumber]   ,   pedrms[index] ) ;   
//     
//   }
  
  
  
  //---- decide which crystal and fed we will display
  T->GetEntry(0);
  fedNumber = fed [index] - 601; //---- first fed for ECAL is 601
  
  
  TString ped_string  = Form ("ped[%d]", index);
  TString rms_string  = Form ("pedrms[%d]", index);
  TString time_string = Form ("time[%d]", fedNumber);
  TString toDraw = Form ("%s : %s : %s", ped_string.Data(), rms_string.Data(), time_string.Data());
  
  T->Draw(toDraw.Data(), "", "goff");
  TGraph *gr_ped  = new TGraph(T->GetSelectedRows(), T->GetV3(), T->GetV1());  
  TGraph *gr_rms  = new TGraph(T->GetSelectedRows(), T->GetV3(), T->GetV2());  
  
  
  //---- style ----
  
  gr_ped->SetMarkerSize  (1);               
  gr_ped->SetMarkerStyle (24);              
  gr_ped->SetMarkerColor (kRed);            
  gr_ped->SetLineWidth (1);                 
  gr_ped->SetLineColor (kRed);              
  
  gr_rms->SetMarkerSize  (1);               
  gr_rms->SetMarkerStyle (24);              
  gr_rms->SetMarkerColor (kBlue);            
  gr_rms->SetLineWidth (1);                 
  gr_rms->SetLineColor (kBlue);              
  
  //---- style (end) ----
  
  
  TCanvas* ccPed = new TCanvas ("ccPed","",1600,600);
  gr_ped->Draw("AP");
  gr_ped->GetYaxis()->SetTitle("ped ADC");
  gr_ped->GetXaxis()->SetTitle("time");
  gr_ped->GetXaxis()->SetTimeDisplay(1);
  gr_ped->GetXaxis()->SetNdivisions(-503);
  gr_ped->GetXaxis()->SetTimeFormat("%Y-%m-%d %H:%M");
  gr_ped->GetXaxis()->SetTimeOffset(0,"gmt");
  ccPed->SetGrid();
  
  
  
  TCanvas* ccRms = new TCanvas ("ccRms","",1600,600);
  gr_rms->Draw("AP");
  gr_rms->GetYaxis()->SetTitle("rms ADC");
  gr_rms->GetXaxis()->SetTitle("time");
  gr_rms->GetXaxis()->SetTimeDisplay(1);
  gr_rms->GetXaxis()->SetNdivisions(-503);
  gr_rms->GetXaxis()->SetTimeFormat("%Y-%m-%d %H:%M");
  gr_rms->GetXaxis()->SetTimeOffset(0,"gmt");
  ccRms->SetGrid();
  
  
}

