Linux Udev configuration
---

The user can run this ioc are in dialout group, if one wants to use other group, one should create it, and change rules according to.

* Please Run the following commands:

```
$ sudo scp 56-usbtmc.rules /etc/udev/rules.d/
$ sudo udevadm control --reload
$ sudo udevadm trigger
```

* When the device is connected to a PC, the following device will be shown :
```
crw-rw---- 1 root dialout 180, 1 Mar  8 10:06 /dev/usbtmc1
```

* One can test its access through 
```
$ echo "*IDN?" > /dev/usbtmc1 
$ cat /dev/usbtmc1 
*IDN WST,WaveStation 3122,LCRY3601C00251,5.01.02.13,00-00-00-17-35
```

* One can access the usbtmc1 information as follows:

```
# udevadm info --name=/dev/usbtmc1 --attribute-walk

Udevadm info starts with the device specified by the devpath and then
walks up the chain of parent devices. It prints for every device
found, all possible attributes in the udev rules key format.
A rule to match, can be composed by the attributes of the device
and the attributes from one single parent device.

  looking at device '/devices/pci0000:00/0000:00:14.0/usb1/1-1/1-1:1.0/usbmisc/usbtmc1':
    KERNEL=="usbtmc1"
    SUBSYSTEM=="usbmisc"
    DRIVER==""

  looking at parent device '/devices/pci0000:00/0000:00:14.0/usb1/1-1/1-1:1.0':
    KERNELS=="1-1:1.0"
    SUBSYSTEMS=="usb"
    DRIVERS=="usbtmc"
    ATTRS{TermChar}==""
    ATTRS{interface_capabilities}=="0"
    ATTRS{bInterfaceClass}=="fe"
    ATTRS{device_capabilities}=="0"
    ATTRS{auto_abort}=="0"
    ATTRS{bInterfaceSubClass}=="03"
    ATTRS{bInterfaceProtocol}=="01"
    ATTRS{bNumEndpoints}=="03"

    ATTRS{supports_autosuspend}=="0"
    ATTRS{usb488_interface_capabilities}=="7"
    ATTRS{bAlternateSetting}==" 0"
    ATTRS{usb488_device_capabilities}=="15"
    ATTRS{bInterfaceNumber}=="00"
    ATTRS{TermCharEnabled}=="0"

  looking at parent device '/devices/pci0000:00/0000:00:14.0/usb1/1-1':
    KERNELS=="1-1"
    SUBSYSTEMS=="usb"
    DRIVERS=="usb"
    ATTRS{bDeviceSubClass}=="00"
    ATTRS{bDeviceProtocol}=="00"
    ATTRS{devpath}=="1"
    ATTRS{idVendor}=="05ff"
    ATTRS{speed}=="12"
    ATTRS{bNumInterfaces}==" 1"
    ATTRS{bConfigurationValue}=="1"
    ATTRS{bMaxPacketSize0}=="64"
    ATTRS{busnum}=="1"
    ATTRS{devnum}=="120"
    ATTRS{configuration}==""
    ATTRS{bMaxPower}=="2mA"
    ATTRS{authorized}=="1"
    ATTRS{bmAttributes}=="c0"
    ATTRS{bNumConfigurations}=="1"
    ATTRS{maxchild}=="0"
    ATTRS{bcdDevice}=="0200"
    ATTRS{avoid_reset_quirk}=="0"
    ATTRS{quirks}=="0x0"
    ATTRS{serial}=="LCRY3601C00251"
    ATTRS{version}==" 2.00"
    ATTRS{urbnum}=="11"
        ATTRS{ltm_capable}=="no"
    ATTRS{manufacturer}=="Teledyne LeCroy"
    ATTRS{removable}=="unknown"
    ATTRS{idProduct}=="0a21"
    ATTRS{bDeviceClass}=="00"
    ATTRS{product}=="WaveStation 3122"

  looking at parent device '/devices/pci0000:00/0000:00:14.0/usb1':
    KERNELS=="usb1"
    SUBSYSTEMS=="usb"
    DRIVERS=="usb"
    ATTRS{bDeviceSubClass}=="00"
    ATTRS{bDeviceProtocol}=="01"
    ATTRS{devpath}=="0"
    ATTRS{idVendor}=="1d6b"
    ATTRS{speed}=="480"
    ATTRS{bNumInterfaces}==" 1"
    ATTRS{bConfigurationValue}=="1"
    ATTRS{bMaxPacketSize0}=="64"
    ATTRS{authorized_default}=="1"
    ATTRS{busnum}=="1"
    ATTRS{devnum}=="1"
    ATTRS{configuration}==""
    ATTRS{bMaxPower}=="0mA"
    ATTRS{authorized}=="1"
    ATTRS{bmAttributes}=="e0"
    ATTRS{bNumConfigurations}=="1"
    ATTRS{maxchild}=="16"
    ATTRS{bcdDevice}=="0316"
    ATTRS{avoid_reset_quirk}=="0"
    ATTRS{quirks}=="0x0"
    ATTRS{serial}=="0000:00:14.0"
    ATTRS{version}==" 2.00"
    ATTRS{urbnum}=="2540"
    ATTRS{ltm_capable}=="no"
    
   ATTRS{manufacturer}=="Linux 3.16.0-5-amd64 xhci_hcd"
    ATTRS{removable}=="unknown"
    ATTRS{idProduct}=="0002"
    ATTRS{bDeviceClass}=="09"
    ATTRS{product}=="xHCI Host Controller"

  looking at parent device '/devices/pci0000:00/0000:00:14.0':
    KERNELS=="0000:00:14.0"
    SUBSYSTEMS=="pci"
    DRIVERS=="xhci_hcd"
    ATTRS{irq}=="140"
    ATTRS{subsystem_vendor}=="0x1028"
    ATTRS{broken_parity_status}=="0"
    ATTRS{class}=="0x0c0330"
    ATTRS{driver_override}=="(null)"
    ATTRS{consistent_dma_mask_bits}=="64"
    ATTRS{dma_mask_bits}=="64"
    ATTRS{local_cpus}=="00000000,00000000,00000000,00000000,00000000,00000000,00000000,00000000,00000000,00000000,00000000,00000000,00000000,00000000,00000000,000000ff"
    ATTRS{device}=="0xa12f"
    ATTRS{enable}=="1"
    ATTRS{msi_bus}==""
    ATTRS{local_cpulist}=="0-7"
    ATTRS{vendor}=="0x8086"
    ATTRS{subsystem_device}=="0x06b8"
    ATTRS{numa_node}=="-1"
    ATTRS{d3cold_allowed}=="1"

  looking at parent device '/devices/pci0000:00':
    KERNELS=="pci0000:00"
    SUBSYSTEMS==""
    DRIVERS==""
```


* Start IOC, and device will be disappeared
