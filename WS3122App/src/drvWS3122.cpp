

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


/*
 * Decode a status byte
 */
static void
showHexval(FILE *fp, const char *name, int val, int bitmask, const char *bitname, ...)
{
    const char *sep = " -- ";
    va_list ap;
    va_start(ap, bitname);
    fprintf(fp, "%28s: ", name);
    if (bitmask)
        fprintf(fp, "%#x", val);
    else
        fprintf(fp, "%#4.4x", val);
    for (;;) {
        if (((bitmask > 0) && ((val & bitmask)) != 0)
         || ((bitmask < 0) && ((val & -bitmask)) == 0)
         || ((bitmask == 0) && (bitname != NULL) && (bitname[0] != '\0'))) {
            fprintf(fp, "%s%s", sep, bitname);
            sep = ", ";
        }
        if (bitmask == 0)
            break;
        bitmask = va_arg(ap, int);
        if (bitmask == 0)
            break;
        bitname = va_arg(ap, char *);
    }
    fprintf(fp, "\n");
    va_end(ap);
}

/*
 * Show a byte number
 */
static void
pcomma(FILE *fp, size_t n)
{
    if (n < 1000) {
        fprintf(fp, "%zu", n);
        return;
    }
    pcomma(fp, n/1000);
    fprintf(fp, ",%03zu", n%1000);
}
    
static void
showCount(FILE *fp, const char *label, size_t count)
{
    fprintf(fp, "%22s Count: ", label);
    pcomma(fp, count);
    fprintf(fp, "\n");
}







drvWS3122::drvWS3122(const char *portName, const char *asynUSBTMCPortName) 
  : asynPortDriver(portName, 
		   MAX_ASYN_ADDRESS, 
		   asynInt32Mask  | asynFloat64Mask | asynOctetMask |  asynDrvUserMask, 
		   asynInt32Mask  | asynFloat64Mask | asynOctetMask,
		   0, /* asynFlags.  This driver does not block and it is not multi-device, so flag is 0 */
		   1,    /* Autoconnect */
		   0, 0) /* Default priority and stack size*/    
{


  asynUsbTmcPortName = asynUSBTMCPortName;
  /* Create an EPICS exit handler */
  epicsAtExit(exitHandler, this);

   
  Init();
  
 
};

drvWS3122::~drvWS3122()
{
  pasynOctetSyncIO->disconnect(usbTmcAsynUser);
  if(basicWave) { delete basicWave; basicWave = NULL; }
  if(burstWave) { delete burstWave; burstWave = NULL; }
};

