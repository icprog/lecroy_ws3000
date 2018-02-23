require require, 2.5.4
require stream, 2.7.7


epicsEnvSet("STREAM_PROTOCOL_PATH", "/home/jhlee/gitsrc/m-epics-wavestation-3000")

#usbtmcConfigure(const char *portName,
#                int vendorId, int productId, const char *serialNumber,
#                int priority, int flags)


epicsEnvSet(P, "$(USBTMC_P=usbtmc:)")
epicsEnvSet(R, "$(USBTMC_R=asyn)")
epicsEnvSet(PORT, "asyn-$(P)$(R)")

# Bus 002 Device 015: ID 05ff:0a21 LeCroy Corp.

# Configure hardware
usbtmcConfigure("$(PORT)", 0x05ff, 0x0a21)
# asynSetTraceIOMask("$(PORT)",0,0x2)
# asynSetTraceMask("$(PORT)",0,0x03)

###############################################################################
# Load record instances
#dbLoadRecords("asynRecord.db","P=$(P),R=$(R),PORT=$(PORT),ADDR=0,OMAX=100,IMAX=100")


# Load the database defining the EPICS records
dbLoadRecords(wavestation3122.db, "P=$(P), R=$(R), PORT=$(PORT)")
