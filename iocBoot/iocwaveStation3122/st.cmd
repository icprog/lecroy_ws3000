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

dbLoadRecords("${TOP}/db/wavestation3122.db", "P=$(R):$(IOC), PORT=$(PORT)")

cd "${TOP}/iocBoot/${IOC}"
iocInit

dbl > "${TOP}/${IOC}_PVs.list"