asynStatus
drvWS3122::Init()
{
  basicWave = new BasicWave();
  burstWave = new BurstWave();
  
  asynStatus status = asynSuccess;
  
  createParam(DevIDNString,                asynParamOctet,   &devIdentification_);
  createParam(DevManufacturerString,       asynParamOctet,   &devManufacturer_  );
  createParam(DevModelString,              asynParamOctet,   &devModel_         );
  createParam(DevSerialNumberString,       asynParamOctet,   &devSerialNumber_  );

  createParam(ParHeaderPathString,         asynParamInt32,   &parHeaderPath_    );
  createParam(ParHeaderString,             asynParamInt32,   &parHeader_    );
  createParam(ParBasicWaveTypeSelecString, asynParamInt32,   &parBasicWaveTypeSelect_);
 
    
  createParam(ParWaveFrequencyString,      asynParamFloat64, &parWaveFrequency_);
  createParam(ParWaveAmplifierString,      asynParamFloat64, &parWaveAmplifier_);
  createParam(ParWaveOffsetString,         asynParamFloat64, &parWaveOffset_);
  createParam(ParWavePhaseString,          asynParamFloat64, &parWavePhase_);
  createParam(ParWaveWidthString,          asynParamFloat64, &parWaveWidth_);
  createParam(ParWaveRiseString,           asynParamFloat64, &parWaveRise_);
  createParam(ParWaveFallString,           asynParamFloat64, &parWaveFall_);
  createParam(ParWaveDelayString,          asynParamFloat64, &parWaveDelay_);
  createParam(ParWaveSymmetryString,       asynParamFloat64, &parWaveSymmetry_);
  createParam(ParWaveStdDevString,         asynParamFloat64, &parWaveStdDev_);
  createParam(ParWaveMeanString,           asynParamFloat64, &parWaveMean_);
  createParam(ParWaveDutyCycleString,      asynParamFloat64, &parWaveDutyCycle_);


  createParam(ParBurstGateNcycString,      asynParamInt32,   &parBurstGateNcyc_);

  createParam(ParBurstTriggerSrcString,    asynParamInt32,   &parBurstTriggerSrc_);
  createParam(ParBurstTriggerModeString,   asynParamInt32,   &parBurstTriggerMode_);
  createParam(ParBurstEdgeString,          asynParamInt32,   &parBurstEdge_);
  createParam(ParBurstPolarityString,      asynParamInt32,   &parBurstPolatiry_);
  createParam(ParBurstManualTriggerString, asynParamInt32,   &parBurstManualTrigger_);
    
  createParam(ParBurstPeriodString,        asynParamFloat64, &parBurstPeriod_);
  createParam(ParBurstStartPhaseString,    asynParamFloat64, &parBurstStartPhase_);
  createParam(ParBurstDelayString,         asynParamFloat64, &parBurstDelay_);
  createParam(ParBurstCycleTimeString,     asynParamFloat64, &parBurstCycleTime_);
  

  createParam(CmdWaveStateString,          asynParamInt32,   &cmdWaveState_   );
  createParam(CmdOutputString,             asynParamInt32,   &cmdOutput_      );
  createParam(CmdOutputLoadString,         asynParamInt32,   &cmdOutputLoad_  );
  createParam(CmdOutputPolarityString,     asynParamInt32,   &cmdOutputPolarity_);
  createParam(CmdSetWaveTypeString,        asynParamInt32,   &cmdSetWaveType_ );
  createParam(CmdPhaseInvertString,        asynParamInt32,   &cmdPhaseInvert_ );
  createParam(CmdScreenSaveString,         asynParamInt32,   &cmdScreenSave_  );
  createParam(CmdClockSourceString,        asynParamInt32,   &cmdClockSource_ );
  
  status = pasynOctetSyncIO->connect(this->getUsbTmcPortName(), 0, &usbTmcAsynUser, NULL);

  pasynInterface = pasynManager->findInterface(usbTmcAsynUser, asynDrvUserType, 1);
  pasynDrvPvt    = (drvPvt *)pasynInterface->drvPvt;

  status = report_device_information(stdout);
  status = set_device_information();

  if (status) {
    printf("Initialization  failure\n");  
  }
  
  return status;
}


asynStatus
drvWS3122::report_device_information(FILE *fp)
{
  asynStatus status = asynSuccess;
  pasynManager->report(stdout, 10, this->getUsbTmcPortName());
  

  fprintf(fp, "\nReport Device Information : %s\n", __func__ );
  fprintf(fp,"        %s pinterface %p drvPvt %p\n", pasynInterface->interfaceType, pasynInterface->pinterface, pasynInterface->drvPvt);
  
  fprintf(fp, "%20sonnected, Interrupt handler thread %sactive\n", pasynDrvPvt->isConnected ? "C" : "Disc", pasynDrvPvt->interruptTid ? "" : "in");
  
  showHexval(fp, "Vendor",  pasynDrvPvt->deviceVendorId,  0, pasynDrvPvt->deviceVendorString);
  showHexval(fp, "Product", pasynDrvPvt->deviceProductId, 0, pasynDrvPvt->deviceProductString);

  if (pasynDrvPvt->deviceSerialString[0]) {
    fprintf(fp, "%28s: \"%s\"\n", "Serial Number", pasynDrvPvt->deviceSerialString);
  }
  
  fprintf(fp, "          Interface Protocol: %x",  pasynDrvPvt->bInterfaceProtocol);
  
  switch ( pasynDrvPvt->bInterfaceProtocol)
    {
    case  0 : fprintf(fp, " -- USBTMC\n");        break;
    case  1 : fprintf(fp, " -- USBTMC USB488\n"); break;
    default : fprintf(fp, "\n");                  break;
    }
  
  if (pasynDrvPvt->termChar >= 0) {
    fprintf(fp, "%28s: %x\n", "Terminator",  pasynDrvPvt->termChar);
  }
  
  
  showHexval(fp, "TMC Interface Capabilities", pasynDrvPvt->tmcInterfaceCapabilities,
	     0x4, "Accepts INDICATOR_PULSE", 0x2, "Talk-only", 0x1, "Listen-only", -0x3, "Talk/Listen", 0);
  
  showHexval(fp, "TMC Device Capabilities",    pasynDrvPvt->tmcDeviceCapabilities,
	     0x1, "Supports termChar", 0);
  
  if (pasynDrvPvt->bInterfaceProtocol == 1) {
    showHexval(fp, "488 Interface Capabilities", pasynDrvPvt->usb488InterfaceCapabilities,
	       0x4, "488.2", 0x2, "REN/GTL/LLO", 0x1, "TRG", 0);
    
    showHexval(fp, "488 Device Capabilities", pasynDrvPvt->usb488DeviceCapabilities,
	       0x8, "SCPI", 0x4, "SR1", -0x4, "SR0", 0x2, "RL1", -0x2, "RL0", 0x1, "DT1", -0x1, "DT0", 0);
  }
  
  showCount(fp, "Connection", pasynDrvPvt->connectionCount);
  showCount(fp, "Interrupt",  pasynDrvPvt->interruptCount);
  showCount(fp, "Send",       pasynDrvPvt->bytesSentCount);
  showCount(fp, "Receive",    pasynDrvPvt->bytesReceivedCount);
  
  fprintf(fp, "\n");
  return status;
  
};


