EPICS IOC for WaveStation 3122
---
[![Build Status](https://travis-ci.org/icshwi/WaveStation3122.svg?branch=master)](https://travis-ci.org/icshwi/WaveStation3122)


# Warning

This repository doesn't hold any WaveStation 3122 support. That means no DB and no protocol file.

# Requirements

* Asyn should be compiled with DRV_USBTMC=YES in configure/CONFIG_SITE
* RELEASE.local
```
EPICS_BASE=/home/jhlee/epics_env/epics-base
ASYN=${EPICS_MODULES}/asyn
STREAM=${EPICS_MODULES}/stream
```



https://epics.anl.gov/modules/soft/asyn/R4-33/asynDriver.html#testUsbtmcApp
