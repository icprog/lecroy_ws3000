 Linux udev configuration

If attempts to communicate with a device result in error messages of the form "Access denied (insufficient permissions)" you must add udev entries to allow access. In the /etc/udev/rules.d directory create a file, 55-usbtmc.rules for example, with a line for each device of interest.

To allow everyone in the "usbtmc" group access to a device with vendor code 1BFA (hexadecimal) and product code 0498 (hexadecimal) the line would look like:

SUBSYSTEM=="usb", ATTRS{idVendor}=="1BFA", ATTRS{idProduct}=="0498", GROUP="usbtmc", MODE="0660"

To allow everyone access the line would look like:

SUBSYSTEM=="usb", ATTRS{idVendor}=="1BFA", ATTRS{idProduct}=="0498", MODE="0666"

The {idVendor} and {idProduct} values must match those of the device with which you wish to communicate.
