require require, 2.5.4
require stream, 2.7.7


epicsEnvSet("STREAM_PROTOCOL_PATH", "/home/jhlee/gitsrc/m-epics-wavestation-3000")

#usbtmcConfigure(const char *portName,
#                int vendorId, int productId, const char *serialNumber,
#                int priority, int flags)


epicsEnvSet(P, "$(USBTMC_P=usbtmc:)")
epicsEnvSet(R, "$(USBTMC_R=asyn)")

# Bus 002 Device 015: ID 05ff:0a21 LeCroy Corp.

# Configure hardware
usbtmcConfigure("/dev/usbtmc1", 0x05ff, 0x0a21)
# asynSetTraceIOMask("/dev/usbtmc1",0,0x2)
# asynSetTraceMask("/dev/usbtmc1",0,0x03)

###############################################################################
# Load record instances
#dbLoadRecords("asynRecord.db","P=$(P),R=$(R),PORT=/dev/usbtmc1,ADDR=0,OMAX=100,IMAX=100")


epicsEnvSet("IPPORT", "002")
epicsEnvSet("SCAN", "10");


# Load the database defining the EPICS records
dbLoadRecords(wavestation.db, "P=$(P), PORT=/dev/usbtmc1, ADDR=$(IPPORT), SCAN=$(SCAN)")
