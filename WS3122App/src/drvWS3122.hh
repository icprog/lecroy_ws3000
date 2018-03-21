#ifndef INC_DRVWS3122_HH
#define INC_DRVWS3122_HH

/*
 * drvWS3122.hh
 *
 * Author: Jeong Han Lee
 *
 * Created Wednesday, February 28 21:20:54 CET 2018
 */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <math.h>

#include <iostream>
#include <iomanip>
#include <sstream>
#include <cstdio>
#include <string>
#include <vector>



#include <asynPortDriver.h>
#include <asynOctetSyncIO.h>


#include "WS3122Def.hh"
#include "WSTypes.hh"
#include "snipDrvAsynUSBTMC.hh"

#define MAX_BUF_SIZE            65536
#define TIMEOUT                 2

#define MAX_ASYN_ADDRESS        0
#define MAX_BUFFER_SIZE	        512

#define DevIDNString                 "DEV_IDN"                   // asynOctet      w  
#define DevManufacturerString        "DEV_MANUFACTURER"          // asynOctet,    r/o manufacturer name 
#define DevModelString               "DEV_MODEL"                 // asynOctet,    r/o model name 
#define DevSerialNumberString        "DEV_SERIAL_NUMBER"         // asynOctet,    r/o serial number 



#define ParHeaderPathString          "PAR_HEADER_PATH"              // asynParamInt32
#define ParHeaderString              "PAR_HEADER"                   // asynParamInt32
#define ParBasicWaveTypeSelecString  "BASIC_WAVE_TYPE_SELECT"

#define ParWaveFrequencyString       "PAR_WAVE_FREQUENCY"           //asynFloat64
#define ParWaveAmplifierString       "PAR_WAVE_AMPLIFIER"           //asynFloat64
#define ParWaveOffsetString          "PAR_WAVE_OFFSET"              //asynFloat64
#define ParWavePhaseString           "PAR_WAVE_PHASE"               //asynFloat64
#define ParWaveWidthString           "PAR_WAVE_WIDTH"               //asynFloat64
#define ParWaveRiseString            "PAR_WAVE_RISE"                //asynFloat64
#define ParWaveFallString            "PAR_WAVE_FALL"                //asynFloat64
#define ParWaveDelayString           "PAR_WAVE_DELAY"               //asynFloat64
#define ParWaveSymmetryString        "PAR_WAVE_SYMMETRY"            //asynFloat64
#define ParWaveStdDevString          "PAR_WAVE_STDDEV"              //asynFloat64
#define ParWaveMeanString            "PAR_WAVE_MEAN"                //asynFloat64
#define ParWaveDutyCycleString       "PAR_WAVE_DUTYCYCLE"           //asynFloat64


#define ParBurstGateNcycString       "PAR_BURST_GATE_NCYC"          // asynParamInt32
#define ParBurstTriggerSrcString     "PAR_BURST_TRSR"               // asynParamInt32
#define ParBurstTriggerModeString    "PAR_BURST_TRMD"               // asynParamInt32
#define ParBurstPolarityString       "PAR_BURST_PLRT"               // asynParamInt32
#define ParBurstEdgeString           "PAR_BURST_EDGE"               // asynParamInt32

#define ParBurstManualTriggerString  "PAR_BURST_MTRIG"              // asynParamInt32

#define ParBurstPeriodString         "PAR_BURST_PERIOD"             //asynFloat64
#define ParBurstStartPhaseString     "PAR_BURST_START_PHASE"        //asynFloat64
#define ParBurstDelayString          "PAR_BURST_DELAY"              //asynFloat64
#define ParBurstCycleTimeString      "PAR_BURST_CYCLE_TIME"         //asynFloat64




#define CmdWaveStateString           "CMD_WAVE_STATE"               // asynParamInt32
#define CmdOutputString              "CMD_OUTPUT"
#define CmdOutputLoadString          "CMD_OUTPUT_LOAD"
#define CmdOutputPolarityString      "CMD_OUTPUT_POLARITY"
#define CmdSetWaveTypeString         "CMD_SET_WAVE_TYPE"
#define CmdPhaseInvertString         "CMD_PHASE_INVERT"             // asynParamInt32
#define CmdScreenSaveString          "CMD_SCREEN_SAVE"
#define CmdClockSourceString         "CMD_CLOCK_SOURCE"             // asynParamInt32


