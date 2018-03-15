EPICS IOC for WaveStation 3122
---
[![Build Status](https://travis-ci.org/jeonghanlee/lecroy_ws3122.svg?branch=master)](https://travis-ci.org/jeonghanlee/lecroy_ws3122)


# Limitations

* No Status reading from WaveStation 3122, the current implementation is *write* commands to the device.


# Support Waves

* BSWV (SINE, SQUARE, RAMP, PULSE, NOISE, ARB, DC) : all parameters can be defined, however, according to the wave type, invalid parameters will be disabled automatically. The all parameters should be defined first, before send the command to the wave station. If one would like to use, please look command orders at cmds file.

* BTWV (work in progress)


# Warning



# Requirements

* Asyn should be compiled with DRV_USBTMC=YES in configure/CONFIG_SITE
* RELEASE.local
```
EPICS_BASE=/home/jhlee/epics_env/epics-base
ASYN=${EPICS_MODULES}/asyn
STREAM=${EPICS_MODULES}/stream
```

# Device Configuration and Information
## Settings
* USBTMC :  Utility > Interface > USB setup > USBTMC > Done
* 50Ω & Normal Polarity : Utility > Output >  50Ω > Polarity Normal > Done
* Point , & Separator On : Utility > System > Number > Point , > Separator On > Done
* Power On Setting : Utility > System > Power On : Last > More > Done
## Information
* Software version : 5.01.02.13
* Hardware version : 00-00-00-17-36
* Product Type : WaveStation 3122

## lsusb
```
$ lsusb 
Bus 001 Device 072: ID 05ff:0a21 LeCroy Corp.
```

## UDEV

[UDEV configuration](tools/README.md)


# References
* https://epics.anl.gov/modules/soft/asyn/R4-33/asynDriver.html#testUsbtmcApp