asynStatus drvWS3122::set_device_information()
{
  asynStatus status = asynSuccess;

  setStringParam(devManufacturer_, pasynDrvPvt->deviceVendorString);
  setStringParam(devModel_,        pasynDrvPvt->deviceProductString);
  
  if (pasynDrvPvt->deviceSerialString[0]) {
    setStringParam(devSerialNumber_, pasynDrvPvt->deviceSerialString);
  }
  
  return status;
  
};


void
drvWS3122::set_burst_wave_flags()
{
  int    wave_type  = 0;
  int    burst_mode = 0;
  int    trig_src   = 0;
  
  getIntegerParam(parBasicWaveTypeSelect_, &wave_type);
  getIntegerParam(parBurstGateNcyc_,       &burst_mode);
  getIntegerParam(parBurstTriggerSrc_,     &trig_src);
  burstWave -> set_flags((EBasicWaveType_t) wave_type, (EBurstMode_t) burst_mode , (ETriggerSrc_t) trig_src);

  
};

asynStatus
drvWS3122::set_wave_parameters(int function, epicsInt32 iValue, epicsFloat64 fValue, std::string value_s, asynParamType paramType)
{
  asynStatus status = asynSuccess;
  int    par        = 0;
  double val        = 0.0;
  double val_reset  = -0.1;

  int    wave_type  = 0;
  int    par_header = 0;
  // int    trig_src   = 0;

  // std::cout << "function " << function
  // 	    << " iValue "   << iValue
  // 	    << " fValue "   << fValue
  // 	    << " value_s "   << value_s
  // 	    << std::endl;

  
  
  if (paramType ==  asynParamInt32) {

    par = 0;
    getIntegerParam(function, &par);
    
    if (function ==  parHeader_ ) {
      switch ((EHeaderType_t) par)
	{
	case kHeaderBSWV:
	  
	  basicWave -> setCarrierFlag(false);
	  getIntegerParam(parBasicWaveTypeSelect_, &wave_type);
	  basicWave -> setWaveTypeID((EBasicWaveType_t) wave_type);
	  setIntegerParam(parBasicWaveTypeSelect_, wave_type);
	  
	  break;
	case kHeaderBTWV:
	  getIntegerParam(parBasicWaveTypeSelect_,      &wave_type);
	  basicWave -> setWaveTypeID((EBasicWaveType_t) wave_type);
	  setIntegerParam(parBasicWaveTypeSelect_, wave_type);
	  if( (EBasicWaveType_t) wave_type == kWaveTypeDc) {
	    basicWave -> setCarrierFlag(false);
	    burstWave -> setEnable(false);
	  }
	  else {

	    basicWave -> setCarrierFlag(true);
	    burstWave -> setEnable(true);
	    burstWave -> setCarrierWaveTypeID((EBasicWaveType_t) wave_type);
	    //	    std::cout << GetBasicWaveType((EBasicWaveType_t) wave_type) << std::endl;
	    //	    std::cout << burstWave -> getCarrierWaveTypeString() << std::endl;
	  }
	  this -> set_burst_wave_flags();
	  break;
	case kHeaderARWV:	// not yet implemented
	  basicWave -> setCarrierFlag(true);
	  break;
	case kHeaderMDWV: // not yet implemented
	  basicWave -> setCarrierFlag(true);
	  break;
	case kHeaderSWWV:	// not yet implemented
	  basicWave -> setCarrierFlag(true);
	  break;
	default:
	  basicWave-> setCarrierFlag(true);
	  break;
	}

    }
    else if ( function == parHeaderPath_ ) {
      basicWave -> setHeaderPath((EHeaderPath_t) par);
      burstWave -> setHeaderPath((EHeaderPath_t) par);
    }
    else if ( function == parBasicWaveTypeSelect_ ){

      getIntegerParam(parHeader_, & par_header);
      
      switch ((EHeaderType_t) par_header)
	{
	case kHeaderBSWV:
	  basicWave -> setCarrierFlag(false);
	  basicWave -> setWaveTypeID((EBasicWaveType_t) par);
	  break;
	case kHeaderBTWV:
	  basicWave -> setWaveTypeID((EBasicWaveType_t) par);
	  if( (EBasicWaveType_t) wave_type == kWaveTypeDc) {
	    basicWave -> setCarrierFlag(false);
	    burstWave -> setEnable(false);
	  }
	  else {
	    basicWave -> setCarrierFlag(true);
	    burstWave -> setEnable(true);
	    burstWave -> setCarrierWaveTypeID((EBasicWaveType_t) par);
	  }
	  this -> set_burst_wave_flags();
	  
	  break;
	case kHeaderARWV:	// not yet implemented
	  basicWave -> setCarrierFlag(true);
	  break;
	case kHeaderMDWV: // not yet implemented
	  basicWave -> setCarrierFlag(true);
	  break;
	case kHeaderSWWV:	// not yet implemented
	  basicWave -> setCarrierFlag(true);
	  break;
	default:
	  basicWave-> setCarrierFlag(true);
	  break;
	}
      
    }
    else if ( function == parBurstGateNcyc_ ) {
      burstWave -> setBurstMode((EBurstMode_t) par);
      this -> set_burst_wave_flags();
    }
    else if ( function == parBurstTriggerSrc_ ) {
      burstWave -> setTriggerSrc((ETriggerSrc_t) par);
      this -> set_burst_wave_flags();
    }
    else if ( function == parBurstTriggerMode_ ) {
      burstWave -> setTriggerMode((ETriggerMode_t) par);
      this -> set_burst_wave_flags();
    }
    else if ( function == parBurstPolatiry_ ) {
      burstWave -> setPolarity((EPolarityMap_t) par);
      this -> set_burst_wave_flags();
    }
    else if ( function == parBurstEdge_ ) {
      burstWave -> setEdge((EEdgeModeMap_t) par);
      this -> set_burst_wave_flags();
    }
    else if ( function == cmdWaveState_ ) {
      burstWave -> setWaveState((EWaveState_t) par);
      this -> SetWaveState(iValue);
    }
    else if ( function == parBurstManualTrigger_) {
      
    }
    
    setIntegerParam(function, par);
  }; // if (fValue == 0.0)

  
  if (paramType== asynParamFloat64) {

    val = 0.0;
    getDoubleParam(function, &val);

    if (function == parWaveFrequency_ ) {
      if( basicWave->IsFrequency() ) basicWave -> setFrequencyVal(val);
      else val = val_reset;
    }
    else if (function ==  parWaveAmplifier_ ) {
      if( basicWave->IsAmplifier() ) basicWave -> setAmplifierVal(val);
      else val = val_reset;
    }
    else if (function ==  parWaveOffset_ ) {
      if( basicWave->IsOffset() ) basicWave -> setOffsetVal(val);
      else val = val_reset;
    }
    else if (function ==  parWavePhase_ ) {
      if( basicWave->IsPhase() ) basicWave -> setPhaseVal(val);
      else val = val_reset;
    }
    else if (function ==  parWaveWidth_ ) {
      if( basicWave->IsWidth() ) basicWave -> setWidthVal(val);
      else val = val_reset;
    }
    else if (function ==  parWaveRise_ ) {
      if( basicWave->IsRise() ) basicWave -> setRiseVal(val);
      else  val = val_reset;
    }
    else if (function ==  parWaveFall_ ) {
      if( basicWave->IsFall() ) basicWave -> setFallVal(val);
      else val = val_reset;
    }
    else if (function ==  parWaveDelay_ ) {
      if( basicWave->IsDelay() ) basicWave -> setDelayVal(val);
      else val = val_reset;
    }
    else if (function ==  parWaveSymmetry_ ) {
      if( basicWave->IsSymmetry() ) basicWave -> setSymmetryVal(val);
      else val = val_reset;
    }    
    else if (function ==  parWaveStdDev_ ) {
      if( basicWave->IsStdDev() ) basicWave -> setStdDevVal(val);
      else val = val_reset;
    }
    else if (function ==  parWaveMean_ ) {
      if( basicWave->IsMean() ) basicWave -> setMeanVal(val);
      else val = val_reset;
    }
    else if (function ==  parWaveDutyCycle_ ) {
      if( basicWave->IsDutyCycle() ) basicWave -> setDutyCycleVal(val);
      else val=val_reset;
    }
    else if (function ==  parBurstPeriod_ ) {
      // here we put value into the class first. 
      burstWave -> setPeriodVal(val);
      this -> set_burst_wave_flags();
    }
    else if (function ==  parBurstStartPhase_ ) {
      burstWave -> setStartPhaseVal(val);
      this -> set_burst_wave_flags();
    }
    else if (function ==  parBurstDelay_ ) {
      burstWave -> setDelayVal(val);
      this -> set_burst_wave_flags();
    }
    else if (function ==  parBurstCycleTime_ ) {
      burstWave -> setCycleTimeVal(val);
      this -> set_burst_wave_flags();
    }
    
    setDoubleParam(function, val);
  } // if fValue == 0
  

  // basicWave -> Print( value_s );
  
  return status;
  
}

  



