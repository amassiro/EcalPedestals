# EcalPedestals

Code to plot pedestals extracted from laser sequence

    T->Draw("ped[1] : time[2]", "run==283270 && ped[1]>50", "box")
    PedChan->Scan("iChannel:ix:iy:iz") 

    
    
    r00t draw.cxx\(\"ana_ped_2016-2017.root\",0,283270\)
    
    
    
    python tellMe.py --iz 0 --ix -85 --iy 3
    
    python tellMe.py --iz 1 --ix 2 --iy 41
    
    python tellMe.py --iz -1 --ix 2 --iy 41
    
    
    