
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <math.h>

#include <epicsTypes.h>
#include <epicsTime.h>
#include <epicsThread.h>
#include <epicsString.h>
#include <epicsTimer.h>
#include <epicsMutex.h>
#include <epicsEvent.h>
#include <iocsh.h>


#include <epicsExport.h>
#include <asynDriver.h>

#define epicsExportSharedSymbols
#include <shareLib.h>

#include "WS3122Def.hh"
#include "drvWS3122.hh"


static const char *driverName="drvWS3122";

asynStatus
drvWS3122::CmdsSendRead(asynUser *pasynUser, epicsInt32 value)
{
  asynStatus rtn = asynSuccess;
  char cmndBuf[64];
  char dataBuf[256];
  int  dataBufSize;


  memset(cmndBuf, 0x00, sizeof(cmndBuf));
  memset(dataBuf, 0x00, sizeof(dataBuf));
  
  pasynOctetSyncIO->setOutputEos(this->pasynUserDriver, "\n", 1);
  pasynOctetSyncIO->setInputEos(this->pasynUserDriver, "", 0);
  sprintf(cmndBuf, "*IDN?");
   
  if(this->usbTMCPortWriteRead(cmndBuf, dataBuf, dataBufSize) != asynError)
    {
      printf("%s\n", dataBuf);
    }

  return rtn;
}


asynStatus
drvWS3122::writeInt32(asynUser *pasynUser, epicsInt32 value)
{
  int addr;
  int function = pasynUser->reason;
  int status=0;
  static const char *functionName = "writeInt32";
  this->getAddress(pasynUser, &addr);
  setIntegerParam(addr, function, value);

  
  
  if(function == Identification) {
    status = this->CmdsSendRead(pasynUser, 0);
  } 

  /* Do callbacks so higher layers see any changes */

  callParamCallbacks(addr);

  if (status == 0) {
    asynPrint(pasynUser, ASYN_TRACEIO_DRIVER,
	      "%s:%s, port %s, wrote %d to address %d\n",
	      driverName, functionName, this->portName, value, addr);
  } else {
    asynPrint(pasynUser, ASYN_TRACE_ERROR,
	      "%s:%s, port %s, ERROR writing %d to address %d, status=%d\n",
	      driverName, functionName, this->portName, value, addr, status);
  }
  return (status==0) ? asynSuccess : asynError;
}



drvWS3122::drvWS3122(const char *portName, const char *asynUSBTMCPortName) 
  : asynPortDriver(portName, 
		   1, /* maxAddr */ 
		   NUM_WS3122_PARAMS,
		   asynInt32Mask | asynFloat64Mask | asynOctetMask | asynInt32ArrayMask | asynGenericPointerMask | asynDrvUserMask, 
		   asynInt32Mask | asynFloat64Mask | asynOctetMask | asynInt32ArrayMask | asynGenericPointerMask,
		   0, /* asynFlags.  This driver does not block and it is not multi-device, so flag is 0 */
		   1, /* Autoconnect */
		   0, /* Default priority */
		   0) /* Default stack size*/    
{
  
  const char *functionName = "WS3122";
  asynStatus status;

  createParam(DevIDNString,                    asynParamInt32, &Identification);
  createParam(WS3122ManufacturerString,        asynParamOctet, &WS3122Manufacturer);
  createParam(WS3122ModelString,               asynParamOctet, &WS3122Model);
  createParam(WS3122SerialNumberString,        asynParamOctet, &WS3122SerialNumber);
  createParam(WS3122FirmwareVersionString,     asynParamOctet, &WS3122FirmwareVersion);

  status = pasynOctetSyncIO->connect(asynUSBTMCPortName, 0, &(this->pasynUserDriver), NULL);

  setStringParam(WS3122Manufacturer, "TeleDyne Lecroy");
  setStringParam(WS3122Model,        "WaveStation 3122");
  setStringParam(WS3122SerialNumber, "000000000000");
  setStringParam(WS3122FirmwareVersion, "xxxxxxxxxxxx");
		
  
  if (status) {
    printf("%s:%s: epicsThreadCreate failure\n", driverName, functionName);
  }
  
};



asynStatus
drvWS3122::usbTMCPortWriteRead(char *sendBuffer, char *recvBuffer, int &recvBufSize, double timeout)
{
  size_t nActual = 0;
  size_t nRead = 0;
  int    eomReason;
  char   *pValue      = sendBuffer;
  size_t nChars       = strlen(sendBuffer);
  char   *pReadBuffer = recvBuffer;

  printf("hello\n");
  printf("%s\n", sendBuffer);
  // printf("%s %s, %s, %d, %zd %zd, %d\n",  pValue, nChars,
  // 						    pReadBuffer, MAX_BUF_SIZE, timeout,
  // 						    &nActual, &nRead, &eomReason);
  
  asynStatus status   = pasynOctetSyncIO->writeRead(pasynUserDriver, pValue, nChars,
						    pReadBuffer, MAX_BUF_SIZE, timeout,
						    &nActual, &nRead, &eomReason);
  status = (asynStatus) callParamCallbacks();
  recvBufSize = nRead;
  
  return status;
}


// asynStatus
// drvWS3122::writeOctet(asynUser *pasynUser, const char *value, size_t Chars, size_t *nActual)
// {
//     asynStatus status = asynSuccess;

//     return status;
// }





/* Configuration routine.  Called directly, or from the iocsh function below */

extern "C" {

  /** EPICS iocsh callable function to call constructor for the WS3122 class.**/
  
  int drvWS3122Configure(const char *portName, const char *asynUSBTMCPortName)
  {
    new drvWS3122(portName, asynUSBTMCPortName);
    return(asynSuccess);
  }


  /* EPICS iocsh shell commands */

  static const iocshArg initArg0 = { "portName",           iocshArgString};
  static const iocshArg initArg1 = { "asynUSBTMCPortName", iocshArgString};
  static const iocshArg * const initArgs[] = {&initArg0,
					      &initArg1};
  static const iocshFuncDef initFuncDef = {"drvWS3122Configure",2,initArgs};
  static void initCallFunc(const iocshArgBuf *args)
  {
    drvWS3122Configure(args[0].sval, args[1].sval);
  }

  void drvWS3122Register(void) {
    iocshRegister(&initFuncDef, initCallFunc);
  }

  epicsExportRegistrar(drvWS3122Register);

}


