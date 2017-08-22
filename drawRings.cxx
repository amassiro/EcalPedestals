//
//---- plot pedestals
//

#include <algorithm>


int GetColorFromPalette ( TH2 * histo, Double_t z ) {
  /// http://root.cern.ch/phpBB3/viewtopic.php?f=12&t=4101&p=16258 -> See THistPainter::PaintColorLevels()
  Int_t ncolors  = gStyle->GetNumberOfColors() ;
  Int_t ndiv = histo->GetContour() ;
  if (ndiv == 0 ) { ndiv = gStyle->GetNumberContours() ; }
  Int_t ndivz = TMath::Abs(ndiv) ;
  Double_t zmin = histo->GetMinimum(), zmax = histo->GetMaximum() ;
  Double_t dz = zmax - zmin ;
  Double_t scale = ndivz/dz ;
  Int_t color = Int_t(0.01+(z-zmin)*scale) ;
  Int_t theColor = Int_t((color+0.99)*Float_t(ncolors)/Float_t(ndivz)) ;
  if (theColor > ncolors-1) theColor = ncolors-1 ;  
  // printf ("\ntheColor=%d",theColor) ;
  return (gStyle->GetColorPalette(theColor)) ;
}


// https://root-forum.cern.ch/t/th2-col-above-lego/16180
int GetColorFromPalette ( int iter, int max, int min=0) {
  /// http://root.cern.ch/phpBB3/viewtopic.php?f=12&t=4101&p=16258 -> See THistPainter::PaintColorLevels()
  //  https://root-forum.cern.ch/t/th2-col-above-lego/16180
  Int_t ncolors  = gStyle->GetNumberOfColors() ;
  Int_t ndiv = max - min;
  if (ndiv == 0 ) { ndiv = gStyle->GetNumberContours() ; }
  Int_t ndivz = TMath::Abs(ndiv) ;
  Double_t zmin = min, zmax = max ;
  Double_t dz = zmax - zmin ;
  Double_t scale = ndivz/dz ;
  Int_t color = Int_t(0.01+(iter-zmin)*scale) ;
  Int_t theColor = Int_t((color+0.99)*Float_t(ncolors)/Float_t(ndivz)) ;
  if (theColor > ncolors-1) theColor = ncolors-1 ;  
  return (gStyle->GetColorPalette(theColor)) ;
}




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
  
  
  //       ring       value-vs-time
  std::map< int , std::vector<float> > ringPlus_ped;          std::map< int, std::vector<float> > ringMinus_ped;
  std::map< int, std::vector<int> >   ringPlus_pedcount;      std::map< int, std::vector<int> >   ringMinus_pedcount;  
  
  
  std::vector<float> ring_time;
  
  