class drvWS3122 : public asynPortDriver {
public:
  drvWS3122(const char *portName, const char *asynUSBTMCPortName);
  virtual ~drvWS3122();


  //virtual asynStatus writeInt32(asynUser *pasynUser, epicsInt32 value);
  //virtual asynStatus readOctet (asynUser *pasynUser, char *value, size_t maxChars, size_t *nActual, int *eomReason);
  virtual asynStatus writeInt32(asynUser *pasynUser, epicsInt32 value);
  virtual asynStatus writeFloat64(asynUser *pasynUser, epicsFloat64 value);
  virtual asynStatus writeOctet(asynUser *pasynUser, const char *value, size_t 	nChars, size_t *nActual);
  
  friend std::ostream& operator<<(std::ostream& os, const drvWS3122 &ws);

protected:

  /** Values used for pasynUser->reason, and indexes into the parameter library. */
  int devIdentification_;
#define FIRST_PARAM devIdentification_
  int devManufacturer_;
  int devModel_;
  int devSerialNumber_;

  int parHeaderPath_;
  int parHeader_;
  int parBasicWaveTypeSelect_;
  
  int parWaveFrequency_;
  int parWaveAmplifier_;
  int parWaveOffset_;
  int parWavePhase_;
  int parWaveWidth_;
  int parWaveRise_;
  int parWaveFall_;
  int parWaveDelay_;
  int parWaveSymmetry_;
  int parWaveStdDev_;
  int parWaveMean_;
  int parWaveDutyCycle_;


  int parBurstGateNcyc_;

  int parBurstTriggerSrc_;
  int parBurstTriggerMode_;
  int parBurstEdge_;
  int parBurstPolatiry_;
  
  
  int parBurstManualTrigger_;
  int parBurstPeriod_;
  int parBurstStartPhase_;
  int parBurstDelay_;
  int parBurstCycleTime_;

  int cmdWaveState_;
  int cmdOutput_;
  int cmdOutputLoad_;
  int cmdOutputPolarity_;
  int cmdSetWaveType_;
  int cmdPhaseInvert_;
  int cmdScreenSave_;
  int cmdClockSource_;
#define LAST_PARAM cmdClockSource_

  
private:


  asynStatus Init();
  
  asynUser	*usbTmcAsynUser;
  std::string    asynUsbTmcPortName;
  
  const char* getUsbTmcPortName() const { return asynUsbTmcPortName.c_str();};
  asynStatus  usbTmcWriteRead(char *sendBuffer, char *recvBuffer, int &recvBufSize, double timeout=TIMEOUT);
  asynStatus  usbTmcWrite(std::string sendBuffer, double timeout=TIMEOUT);
  //asynStatus usbTmcRead(double timeout=TIMEOUT);
  //asynStatus usbTmcWrite(double timeout=TIMEOUT);

  asynStatus write_read     (std::string cmd);
  asynStatus SetClockSource (epicsInt32 value);
  asynStatus SetPhaseInvert (epicsInt32 value);
  asynStatus SetScreenSave  (epicsInt32 value);
  asynStatus SetWaveTypeCmds(epicsInt32 value);
  asynStatus SetOutput      (epicsInt32 value, EOutputParameter_t id);

  
  asynInterface *pasynInterface;
  drvPvt        *pasynDrvPvt;

  BasicWave  *basicWave;
  BurstWave  *burstWave;
  
  asynStatus report_device_information(FILE *fp);
  asynStatus set_device_information();
  asynStatus set_wave_parameters(int function, epicsInt32 iValue, epicsFloat64 fValue, std::string value_s, asynParamType paramType);

  void  set_burst_wave_flags();
  
};

#define NUM_PARAMS ((int)(&LAST_PARAM - &FIRST_PARAM + 1))

#endif 