asynStatus
drvWS3122::usbTmcWrite(std::string sendBuffer, double timeout)
{
  size_t nbytesOut       = 0;
 /* 
    asynUser   *pasynUser,
    const char *write_buffer, 
    size_t      write_buffer_len,
    double      timeout,
    size_t     *nbytesOut
 */

 
  asynStatus status = pasynOctetSyncIO->write(this->usbTmcAsynUser,
					      sendBuffer.c_str(),
					      sendBuffer.size(),
					      timeout,
					      &nbytesOut);
    
  callParamCallbacks();
 
  return status;

}




asynStatus
drvWS3122::usbTmcWriteRead(char *sendBuffer, char *recvBuffer, int &recvBufSize, double timeout)
{
  size_t nbytesOut = 0;
  size_t nbytesIn  = 0;
  int    eomReason;
  size_t send_buffer_len = strlen(sendBuffer);
  size_t read_buffer_len = MAX_BUF_SIZE;
  char   *pReadBuffer    = recvBuffer;

 printf("%s :: sendBuffer %s, len %zd, recvBuffer %s, len %zd, nbytesOut %zd, nbytesIn %zd, eomReason %d\n",
	 __func__,
	 sendBuffer,
	 send_buffer_len,
	 pReadBuffer,
	 read_buffer_len,
	 nbytesOut,
	 nbytesIn,
	 eomReason);

 /* 
    asynUser   *pasynUser,
    const char *write_buffer, 
    size_t      write_buffer_len,
    char       *read_buffer, 
    size_t      read_buffer_len,
    double      timeout,
    size_t     *nbytesOut, 
    size_t     *nbytesIn, 
    int        *eomReason
 */
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


asynStatus
drvWS3122::writeInt32(asynUser *pasynUser, epicsInt32 value)
{
  int function = pasynUser->reason;
  asynStatus status = asynSuccess;
  const char* functionName = "writeInt32";

  setIntegerParam(function, value);

  std::string value_s;
  std::string string_in = functionName;
  
  if ( function == cmdClockSource_ ) {
    status = this-> SetClockSource(value);
  } else if (function == cmdPhaseInvert_ ) {
    status = this-> SetPhaseInvert(value);
  } else if (function == cmdScreenSave_ ) {
    status = this-> SetScreenSave(value);
  } else if (function == cmdSetWaveType_ ) {
    status = this-> SetWaveTypeCmds(value);
  } else if (function == cmdOutput_ ) {
    status = this-> SetOutput(value, kOutputOnOff);
  } else if (function == cmdOutputLoad_ ) {
    status = this-> SetOutput(value, kOutputLoad);
  } else if  (function == cmdOutputPolarity_ ) {
    status = this-> SetOutput(value, kOutputPolarity);
  } else {
    status = this->set_wave_parameters(function, value, 0, functionName, asynParamInt32);
  }
  
  callParamCallbacks();
  
  if (status == 0) {
    asynPrint(pasynUser, ASYN_TRACEIO_DRIVER, 
	      "%s:%s, port %s, wrote %d \n",
	      driverName, functionName, this->portName, value);
  } else {
    asynPrint(pasynUser, ASYN_TRACE_ERROR, 
	      "%s:%s, port %s, ERROR writing %d to  status=%d\n",
	      driverName, functionName, this->portName, value, status);
  }
  return (status==0) ? asynSuccess : asynError;
}


asynStatus
drvWS3122::writeFloat64(asynUser *pasynUser, epicsFloat64 value)
{
    int function = pasynUser->reason;
    asynStatus status = asynSuccess;
    //    epicsInt32 run;
    const char *paramName;
    const char* functionName = "writeFloat64";

    /* Set the parameter in the parameter library. */
    status = (asynStatus) setDoubleParam(function, value);

    /* Fetch the parameter string name for possible use in debugging */
    getParamName(function, &paramName);

    status = this->set_wave_parameters( function, 0, value, functionName, asynParamFloat64);
    
    /* Do callbacks so higher layers see any changes */
    status = (asynStatus) callParamCallbacks();
    
    if (status) 
        epicsSnprintf(pasynUser->errorMessage, pasynUser->errorMessageSize, 
                  "%s:%s: status=%d, function=%d, name=%s, value=%f", 
                  driverName, functionName, status, function, paramName, value);
    else        
        asynPrint(pasynUser, ASYN_TRACEIO_DRIVER, 
              "%s:%s: function=%d, name=%s, value=%f\n", 
              driverName, functionName, function, paramName, value);
    return status;
}



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
	status = this->write_read(value_s);
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
drvWS3122::write_read(std::string cmd)
{
  asynStatus status = asynSuccess;

  char dataBuf[256];
  int  dataBufSize;
 
  printf("%s\n", __func__);
  memset(dataBuf, 0x00, sizeof(dataBuf));

  pasynOctetSyncIO->setOutputEos(this->usbTmcAsynUser, "", 0);
  pasynOctetSyncIO->setInputEos(this->usbTmcAsynUser, "\n", 1);


  if (this->usbTmcWriteRead((char*) cmd.c_str(), dataBuf, dataBufSize) != asynError) {
    asynPortDriver::setStringParam(devIdentification_, dataBuf);
  }

  return status;
}


asynStatus
drvWS3122::SetWaveTypeCmds(epicsInt32 value)
{
  asynStatus status = asynSuccess;
  std::string value_s;
  value_s.clear();

  int par = 0;
  
  getIntegerParam(parHeader_, &par);

  // std::cout <<  GetHeaderType((EHeaderType_t)  par) << std::endl;
  
  switch ((EHeaderType_t)  par)
    {
    case kHeaderBSWV:
      basicWave -> buildCommand();
      value_s = basicWave-> getFullCommand();
      basicWave -> clearCommand();
      break;
    case kHeaderBTWV:
      basicWave -> buildCommand();
      burstWave -> setCarrierCmd( basicWave->getFullCommand());
      basicWave -> clearCommand();
      
      burstWave -> buildCommand();
      value_s = burstWave-> getFullCommand();
      burstWave -> clearCommand();
      break;
    case kHeaderARWV:	// not yet implemented
      break;
    case kHeaderMDWV: // not yet implemented
      break;
    case kHeaderSWWV:	// not yet implemented
      break;
    default:
      break;
    }
  
  std::cout << "0 : value " << value << "\tdrvWS3122::SetWaveTypeCmds : " << value_s.length()
   	    << "\t cmd " << value_s << std::endl;

  
  status = this->usbTmcWrite(value_s);

  return status;
};






asynStatus
drvWS3122::SetClockSource(epicsInt32 value)
{
  asynStatus status = asynSuccess;
  
  std::string value_s; value_s.clear();
  value_s  = "ROSC";
  value_s += GetCmdSymbol(kCmdSymbolBlank);

  if (value == 0)  value_s += "INT"; // Internal Clock
  else             value_s += "EXT"; // External Clock

  status = this->usbTmcWrite(value_s);

  return status;
};


asynStatus
drvWS3122::SetPhaseInvert(epicsInt32 value)
{
  asynStatus status = asynSuccess;
  
  std::string value_s; value_s.clear();
  
  value_s  = "INVT";
  value_s += GetCmdSymbol(kCmdSymbolBlank);
  
  if (value == 0) value_s += "OFF";
  else            value_s += "ON";

  status = this->usbTmcWrite(value_s);

  return status;
};


//   OutputParameterMap     outputParameterMap;
// EOutputParameter_t
asynStatus
drvWS3122::SetOutput(epicsInt32 value, EOutputParameter_t id)
{
  asynStatus  status = asynSuccess;

  OutputParameterMap outputMap = CreateOutputParameterMap();
  
  std::string value_s;  value_s.clear();

  value_s  = basicWave->getHeaderPath();
  value_s += GetCmdSymbol(kCmdSymbolColon);
  value_s += "OUTPUT";
  value_s += GetCmdSymbol(kCmdSymbolBlank);

  
  switch (id)
    {
    case kOutputOnOff:
      value_s += outputMap[id];
      if(value == 0) value_s += "OFF";
      else           value_s += "ON";
      break;
    case kOutputLoad:
      value_s += outputMap[id];
      value_s += GetCmdSymbol(kCmdSymbolComma);
      if(value == 0) value_s += "50";
      else           value_s += "HZ";
      break;
    case kOutputPolarity:
      value_s += outputMap[id];
      value_s += GetCmdSymbol(kCmdSymbolComma);
      if(value == 0) value_s += "NOR";
      else           value_s += "INVT";
      break;
    default:
      //      if(value == 0) value_s += "ON";
      //      else           value_s += "OFF";
      break;
      
    }

  // std::cout << __func__ << "output " << value_s << std::endl;
  status = this->usbTmcWrite(value_s);

  return status;
};
  

asynStatus
drvWS3122::SetScreenSave(epicsInt32 value)
{
  asynStatus  status = asynSuccess;
  std::ostringstream value_oss;   // stream used for the conversion
  value_oss.str(""); value_oss.clear();

  value_oss << "SCSV";
  value_oss << GetCmdSymbol(kCmdSymbolBlank);
  value_oss << value;

  status = this->usbTmcWrite(value_oss.str());

  return status;
};



asynStatus
drvWS3122::SetWaveState(epicsInt32 value)
{
  asynStatus  status = asynSuccess;
  int    par_header_path = 0;
  int    par_header = 0;
  //  int    wave_type  = 0;
  std::ostringstream value_oss;   // stream used for the conversion
  value_oss.str(""); value_oss.clear();

  
  getIntegerParam(parHeaderPath_, &par_header_path); // Chanel
  getIntegerParam(parHeader_,     &par_header);      // 
  

	  
  value_oss << GetHeaderPath((EHeaderPath_t) par_header_path);
  value_oss << GetCmdSymbol(kCmdSymbolColon);
  value_oss << GetHeaderType((EHeaderType_t) par_header);
  value_oss << GetCmdSymbol(kCmdSymbolBlank);
  value_oss << "STATE";
  value_oss << GetCmdSymbol(kCmdSymbolComma);
  value_oss << GetWaveState((EWaveState_t) value);

  // std::cout << "SetWaveState " << value_oss.str() << std::endl;
  // Only OFF is valid
  if (value == 0) {
    setIntegerParam(parHeader_, (int) kHeaderBSWV);
  }
  else if (value == 1) {
    setIntegerParam(parHeader_, (int) kHeaderBTWV);
    
  }

  status = this->usbTmcWrite(value_oss.str());
  
  return status;
};




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


