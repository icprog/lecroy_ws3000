TOP=../..
include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE


#DB += ws3122.db
DB += WS3122Base.db
DB += BasicWave.db
DB += BurstWave.db
DB += WS3122Cmds.db


DB_INSTALLS += $(ASYN)/db/asynRecord.db


ifneq ($(DEVIOCSTATS),)
DB_INSTALLS += $(DEVIOCSTATS)/db/iocAdminSoft.db
endif

ifneq ($(AUTOSAVE),)
DB_INSTALLS += $(AUTOSAVE)/db/save_restoreStatus.db
endif


include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