//   for (int index = 0; index< 3000; index++) {
    
  for (int index = 0; index< 75848; index++) {
    
    if (!(index%1000)) std::cout << " [" << index << " :: " << 75848 << "]" << std::endl;
    
    if ((index < 61200 && index%100) || (index > 61200 && index%20)) continue; //---- only 75848/100 ~ 700 xtals per ring, to test and speed up
//     if (index%50) continue; //---- only 75848/50 ~ 1400 xtals per ring, to test and speed up
    
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

    //---- just the first time
    if (ring_time.size() == 0) {
      // get timing ranges
      for (int itime = 0; itime < T->GetSelectedRows(); itime++) {
        ring_time.push_back( T->GetV3()[itime] );
        
        std::vector< float > temp;
        std::vector< int > temp_int;
        for (int iring = 0; iring < 127; iring++) {
          temp.push_back(0); 
          temp_int.push_back(0); 
        }
        
        ringPlus_ped[itime] = temp;
        ringPlus_pedcount[itime] = temp_int;
        ringMinus_ped[itime] = temp;
        ringMinus_pedcount[itime] = temp_int;
      }
    }
    
    //---- and the rest of the times do this
    
    int iring = 0;
    
    if (z != 0) {
      //---- EE
      float dx = x - 50.5;
      float dy = y - 50.5;
      
      float ring = sqrt( dx*dx + dy*dy );
      
//       42 -  iter
      iring = 41 - ( round(ring) - 11)  + 85;  //---- 12 [ = (62 - 50 - 1) from the 2D plot] is the first ring
//       iring = round(ring) - 11 + 85;  //---- 12 [ = (62 - 50 - 1) from the 2D plot] is the first ring
      //  +85, since the first 85 are for EB
    }
    else {
      //---- EB      
      iring = abs( x ) - 1 ;
    }
    
    
    int totalTime = 0;
    if (ring_time.size() > T->GetSelectedRows()) {
      totalTime = T->GetSelectedRows();
    }
    else {
      totalTime = ring_time.size();      
    }
    
    if (z>0 || (z==0 && x>0) ) { 
      for (int itime = 0; itime < totalTime; itime++) {
        ringPlus_pedcount[itime].at(iring)  =  ringPlus_pedcount[itime].at(iring)  + 1;
        ringPlus_ped[itime].at(iring)       =  ringPlus_ped[itime].at(iring) + gr_ped->Eval( ring_time.at(itime) ); //---- extrapolate
      }
    }
    else {
      for (int itime = 0; itime < totalTime; itime++) {
        ringMinus_pedcount[itime].at(iring)  =  ringMinus_pedcount[itime].at(iring)  + 1;
        ringMinus_ped[itime].at(iring)       =  ringMinus_ped[itime].at(iring) + gr_ped->Eval( ring_time.at(itime) ); //---- extrapolate
      }
    }
  
    delete gr_ped;
    delete gr_rms;
    
  }
  
  
  
  for (int iring = 0; iring < 127; iring++) {
    
    gr_ped_ring_plus[iring] = new TGraph();
    gr_ped_ring_minus[iring] = new TGraph();
    
    TString nameTitle = Form("ring + %d", iring);
    gr_ped_ring_plus[iring]  -> SetTitle( nameTitle.Data() );
    nameTitle = Form("ring - %d", iring);
    gr_ped_ring_minus[iring] -> SetTitle( nameTitle.Data() );
    
    for (int itime = 0; itime < ring_time.size(); itime++) {
      gr_ped_ring_plus[iring] -> SetPoint (itime, ring_time.at(itime),   ringPlus_ped[itime].at(iring)  ? ringPlus_ped[itime].at(iring)  / ringPlus_pedcount[itime].at(iring)  : 0 ) ;           
      gr_ped_ring_minus[iring] -> SetPoint (itime, ring_time.at(itime),  ringMinus_ped[itime].at(iring) ? ringMinus_ped[itime].at(iring) / ringMinus_pedcount[itime].at(iring) : 0 ) ;           
    }
    
    //---- style ----
    gr_ped_ring_plus[iring]->SetFillColor  (0);               
    gr_ped_ring_plus[iring]->SetMarkerSize  (1);               
    gr_ped_ring_plus[iring]->SetMarkerStyle (24);              
//     gr_ped_ring_plus[iring]->SetMarkerColor (iring % 50 +50);    
    gr_ped_ring_plus[iring]->SetMarkerColor (GetColorFromPalette ( iring, 127, 0) );        
    gr_ped_ring_plus[iring]->SetLineWidth (1);                 
    gr_ped_ring_plus[iring]->SetLineColor (GetColorFromPalette ( iring, 127, 0) );    
    
    gr_ped_ring_minus[iring]->SetFillColor  (0);               
    gr_ped_ring_minus[iring]->SetMarkerSize  (1);               
    gr_ped_ring_minus[iring]->SetMarkerStyle (20);              
    gr_ped_ring_minus[iring]->SetMarkerColor (GetColorFromPalette ( iring, 127, 0) );    
    gr_ped_ring_minus[iring]->SetLineWidth (1);                 
    gr_ped_ring_minus[iring]->SetLineColor (GetColorFromPalette ( iring, 127, 0) );    
    //---- style (end) ----
    
    
  }
  
  
  TMultiGraph* mgr = new TMultiGraph();
  for (int iring = 0; iring < 127; iring++) {
    
    //---- plot only 1 every 10
    if (iring % 10) continue;
    
    mgr->Add(gr_ped_ring_plus[iring]);
    mgr->Add(gr_ped_ring_minus[iring]);
    
  }
  
  
  mgr->Draw("APL");
  mgr->GetYaxis()->SetTitle("ped ADC");
  mgr->GetXaxis()->SetTitle("time");
  mgr->GetXaxis()->SetTimeDisplay(1);
  mgr->GetXaxis()->SetNdivisions(-503);
  mgr->GetXaxis()->SetTimeFormat("%Y-%m-%d %H:%M");
  mgr->GetXaxis()->SetTimeOffset(0,"gmt");
  ccPed->SetGrid();
  
  ccPed->BuildLegend();
  
  
  
  TString namePlot = Form ("plot/ped_xyz_all_%d.png",runNumber);
  ccPed->SaveAs(namePlot.Data());
  
  
  
  
  
  //---- check rings 
  TCanvas* ccRingEE = new TCanvas ("ccRingEE","",1200,600);
  
  TH2F* histo_EE = new TH2F ("histo_EE", "rings" ,  200, 0.5, 200.5,  100, 0.5, 100.5);
  
  for (int index = 0; index< 75848; index++) {
    PedChan->GetEntry(index);
    
    if (index < 61200) continue;
//     if (z == 0) continue; //---- skip the EB

    int iring = 0;
    
    //---- EE
    float dx = x - 50.5;
    float dy = y - 50.5;
    
    float ring = sqrt( dx*dx + dy*dy );
    
    //       42 -  iter
    iring = 41 - ( round(ring) - 11)  + 85;  //---- 12 [ = (62 - 50 - 1) from the 2D plot] is the first ring
    
    
    histo_EE->Fill(x + 100*(z>0), y, iring);
    
  }
  
  histo_EE->Draw("colz text");
  
  
  
  
  
  
  
  TCanvas* ccPedEE = new TCanvas ("ccPedEE","",1600,600);
  
  TMultiGraph* mgr_EE = new TMultiGraph();
  for (int iring = 86; iring < 127; iring++) {
    
    mgr_EE->Add(gr_ped_ring_plus[iring]);
    mgr_EE->Add(gr_ped_ring_minus[iring]);
    
  }
  
  mgr_EE->Draw("APL");
  mgr_EE->GetYaxis()->SetTitle("ped ADC");
  mgr_EE->GetXaxis()->SetTitle("time");
  mgr_EE->GetXaxis()->SetTimeDisplay(1);
  mgr_EE->GetXaxis()->SetNdivisions(-503);
  mgr_EE->GetXaxis()->SetTimeFormat("%Y-%m-%d %H:%M");
  mgr_EE->GetXaxis()->SetTimeOffset(0,"gmt");
  ccPedEE->SetGrid();
  
  ccPedEE->BuildLegend();
  
  
  
  
  
  
  TCanvas* ccPedEB = new TCanvas ("ccPedEB","",1600,600);
  
  TMultiGraph* mgr_EB = new TMultiGraph();
  for (int iring = 0; iring < 86; iring++) {
    
    mgr_EB->Add(gr_ped_ring_plus[iring]);
    mgr_EB->Add(gr_ped_ring_minus[iring]);
    
  }
  
  mgr_EB->Draw("APL");
  mgr_EB->GetYaxis()->SetTitle("ped ADC");
  mgr_EB->GetXaxis()->SetTitle("time");
  mgr_EB->GetXaxis()->SetTimeDisplay(1);
  mgr_EB->GetXaxis()->SetNdivisions(-503);
  mgr_EB->GetXaxis()->SetTimeFormat("%Y-%m-%d %H:%M");
  mgr_EB->GetXaxis()->SetTimeOffset(0,"gmt");
  ccPedEB->SetGrid();
  
  ccPedEB->BuildLegend();
  
  
  
}




