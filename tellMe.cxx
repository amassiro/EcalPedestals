void tellMe(){

  Int_t           Channels;
  Int_t           x;
  Int_t           y;
  Int_t           z;

  fChain->SetBranchAddress("Channels", &Channels, &b_iChannel);
  fChain->SetBranchAddress("x", &x, &b_ix);
  fChain->SetBranchAddress("y", &y, &b_iy);
  fChain->SetBranchAddress("z", &z, &b_iz);

  
  
}


