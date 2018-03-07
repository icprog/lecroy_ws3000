

#include <epicsTypes.h>
#include <epicsTime.h>
#include <epicsThread.h>
#include <epicsString.h>
#include <epicsTimer.h>
#include <epicsMutex.h>
#include <epicsEvent.h>
#include <epicsExit.h>
#include <iocsh.h>

#include <epicsExport.h>
#include <asynDriver.h>

#define epicsExportSharedSymbols
#include <shareLib.h>

//#include "WS3122Def.hh"
#include "drvWS3122.hh"


static const char *driverName="drvWS3122";

static void exitHandler(void *drvPvt) {
	drvWS3122 *pPvt = (drvWS3122 *)drvPvt;
	delete pPvt;
}


drvWS3122::drvWS3122(const char *portName, const char *asynUSBTMCPortName) 
  : asynPortDriver(portName, 
		   MAX_ASYN_ADDRESS, 
		   NUM_PARAMS,
		   asynInt32Mask | asynOctetMask |  asynDrvUserMask, 
		   asynInt32Mask | asynOctetMask,
		   0, /* asynFlags.  This driver does not block and it is not multi-device, so flag is 0 */
		   1,    /* Autoconnect */
		   0, 0) /* Default priority and stack size*/    
{
  
  const char *functionName = "WS3122";
  asynStatus status;
  
    /* Create an EPICS exit handler */
  epicsAtExit(exitHandler, this);
  
  
  createParam(DevIDNString,                    asynParamOctet, &devIdentification_);
  createParam(WS3122ManufacturerString,        asynParamOctet, &devManufacturer_);
  createParam(WS3122ModelString,               asynParamOctet, &devModel_);
  createParam(WS3122SerialNumberString,        asynParamOctet, &devSerialNumber_);
  createParam(WS3122FirmwareVersionString,     asynParamOctet, &devFirmwareVersion_);
  
  status = pasynOctetSyncIO->connect(asynUSBTMCPortName, 0, &usbTmcAsynUser, NULL);
  
  if (status) {
    printf("%s: Create failure\n", functionName);  
  }
 
};

drvWS3122::~drvWS3122()
{
  pasynOctetSyncIO->disconnect(usbTmcAsynUser);
};


 // size_t nActual = 0;
 //  size_t nRead;
 //  int    eomReason;
 //  char   *pValue      = sendBuffer;
 //  size_t nChars       = strlen(sendBuffer);
 //  char   *pReadBuffer = recvBuffer;

 //  asynStatus status = pasynOctetSyncIO->writeRead(this->pasynUserDriver, pValue, nChars,
 //                                                  pReadBuffer, MAX_BUF_SIZE, timeout,
 //                                                  &nActual, &nRead, &eomReason);
 //  //printf("[DEBUG(drvGL820::WriteRead)]: status=%d pReadBuffer=%s\n", status, pReadBuffer);
 //  //  status = (asynStatus) callParamCallbacks();
 //  callParamCallbacks();
 //  recvBufSize = nRead;


asynStatus
drvWS3122::usbTmcWriteRead(char *sendBuffer, char *recvBuffer, int &recvBufSize, int timeout)
{
  size_t nbytesOut = 0;
  size_t nbytesIn  = 0;
  int    eomReason;
  size_t send_buffer_len = strlen(sendBuffer);
  size_t read_buffer_len = MAX_BUF_SIZE;
  char   *pReadBuffer = recvBuffer;


  asynStatus status = pasynOctetSyncIO->writeRead(this->usbTmcAsynUser,
						  sendBuffer,
						  send_buffer_len,
                                                  pReadBuffer,
						  read_buffer_len,
						  timeout,
                                                  &nbytesOut,
						  &nbytesIn,
						  &eomReason);

  callParamCallbacks();
  recvBufSize = nbytesIn;
  
  printf("%s :: sendBuffer %s, len %zd, recvBuffer %s, len %zd, nbytesOut %zd, nbytesIn %zd, eomReason %d\n",
	 __func__,
	 sendBuffer,
	 send_buffer_len,
	 pReadBuffer,
	 read_buffer_len,
	 nbytesOut,
	 nbytesIn,
	 eomReason);
  
  return status;

}

