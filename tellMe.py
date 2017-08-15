import ROOT



from optparse import OptionParser
parser = OptionParser()
parser.add_option("--ieta",    dest="ieta",    default = -99, help = "ieta",    type = 'int')
parser.add_option("--iphi",    dest="iphi",    default = -99, help = "iphi",    type = 'int')
parser.add_option("--ix",      dest="ix",      default = -99, help = "ix",      type = 'int')
parser.add_option("--iy",      dest="iy",      default = -99, help = "iy",      type = 'int')
parser.add_option("--iz",      dest="iz",      default = -99, help = "iz",      type = 'int')
parser.add_option("--index",   dest="index",   default = -99, help = "index",   type = 'int')

(options, args) = parser.parse_args()



# cmsswId      dbID      hashedId     iphi      ieta   abs(ieta)    pos       FED        SM      TT(25)     iTT     strip(5)    Xtal     phiSM     etaSM
#    0          1           2           3        4        5          6         7          8        9         10        11         12        13        14

map_EB = {}

for line in open("EB.txt"):
    li = line.strip()
    splitLine = line.split()
    if not li.startswith("#"):
        map_EB [ ( int(splitLine[4]) , int(splitLine[3]) ) ] = ( int(splitLine[2]),    # 0     hashedId
                                                                 int(splitLine[0]),    # 1     dbID
                                                                 #int(splitLine[6]),   # 
                                                                 int(splitLine[7]),    # 2     FED     
                                                                 int(splitLine[8]),    # 3     SM
                                                                 int(splitLine[9]),    # 4     TT
                                                                 int(splitLine[10]),   # 5     iTT
                                                                 int(splitLine[11]),   # 6     strip
                                                                 int(splitLine[12]),   # 7     Xtal
                                                                 int(splitLine[13]),   # 8     phiSM
                                                                 int(splitLine[14]),   # 9     etaSM
                                                                 )
        



# cmsswId               dbID      hashedId      side    ix      iy      SC      iSC     Fed     EE      TT/CCU  strip   Xtal    quadrant
#    0                  1           2           3        4      5       6        7        8      9        10      11     12         13
#  872420481     2010041001          0         -1       41      1       175     25      608     EE-05     13      1       1         3

map_EE = {}

for line in open("EE.txt"):
    li = line.strip()
    splitLine = line.split()
    if not li.startswith("#"):
        map_EE [ ( int(splitLine[4]) , int(splitLine[5]),  int(splitLine[3])) ] = ( 
                                                                 int(splitLine[2]),    # 0     hashedId
                                                                 int(splitLine[0]),    # 1     cmsswId
                                                                 int(splitLine[3]),    # 2     side     
                                                                 int(splitLine[6]),    # 3     SC
                                                                 int(splitLine[7]),    # 4     iSC
                                                                 int(splitLine[8]),    # 5     Fed
                                                                 #int(splitLine[9]),    #      EE
                                                                 int(splitLine[10]),   # 6     TT/CCU         
                                                                 int(splitLine[11]),   # 7     strip
                                                                 int(splitLine[12]),   # 8     Xtal
                                                                 int(splitLine[13]),   # 9     quadrant
                                                                 )
 


if options.iz == -1 or options.iz == 1 :
  print " EE " 
  
  print " fed = ", map_EE [ ( options.ix, options.iy, options.iz ) ] [5]
  
  
  
  
  
  
  
  
  

