#-save other things every thirty seconds
#- create_monitor_set("auto_settings.req", 30, "P=as:")

#- epicsEnvSet("AUTO_SETTINGS", "auto_settings.req")
#- autosaveBuild("$(AUTO_SETTINGS)", "_settings.req", 1)


makeAutosaveFileFromDbInfo("$(TOP)/as/req/auto_settings.req", "autosaveFields_pass0")
create_monitor_set("auto_settings.req", 30, "P=as:")
