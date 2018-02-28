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

# Strange behaviors
* a user cannot access the /dev/usbtmc1 even if the proper udev rules are in /etc/udev/rules.d
* sudo ./st.cmd trigger the following events
```
$ sudo udevadm monitor
...
KERNEL[3639135.343536] remove   /devices/pci0000:00/0000:00:14.0/usb1/1-2/1-2:1.0/usbmisc/usbtmc1 (usbmisc)
UDEV  [3639135.343899] remove   /devices/pci0000:00/0000:00:14.0/usb1/1-2/1-2:1.0/usbmisc/usbtmc1 (usbmisc)

```
And the following device will be disappreared.
```
ls: cannot access /dev/usbtmc1: No such file or directory
```

However, IOC is running, can access through epics IOC. 

# References
* https://epics.anl.gov/modules/soft/asyn/R4-33/asynDriver.html#testUsbtmcApp
* StringUtils.{cpp,h} from https://github.com/keenanlang/epics-usb
