TOP=../..

include $(TOP)/configure/CONFIG


USR_CPPFLAGS += -DUSE_TYPED_RSET
#USR_CXXFLAGS += -fpermissive

PROD_IOC = waveStation3122

DBD += waveStation3122.dbd

waveStation3122_DBD += base.dbd
waveStation3122_DBD += stream.dbd
waveStation3122_DBD += asyn.dbd
waveStation3122_DBD += drvAsynUSBTMC.dbd
waveStation3122_DBD += devWaveStation3112.dbd

waveStation3122_LIBS += stream asyn

waveStation3122_SRCS += devWaveStation3112.c
waveStation3122_SRCS += StringUtils.cpp
#waveStation3122_SRCS += DataIO.cpp

waveStation3122_SRCS += waveStation3122_registerRecordDeviceDriver.cpp

waveStation3122_SRCS += waveStation3122Main.cpp

waveStation3122_LIBS += $(EPICS_BASE_IOC_LIBS)

include $(TOP)/configure/RULES



