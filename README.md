# EcalPedestals

Code to plot pedestals extracted from laser sequence

    T->Draw("ped[1] : time[2]", "run==283270 && ped[1]>50", "box")
    PedChan->Scan("iChannel:ix:iy:iz") 

    
    
    r00t draw.cxx\(\"ana_ped_2016-2017.root\",0,283270\)
    
    
    
    python tellMe.py --iz 0 --ix -85 --iy 3
    
    python tellMe.py --iz 1 --ix 2 --iy 41
    
    python tellMe.py --iz -1 --ix 2 --iy 41
    
    
ROOT Tree content

    T rootuple content: 
    
    id --> file number 
    run --> run number 
    run_type --> 1=collisions, 2=cosmics, 3=circulating, 4=test, 0=other not classified 
    seq_id --> within a run is the calibration sequence number (the sweep of the laser over the FEDs, starting from 0 ...)
    las_id --> 447 blue laser, 592 green 
    fill_num --> if collision run then it is the LHC fill number
    run_num_infill -> if in a fill there are several runs then it is the run number 0,1,2,...
    run_time --> UTC start of the run time 
    run_time_stablebeam --> UTC stable beam run time for that fill (note that even for collision runs, it could be that in a run for a few sequences there are no collisions, so if you want to look only at collision runs where there are collisions, you have to make sure that the time of your data (see time vector below) are after  run_time_stablebeam).
    lumi is the initial instantaneous lumi at the start of the fill 
    bfield is the magnetic field 
    nxt is the number of crystals in the sequence (75848 if the sequence is complete)
    time[0,...,54] is the time when the data were taken on the specific FED, the index is #FED-601, for example time[0] is for FED=601.... 
    
    now come the pedestals: 
    fed[] is the fed number 
    chan[] is the channel number 
    ped[] is the pedestal 
    pedrms[] is the pedestal rms 

