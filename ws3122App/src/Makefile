TOP=../..

include $(TOP)/configure/CONFIG

LIBRARY_IOC += ws3122

DBD += ws3122.dbd
DBD += ws3122Support.dbd


ws3122_SRC += WS3122.cpp
ws3122_SRC += StringUtils.cpp

ws3122_LIBS += asyn

ws3122_LIBS += $(EPICS_BASE_IOC_LIBS)


PROD_IOC += ws3122App
ws3122App_SRCS += ws3122_registerRecordDeviceDriver.cpp
ws3122App_SRCS += ws3122Main.cpp

ws3122App_LIBS += asyn
ws3122App_LIBS += $(EPICS_BASE_IOC_LIBS)


include $(TOP)/configure/RULES



