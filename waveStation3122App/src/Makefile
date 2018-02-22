TOP=../..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE
#=============================

#=============================
# Build the IOC application

PROD_IOC = waveStation3122
# waveStation3122.dbd will be created and installed
DBD += waveStation3122.dbd

# waveStation3122.dbd will be made up from these files:
waveStation3122_DBD += base.dbd

# Include dbd files from all support applications:
waveStation3122_DBD += stream.dbd
waveStation3122_DBD += drvAsynUSBTMC.dbd
waveStation3122_DBD += asyn.dbd

waveStation3122_LIBS += stream asyn

# waveStation3122_registerRecordDeviceDriver.cpp derives from waveStation3122.dbd
waveStation3122_SRCS += waveStation3122_registerRecordDeviceDriver.cpp

# Build the main IOC entry point on workstation OSs.
waveStation3122_SRCS_DEFAULT += waveStation3122Main.cpp
waveStation3122_SRCS_vxWorks += -nil-

# Add support from base/src/vxWorks if needed
#waveStation3122_OBJS_vxWorks += $(EPICS_BASE_BIN)/vxComLibrary

# Finally link to the EPICS Base libraries
waveStation3122_LIBS += $(EPICS_BASE_IOC_LIBS)

#===========================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

