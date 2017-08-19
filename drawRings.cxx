//
//---- plot pedestals
//

void drawRings(std::string nameInputFile = "ana_ped_2016-2017.root", int runNumber = -1) {
  
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
  
   
  
  Int_t           fed[75848];
  
  T -> SetBranchAddress ("fed", fed);
  
  
  int nEntries = T->GetEntries();

  std::cout << " nEntries = " << nEntries << std::endl;
  
  TEntryList* myList;
  if (runNumber != -1) {
    TString cut = Form ("run == %d", runNumber);
    T -> SetEntryList(0); 
    T -> Draw(">> myList",cut.Data(),"entrylist");
    myList = (TEntryList*)gDirectory->Get("myList");
    T -> SetEntryList(myList); 
    
  }
   
  std::cout << " --> nEntries = " << nEntries << std::endl;
     
  int fedNumber = -1;
 
  //---- decide which crystal and fed we will display
  T->GetEntry(0);
  
  TCanvas* ccRms = new TCanvas ("ccRms","",1600,600);
  TCanvas* ccPed = new TCanvas ("ccPed","",1600,600);
  
  
  TGraph* gr_ped_ring_plus  [ 127 ];
  TGraph* gr_ped_ring_minus [ 127 ];
  //  85 + (50-11+3) = 
  //  85 + 42 = 
  //  127
  
  
  std::vector<float> ringPlus_ped;          std::vector<float> ringMinus_ped;
  std::vector<int> ringPlus_pedcount;       std::vector<int> ringMinus_pedcount;  
  
  
  
  
  for (int index = 0; index< 75848; index++) {

    if (!(index%1000)) std::cout << " [" << index << " :: " << 75848 << "]" << std::endl;
    
    fedNumber = fed [index] - 601; //---- first fed for ECAL is 601
    
    PedChan->GetEntry(index);
    
//     std::cout << " x :  " << x << std::endl;
//     std::cout << " y :  " << y << std::endl;
//     std::cout << " z :  " << z << std::endl;
    
    
//     if (z == 0) continue; //---- skip the EB
    
    
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
    
    
    gr_ped->Draw("AP");
    gr_ped->GetYaxis()->SetTitle("ped ADC");
    gr_ped->GetXaxis()->SetTitle("time");
    gr_ped->GetXaxis()->SetTimeDisplay(1);
    gr_ped->GetXaxis()->SetNdivisions(-503);
    gr_ped->GetXaxis()->SetTimeFormat("%Y-%m-%d %H:%M");
    gr_ped->GetXaxis()->SetTimeOffset(0,"gmt");
    ccPed->SetGrid();
    
    
    
    gr_rms->Draw("AP");
    gr_rms->GetYaxis()->SetTitle("rms ADC");
    gr_rms->GetXaxis()->SetTitle("time");
    gr_rms->GetXaxis()->SetTimeDisplay(1);
    gr_rms->GetXaxis()->SetNdivisions(-503);
    gr_rms->GetXaxis()->SetTimeFormat("%Y-%m-%d %H:%M");
    gr_rms->GetXaxis()->SetTimeOffset(0,"gmt");
    ccRms->SetGrid();
    
    TString namePlot = Form ("plot/ped_xyz_%d_%d_%d.png",x,y,z);
    ccPed->SaveAs(namePlot.Data());
    
    namePlot = Form ("plot/rms_xyz_%d_%d_%d.png",x,y,z);
    ccRms->SaveAs(namePlot.Data());
    
  }
  
}




