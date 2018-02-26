#!../../bin/linux-x86_64/waveStation3122


< envPaths

#epicsEnvSet("EPICS_CAS_INTF_ADDR_LIST" "10.0.7.1")

epicsEnvSet("STREAM_PROTOCOL_PATH", ".:${TOP}/db")
epicsEnvSet(P, "usbtmc:")
epicsEnvSet(R, "icslab")
epicsEnvSet(PORT, "usbtmc1")

cd "${TOP}"

dbLoadDatabase "dbd/waveStation3122.dbd"
waveStation3122_registerRecordDeviceDriver pdbbase


# Bus 001 Device 084: ID 05ff:0a21 LeCroy Corp.
epicsEnvSet(vendorNum, "05ff")
epicsEnvSet(productNum, "0a21")


# usbtmcConfigure(port, vendorNum, productNum, serialNumberStr, priority, flags)
usbtmcConfigure("$(PORT)", "0x$(vendorNum)", "0x$(productNum)")
#asynSetTraceIOMask("usbtmc1",0,0x2)
#asynSetTraceMask("usbtmc1",0,0x03)


dbLoadRecords("db/asynRecord.db","P=$(P),R=$(R),PORT=$(PORT),ADDR=0,OMAX=100,IMAX=100")

#dbLoadRecords("${TOP}/db/wavestation3122.db", "P=$(P), R=$(R), PORT=$(PORT)"

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

