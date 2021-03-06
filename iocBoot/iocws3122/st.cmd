#!../../bin/linux-x86_64/ws3122App

< envPaths

#epicsEnvSet("EPICS_CAS_INTF_ADDR_LIST" "10.0.7.1")

epicsEnvSet("STREAM_PROTOCOL_PATH", ".:${TOP}/db")
epicsEnvSet(P, "usbtmc")
epicsEnvSet(R, "icslab")
epicsEnvSet(PORT, "usbtmc1")

cd "${TOP}"

dbLoadDatabase "dbd/ws3122.dbd"
ws3122_registerRecordDeviceDriver pdbbase


# Bus 001 Device 084: ID 05ff:0a21 LeCroy Corp.
epicsEnvSet(vendorNum, "05ff")
epicsEnvSet(productNum, "0a21")


# usbtmcConfigure(port, vendorNum, productNum, serialNumberStr, priority, flags)
usbtmcConfigure("$(PORT)", "0x$(vendorNum)", "0x$(productNum)")
dbLoadRecords("db/asynRecord.db","P=$(P),R=$(R),PORT=$(PORT),ADDR=0,OMAX=100,IMAX=100")
#dbLoadRecords("${TOP}/db/wavestation3122.db", "P=$(R):, PORT=$(PORT)")
#dbLoadRecords("${TOP}/db/ws3122.db", "P=$(P):, R=$(R):, ADDR=0, PORT=$(PORT), TIMEOUT=0")

cd "${TOP}/iocBoot/${IOC}"

iocInit

dbl > "${TOP}/${IOC}_PVs.list"

< asyn_usbtmc.cmd
