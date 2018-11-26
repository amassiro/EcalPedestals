//
//---- plot pedestals
//

void drawWithType(std::string nameInputFile = "ana_ped_2016-2017.root", int index = 0, int isEE = 0, int runNumber = -1) {
  
  std::cout << " runNumber = " << runNumber << std::endl;
  
  gStyle->SetOptStat(0);
    
  TFile* fileIn = new TFile(nameInputFile.c_str(), "READ");
  TTree* T       = (TTree*) fileIn -> Get ("T");
  TTree* PedChan = (TTree*) fileIn -> Get ("PedChan");
  
  Int_t           Channels;
  Int_t           x;
  Int_t           y;
  Int_t           z;
  
  PedChan -> SetBranchAddress("Channels", &Channels);
  PedChan -> SetBranchAddress("x", &x);
  PedChan -> SetBranchAddress("y", &y);
  PedChan -> SetBranchAddress("z", &z);
  
  std::cout << " index = " << index << std::endl;
  std::cout << " isEE = " << isEE << std::endl;
  
  if (isEE) {
    index += 61200;
  }
  std::cout << " index = " << index << std::endl;
  
  
  PedChan->GetEntry(index);
  
  std::cout << " x :  " << x << std::endl;
  std::cout << " y :  " << y << std::endl;
  std::cout << " z :  " << z << std::endl;
  std::cout << " Channels :  " << Channels << std::endl;
  
  
  Int_t           fed[75848];
  
  T -> SetBranchAddress ("fed", fed);
  
  
  int nEntries = T->GetEntries();

  std::cout << " nEntries = " << nEntries << std::endl;
  
//   
//   run_type --> 1=collisions, 2=cosmics, 3=circulating, 4=test, 0=other not classified 
//   
  
  //----> cosmics
  
  TEntryList* myListCosmics;
  if (runNumber != -1) {
    TString cut = Form ("run == %d && run_type==2", runNumber);
    T -> SetEntryList(0); 
    T -> Draw(">> myListCosmics",cut.Data(),"entrylist");
    myListCosmics = (TEntryList*)gDirectory->Get("myListCosmics");
    T -> SetEntryList(myListCosmics);    
  }
  else {
    TString cut = Form ("run_type==2");
    T -> SetEntryList(0); 
    T -> Draw(">> myListCosmics",cut.Data(),"entrylist");
    myListCosmics = (TEntryList*)gDirectory->Get("myListCosmics");
    T -> SetEntryList(myListCosmics);        
  }
  
  nEntries = T->GetEntries();
  
  std::cout << " --> nEntries cosmics = " << nEntries << std::endl;
     
  int fedNumber = -1;
  
  //---- decide which crystal and fed we will display
  
  //---- avoid truncated sequences with "0"
  for (int ientry = 0; ientry < nEntries; ientry++) {
    T->GetEntry(ientry);
    fedNumber = fed [index] - 601;
    if (fed [index] != 0) {
      break;
    }
  }
  
//   T->GetEntry(0);
//   fedNumber = fed [index] - 601; //---- first fed for ECAL is 601
  std::cout << " fedNumber = " << fedNumber << " = " << fed [index] << " - 601  = fed[" << index << "] - 601" << std::endl;
  
  
  TString ped_string  = Form ("ped[%d]", index);
  TString rms_string  = Form ("pedrms[%d]", index);
  TString time_string = Form ("time[%d]", fedNumber);
  TString toDraw = Form ("%s : %s : %s", ped_string.Data(), rms_string.Data(), time_string.Data());
  
  std::cout << " toDraw = " << toDraw.Data() << std::endl;
  
  T->Draw(toDraw.Data(), "", "goff");
  TGraph *gr_ped_cosmics  = new TGraph(T->GetSelectedRows(), T->GetV3(), T->GetV1());  
  TGraph *gr_rms_cosmics  = new TGraph(T->GetSelectedRows(), T->GetV3(), T->GetV2());  
  
  std::cout << "     --> nEntries cosmics T->GetSelectedRows() = " << T->GetSelectedRows() << std::endl;
  
  
  //----> collisions
  
  
  TEntryList* myListCollisions;
  if (runNumber != -1) {
    TString cut = Form ("run == %d && run_type==1", runNumber);
    T -> SetEntryList(0); 
    T -> Draw(">> myListCollisions",cut.Data(),"entrylist");
    myListCollisions = (TEntryList*)gDirectory->Get("myListCollisions");
    T -> SetEntryList(myListCollisions);    
  }
  else {
    TString cut = Form ("run_type==1");
    T -> SetEntryList(0); 
    T -> Draw(">> myListCollisions",cut.Data(),"entrylist");
    myListCollisions = (TEntryList*)gDirectory->Get("myListCollisions");
    T -> SetEntryList(myListCollisions);        
  }
  
  
  
  std::cout << " --> nEntries collisions = " << nEntries << std::endl;
  
  fedNumber = -1;
  
  //---- decide which crystal and fed we will display
  //---- avoid truncated sequences with "0"
  for (int ientry = 0; ientry < nEntries; ientry++) {
    T->GetEntry(ientry);
    fedNumber = fed [index] - 601;
    if (fed [index] != 0) {
      break;
    }
  }
  
  //   T->GetEntry(0);
  //   fedNumber = fed [index] - 601; //---- first fed for ECAL is 601
  std::cout << " fedNumber = " << fedNumber << " = " << fed [index] << " - 601  = fed[" << index << "] - 601" << std::endl;
  
  
  
  T->Draw(toDraw.Data(), "", "goff");
  TGraph *gr_ped_collisions  = new TGraph(T->GetSelectedRows(), T->GetV3(), T->GetV1());  
  TGraph *gr_rms_collisions  = new TGraph(T->GetSelectedRows(), T->GetV3(), T->GetV2());  

  std::cout << "     --> nEntries collisions T->GetSelectedRows() = " << T->GetSelectedRows() << std::endl;
  
  
  
  
  //---- style ----
  
  gr_ped_cosmics->SetMarkerSize  (1);               
  gr_ped_cosmics->SetMarkerStyle (24);              
  gr_ped_cosmics->SetMarkerColor (kRed);            
  gr_ped_cosmics->SetLineWidth (1);                 
  gr_ped_cosmics->SetLineColor (kRed);              
  
  gr_rms_cosmics->SetMarkerSize  (1);               
  gr_rms_cosmics->SetMarkerStyle (24);              
  gr_rms_cosmics->SetMarkerColor (kBlue);            
  gr_rms_cosmics->SetLineWidth (1);                 
  gr_rms_cosmics->SetLineColor (kBlue);              
 
  
  gr_ped_collisions->SetMarkerSize  (1);               
  gr_ped_collisions->SetMarkerStyle (23);              
  gr_ped_collisions->SetMarkerColor (kGreen);            
  gr_ped_collisions->SetLineWidth (1);                 
  gr_ped_collisions->SetLineColor (kGreen);              
  
  gr_rms_collisions->SetMarkerSize  (1);               
  gr_rms_collisions->SetMarkerStyle (23);              
  gr_rms_collisions->SetMarkerColor (kGreen);            
  gr_rms_collisions->SetLineWidth (1);                 
  gr_rms_collisions->SetLineColor (kGreen);              
  
  //---- style (end) ----
  
  
  TCanvas* ccPed = new TCanvas ("ccPed","",1600,600);
  gr_ped_cosmics->Draw("AP");
  gr_ped_cosmics->GetYaxis()->SetTitle("ped ADC");
  gr_ped_cosmics->GetXaxis()->SetTitle("time");
  gr_ped_cosmics->GetXaxis()->SetTimeDisplay(1);
  gr_ped_cosmics->GetXaxis()->SetNdivisions(-503);
  gr_ped_cosmics->GetXaxis()->SetTimeFormat("%Y-%m-%d %H:%M");
  gr_ped_cosmics->GetXaxis()->SetTimeOffset(0,"gmt");

  gr_ped_collisions->Draw("P");
  ccPed->SetGrid();
  
  
  
  TCanvas* ccRms = new TCanvas ("ccRms","",1600,600);
  gr_rms_cosmics->Draw("AP");
  gr_rms_cosmics->GetYaxis()->SetTitle("rms ADC");
  gr_rms_cosmics->GetXaxis()->SetTitle("time");
  gr_rms_cosmics->GetXaxis()->SetTimeDisplay(1);
  gr_rms_cosmics->GetXaxis()->SetNdivisions(-503);
  gr_rms_cosmics->GetXaxis()->SetTimeFormat("%Y-%m-%d %H:%M");
  gr_rms_cosmics->GetXaxis()->SetTimeOffset(0,"gmt");
 
  gr_rms_collisions->Draw("P");
  
  ccRms->SetGrid();
  
  
}




