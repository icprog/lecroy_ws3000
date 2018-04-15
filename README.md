Naive EPICS IOC for WaveStation 3122
---

[![Build Status](https://travis-ci.org/jeonghanlee/lecroy_ws3000.svg?branch=master)](https://travis-ci.org/jeonghanlee/lecroy_ws3000)


# Limitations

* No Status reading from WaveStation 3122, the current implementation is *write* commands to the device.


# Support Waves

* BSWV (SINE, SQUARE, RAMP, PULSE, NOISE, ARB, DC) : all parameters can be defined, however, according to the wave type, invalid parameters will be disabled automatically. The all parameters should be defined first, before send the command to the wave station. If one would like to use, please look command orders at cmds file.

* BTWV : Due to the vendor SCPI command limitation, we cannot set the entire parameters (BTWW and its carrier Waveform). We have to define the Carrier Waveform parameters first, then we have to define BTWV parameters. 


# Warning



# Requirements

* Asyn should be compiled with DRV_USBTMC=YES, which can be enable in (ASYN)/configure/CONFIG_SITE (libusb-1.0 or libusbx is needed according to the Linux distribution)

* Add RELEASE.local as follows:
```
EPICS_BASE=/home/jhlee/epics_env/epics-base
ASYN=${EPICS_MODULES}/asyn
```

* Optional Modules : (iocStats, autosave)

# Optional 
DEVIOCSTATS=${EPICS_MODULES}/iocStats
AUTOSAVE=${EPICS_MODULES}/autosave

* UI is for latest CS Studio with Display Builder (not BOY) in WS3122App/op


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

## UDEV configuraiton for WS3122 

[UDEV configuration](tools/README.md)


# References
* https://epics.anl.gov/modules/soft/asyn/R4-33/asynDriver.html#testUsbtmcApp