// asynStatus
// drvWS3122::usbTmcWrite(double timeout)
// {
//   asynStatus status        = asynSuccess;
//   const char *functionName = "usbTmcWrite";
  
//   status = pasynOctetSyncIO->write(usbTmcAsynUser,
// 				   sendBuffer.c_str(),
// 				   sendBufferSize,
// 				   timeout,
// 				   &sendBufferActualSize);

//   callParamCallbacks();

//   if (status) {
//     asynPrint(pasynUserSelf, ASYN_TRACE_ERROR,
// 	      "%s:%s, status=%d\n", driverName, functionName, status);
//   }
  
//   return status;
// }



// asynStatus
// drvWS3122::buildSendBuffer()
// {
//   asynStatus status = asynSuccess;

//   memset(sendBuffer, 0, MAX_BUFFER_SIZE);
//   memset(recvBuffer, 0, MAX_BUFFER_SIZE);
//   sendBufferActualSize = 0;
//   recvBufferActualSize = 0;
  
//   sendBufferSize = strlen(sendBuffer);

//   memcpy(sendBuffer, msg, sendBufferSize);

//   return status;
// }





asynStatus
drvWS3122::writeOctet(asynUser *pasynUser, const char *value, size_t nChars, size_t *nActual)
{
  int function = pasynUser->reason;
  asynStatus status = asynSuccess;
  const char *paramName;
  const char* functionName = "writeOctet";

 printf("%s :: value %s, nChars %zd, nActual %zd\n",
	 __func__,
	value, nChars, *nActual);
 

  /* Set the parameter in the parameter library. */
  status = (asynStatus) setStringParam(function, value);

  /* Fetch the parameter string name for possible use in debugging */
  getParamName(function, &paramName);

  std::string value_s(value, nChars);

  try
    {
      if(function == devIdentification_) {
	value_s = "*IDN?";
	nChars = value_s.size();
	status = this->SetDataCmd(value_s);
      }

      callParamCallbacks();
      
      if (status == 0) {
	asynPrint(pasynUser, ASYN_TRACEIO_DRIVER, 
		  "%s:%s: function=%d, name=%s, value=%s\n", 
		  driverName, functionName, function, paramName, value_s.c_str());
	*nActual = value_s.size();
      }
      else {
	asynPrint(pasynUser, ASYN_TRACE_ERROR,
		  "%s:%s, function=%d,  name=%s, value=%s, status=%d\n",
		  driverName, functionName, function, paramName, value_s.c_str(), status);
      }

      return (status==0) ? asynSuccess : asynError;
    }
  catch(const std::exception& ex)
    {
      epicsSnprintf(pasynUser->errorMessage, pasynUser->errorMessageSize, 
  		    "%s:%s: status=%d, function=%d, name=%s, value=%s, error=%s", 
  		    driverName, functionName, status, function, paramName, value_s.c_str(), ex.what());
      *nActual = 0;
      return asynError;
    }
};



asynStatus
drvWS3122::SetDataCmd(std::string cmd)
{
  asynStatus status = asynSuccess;

  char dataBuf[256];
  int  dataBufSize;
 
  printf("%s\n", __func__);
  memset(dataBuf, 0x00, sizeof(dataBuf));

  //pasynOctetSyncIO->setOutputEos(this->usbTmcAsynUser, "\n", 1);
  pasynOctetSyncIO->setInputEos(this->usbTmcAsynUser, "\n", 1);


  if (this->usbTmcWriteRead((char*) cmd.c_str(), dataBuf, dataBufSize) != asynError) {
    asynPortDriver::setStringParam(devIdentification_, dataBuf);
  }

  return status;
}

//
  // std::string sendBuffer;
  // size_t sendBufferSize;
  // size_t sendBufferActualSize;
  // std::string recvBuffer;
  // size_t recvBufferSize;
  // size_t recvBufferActualSize;





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


