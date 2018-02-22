#!../../bin/linux-x86_64/waveStation3122


< envPaths

cd "${TOP}"

epicsEnvSet("vendorID" 

## Register all support components
dbLoadDatabase "dbd/waveStation3122.dbd"
waveStation3122_registerRecordDeviceDriver pdbbase


# usbtmcConfigure("asynPort", vendorId, productId, "serialNumber", priority, flags)
usbtmcConfigure("usbtmc1", 0x05ff, 0x0a21)
asynSetTraceIOMask("usbtmc1",0,0x2)
asynSetTraceMask("usbtmc1",0,0x03)


###############################################################################
# Load record instances
dbLoadRecords("db/asynRecord.db","P=$(P),R=$(R),PORT=usbtmc1,ADDR=0,OMAX=100,IMAX=100")


cd "${TOP}/iocBoot/${IOC}"
iocInit

epicsThreadSleep 2
asynReport 2

dbpf "$(P)$(R).OFMT" "ASCII"
dbpf "$(P)$(R).IFMT" "Hybrid"
dbpf "$(P)$(R).TMOD" "Write/Read"

dbpf "$(P)$(R).AOUT" "*IDN?"
epicsThreadSleep 2

echo In another window run caget -S $(P)$(R).BINP and confirm that the IDN string was read.

dbpr "$(P)$(R)" 1

