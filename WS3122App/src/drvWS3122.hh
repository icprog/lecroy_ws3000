#ifndef INC_DRVWS3122_HH
#define INC_DRVWS3122_HH

/*
 * drvWS3122.hh
 *
 * Author: Jeong Han Lee
 *
 * Created Wednesday, February 28 21:20:54 CET 2018
 */

#include "asynPortDriver.h"
#include "asynOctetSyncIO.h"

#define MAX_BUF_SIZE            65536
#define TIMEOUT                 5

#define WS3122_IPN_Str 		"WS3122_IPN"            /* asynOctet      r  */
#define WS3122_FwVer_Str        "WS3122_FWVER"
#define WS3122_SwVer_Str        "WS3122_SWVER"


class drvWS3122 : public asynPortDriver {
 public:
  drvWS3122(const char *portName, const char *asynUSBTMCPortName);

  /* These are the methods that we override from asynPortDriver */
  virtual asynStatus writeInt32(asynUser *pasynUser, epicsInt32 value);
  virtual asynStatus readFloat64(asynUser *pasynUser, epicsFloat64 *value);


  //  void readTask(void);
 protected:
  /** Values used for pasynUser->reason, and indexes into the parameter library. */
  int  WS3122_IPN;
#define FIRST_WS3122_PARAM WS3122_IPN
  int  WS3122_FwVer;
  int  WS3122_SwVer;
#define LAST_WS3122_PARAM WS3122_SwVer

 private:

  epicsEventId eventId_;

  asynUser	*pasynUserDriver;

  asynStatus usbTMCPortWriteRead(char *sendBuffer, char *recvBuffer, int &recvBufSize, double timeout=TIMEOUT);
  asynStatus usbTMCPortRead(char *recvBuffer, int &recvBufSize, double timeout=TIMEOUT);
  //  asynStatus usbTMCPortWrite(double timeout);

  // asynStatus    GetData(asynUser *pasynUser, epicsFloat64 &value);
  // asynStatus    SetStartStop(asynUser *pasynUser, epicsInt32 value);
  
};

#define NUM_WS3122_PARAMS ((int)(&LAST_WS3122_PARAM - &FIRST_WS3122_PARAM + 1))

#endif 
