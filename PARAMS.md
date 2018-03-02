# WS Parameter List

##
* [A] : WS3122Base.template
* [B] : drvWS3122.hh
* [C] : drvWS3122.cpp

| A   | A,B | B  | B | 
| --- | --- | --- | --- |
| DB Record | DB Record INP Field | #define | type string_name| 
| `DevManufacturer_RBV`		| DEV_MANUFACTURER |		WS3122ManufacturerString	(asynOctet,r/o) | int  WS3122Manufacturer		| 
| `DevModel_RBV`		| DEV_MODEL |	   		WS3122ModelString	 (asynOctet,r/o) | int  WS3122Model			|      
| `DevSerialNumber_RBV`		| DEV_SERIAL_NUMBER |		WS3122SerialNumberString  (asynOctet,r/o) | int  WS3122SerialNumber 	    	|
| `DevFirmwareVersion_RBV`	| DEV_FIRMWARE_VERSION |	WS3122FirmwareVersionString	(asynOctet,r/o) | int  WS3122FirmwareVersion    	|
| `DevGetIDN`                   | DEV_GET_IDN                 | DevIDNString | int Identification |



```
class, protected

createParam(DevIDNString,                    asynParamInt32, &Identification);
createParam(WS3122ManufacturerString,        asynParamOctet, &WS3122Manufacturer);
createParam(WS3122ModelString,               asynParamOctet, &WS3122Model);
createParam(WS3122SerialNumberString,        asynParamOctet, &WS3122SerialNumber);
createParam(WS3122FirmwareVersionString,     asynParamOctet, &WS3122FirmwareVersion);

```					     
