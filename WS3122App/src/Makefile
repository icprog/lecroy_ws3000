TOP=../..

include $(TOP)/configure/CONFIG

PROD_NAME=WS3122

SUPPORT_NAME=$(PROD_NAME)Support

LIBRARY_IOC += $(SUPPORT_NAME)

$(SUPPORT_NAME)_SRCS += drvWS3122.cpp
$(SUPPORT_NAME)_SRCS += WSTypes.cpp
$(SUPPORT_NAME)_SRCS += WS3122Def.cpp
$(SUPPORT_NAME)_LIBS += $(EPICS_BASE_IOC_LIBS)



PROD_IOC = $(PROD_NAME)

# $(PROD_NAME).dbd will be created and installed
DBD += $(PROD_NAME).dbd

# $(PROD_NAME).dbd files from all support applications:

$(PROD_NAME)_DBD += base.dbd
$(PROD_NAME)_DBD += asyn.dbd
$(PROD_NAME)_DBD += drvAsynUSBTMC.dbd
$(PROD_NAME)_DBD += drvWS3122.dbd

$(PROD_NAME)_SRCS += $(PROD_NAME)_registerRecordDeviceDriver.cpp

$(PROD_NAME)_SRCS_DEFAULT += $(PROD_NAME)Main.cpp
$(PROD_NAME)_SRCS_vxWorks += -nil-


$(PROD_NAME)_LIBS += asyn

$(PROD_NAME)_LIBS += $(SUPPORT_NAME)
$(PROD_NAME)_LIBS += $(EPICS_BASE_IOC_LIBS)


ifneq ($(DEVIOCSTATS),)
$(PROD_NAME)_DBD  += devIocStats.dbd
$(PROD_NAME)_LIBS += devIocStats
endif

ifneq ($(AUTOSAVE),)
$(PROD_NAME)_DBD  += asSupport.dbd
$(PROD_NAME)_LIBS += autosave
endif



include $(TOP)/configure/RULES



