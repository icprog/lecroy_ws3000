
#- save_restore setup
#- status-PV prefix
save_restoreSet_status_prefix("as:")
#- Debug-output level
save_restoreSet_Debug(0)

#- Ok to save/restore save sets with missing values (no CA connection to PV)?
save_restoreSet_IncompleteSetsOk(1)

#- specify where save files should be
set_savefile_path    ("$(TOP)/as", "/save")
set_requestfile_path ("$(TOP)/as", "/req")

set_pass0_restoreFile("auto_settings.sav")
set_pass1_restoreFile("auto_settings.sav")

dbLoadRecords("$(TOP)/db/save_restoreStatus.db", "P=as:")
