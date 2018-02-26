Linux Udev configuration

Put 55-usbtmc.rules in /etc/udev/rules.d/ and Run sudo /bin/udevadm trigger

$ sudo scp 55-usbtmc.rules /etc/udev/rules.d/
$ sudo /bin/udevadm trigger


One can access the usbtmc1 information as follows:

```
$ sudo   udevadm info --name=/dev/usbtmc1 

P: /devices/pci0000:00/0000:00:14.0/usb1/1-1/1-1:1.0/usbmisc/usbtmc1
N: usbtmc1
E: DEVNAME=/dev/usbtmc1
E: DEVPATH=/devices/pci0000:00/0000:00:14.0/usb1/1-1/1-1:1.0/usbmisc/usbtmc1
E: MAJOR=180
E: MINOR=1
E: SUBSYSTEM=usbmisc
```
Or more info:
```
$ sudo udevadm info --name=/dev/usbtmc1 --attribute-walk
```