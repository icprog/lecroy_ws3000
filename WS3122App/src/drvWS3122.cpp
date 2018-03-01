
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

#include "drvWS3122.hh"

static const char *driverName="drvWS3122";

asynStatus drvWS3122::writeInt32(asynUser *pasynUser, epicsInt32 value)
{
    int function = pasynUser->reason;
    asynStatus status = asynSuccess;
    const char *functionName = "writeInt32";

    status = setIntegerParam(function, value);

    // if (function == ADShutterControl) {
    //     setShutter(value);
    // } else {
    //     /* If this parameter belongs to a base class call its method */
    //     if (function < FIRST_AD_PARAM) status = asynNDArrayDriver::writeInt32(pasynUser, value);
    // }

    /* Do callbacks so higher layers see any changes */
    callParamCallbacks();

    if (status)
        asynPrint(pasynUser, ASYN_TRACE_ERROR,
              "%s:%s: error, status=%d function=%d, value=%d\n",
              driverName, functionName, status, function, value);
    else
        asynPrint(pasynUser, ASYN_TRACEIO_DRIVER,
              "%s:%s: function=%d, value=%d\n",
              driverName, functionName, function, value);
    return status;
}



drvWS3122::drvWS3122(const char *portName, const char *asynUSBTMCPortName) 
  : asynPortDriver(portName, 
		   0, /* maxAddr */ 
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

  createParam(WS3122ManufacturerString,        asynParamOctet, &WS3122Manufacturer);
  createParam(WS3122ModelString,               asynParamOctet, &WS3122Model);
  createParam(WS3122SerialNumberString,        asynParamOctet, &WS3122SerialNumber);
  createParam(WS3122FirmwareVersionString,     asynParamOctet, &WS3122FirmwareVersion);

  status = pasynOctetSyncIO->connect(asynUSBTMCPortName, 0, &(this->pasynUserDriver), NULL);
    
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
  asynStatus status = pasynOctetSyncIO->writeRead(pasynUserDriver, pValue, nChars,
						  pReadBuffer, MAX_BUF_SIZE, timeout,
						  &nActual, &nRead, &eomReason);
  status = (asynStatus) callParamCallbacks();
  recvBufSize = nRead;
  return status;
}


asynStatus
drvWS3122::usbTMCPortRead(char *recvBuffer, int &recvBufSize, double timeout)
{
  size_t nRead = 0;
  int    eomReason;
  char   *pReadBuffer = recvBuffer;
  //  memset(pReadBuffer, 0x00, sizeof(pReadBuffer));
  asynStatus status = pasynOctetSyncIO->read(pasynUserDriver, pReadBuffer,
					     MAX_BUF_SIZE, timeout,
					     &nRead, &eomReason);
  status = (asynStatus) callParamCallbacks();
  return status;
}


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


