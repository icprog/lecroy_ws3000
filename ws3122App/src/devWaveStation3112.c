/*
 * WaveStation3112 device support
 
 Command Record Description
 0  -  SI      Read Gauge ID
*/

#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>

#include <dbScan.h>
#include <dbDefs.h>
#include <dbAccess.h>
#include <dbCommon.h>
#include <alarm.h>
#include <link.h>
#include <recGbl.h>
#include <recSup.h>
#include <devSup.h>
#include <epicsString.h>
#include <asynDriver.h>
#include <asynEpicsUtils.h>
#include <errlog.h>
#include <asynOctet.h>
#include <aiRecord.h>
#include <biRecord.h>
#include <mbbiRecord.h>
#include <stringinRecord.h>
#include <aoRecord.h>
#include <boRecord.h>
#include <mbboRecord.h>
#include <epicsExport.h>

#include "devWaveStation3112.h"

#define WS3122_BUFFER_SIZE 128
#define WS3122_TIMEOUT 2.0



typedef struct {
    int command;
    char *commandString;
} WS3122CommandStruct;

static  WS3122CommandStruct ws3122Commands[MAX_WS3122_COMMANDS] =
  {
   {GetID,         "GET_IDN"}
  };


typedef struct devWS3122Pvt {
    asynUser     *pasynUser;
    asynOctet    *pasynOctet;
    void         *octetPvt;
    asynStatus   status;
    char         recBuf[WS3122_BUFFER_SIZE];
    char         sendBuf[WS3122_BUFFER_SIZE];
    int          command;
    int		 address;
} devWS3122Pvt;

typedef struct dsetWS3122{
    long      number;
    DEVSUPFUN report;
    DEVSUPFUN init;
    DEVSUPFUN init_record;
    DEVSUPFUN get_ioint_info;
    DEVSUPFUN io;
    DEVSUPFUN convert;
} dsetWS3122;

static long initCommon(dbCommon *pr, DBLINK *plink);
static long startIOCommon(dbCommon *pr);
static void devWS3122Callback(asynUser *pasynUser);
static long WS3122Convert(dbCommon* pr,int pass);
static int buildCommand(devWS3122Pvt *pPvt, char *pvalue);

static long initAi(aiRecord *pr);
static long readAi(aiRecord *pr);

static long initBi(biRecord *pr);
static long readBi(biRecord *pr);

static long initMbbi(mbbiRecord *pr);
static long readMbbi(mbbiRecord *pr);

static long initSi(stringinRecord *pr);
static long readSi(stringinRecord *pr);

static long initAo(aoRecord *pr);
static long writeAo(aoRecord *pr);

static long initBo(boRecord *pr);
static long writeBo(boRecord *pr);

static long initMbbo(mbboRecord *pr);
static long writeMbbo(mbboRecord *pr);

static void rolfl(char *str, const char *toRemove);


dsetWS3122 devAiWS3122 = {6,0,0,initAi,0,readAi,WS3122Convert};
epicsExportAddress(dset,devAiWS3122);

dsetWS3122 devBiWS3122 = {6,0,0,initBi,0,readBi,0};
epicsExportAddress(dset,devBiWS3122);

dsetWS3122 devMbbiWS3122 = {6,0,0,initMbbi,0,readMbbi,0};
epicsExportAddress(dset,devMbbiWS3122);

dsetWS3122 devSiWS3122 = {6,0,0,initSi,0,readSi,0};
epicsExportAddress(dset,devSiWS3122);

dsetWS3122 devAoWS3122 = {6,0,0,initAo,0,writeAo,WS3122Convert};
epicsExportAddress(dset,devAoWS3122);

dsetWS3122 devBoWS3122 = {6,0,0,initBo,0,writeBo,0};
epicsExportAddress(dset,devBoWS3122);

dsetWS3122 devMbboWS3122 = {6,0,0,initMbbo,0,writeMbbo,0};
epicsExportAddress(dset,devMbboWS3122);



static long initCommon(dbCommon *pr, DBLINK *plink)
{
   char *port, *userParam;
   int i;
   int address;
   asynUser *pasynUser=NULL;
   asynStatus status;
   asynInterface *pasynInterface;
   devWS3122Pvt *pPvt=NULL;
   char command[30];
   char *pstring;

   /* Allocate private structure */
   pPvt = calloc(1, sizeof(devWS3122Pvt));

   /* Create an asynUser */
   pasynUser = pasynManager->createAsynUser(devWS3122Callback, 0);
   pasynUser->userPvt = pr;

   /* Parse link */
   status = pasynEpicsUtils->parseLink(pasynUser, plink,
                                       &port, &address, &userParam);
   if (status != asynSuccess) {
      errlogPrintf("devWS3122::initCommon %s bad link %s\n",
                   pr->name, pasynUser->errorMessage);
      goto bad;
   }

   status = pasynManager->connectDevice(pasynUser,port,0);
   if(status!=asynSuccess) goto bad;
   pasynInterface = pasynManager->findInterface(pasynUser,asynOctetType,1);
   if(!pasynInterface) goto bad;
   pPvt->pasynOctet = (asynOctet *)pasynInterface->pinterface;
   pPvt->octetPvt = pasynInterface->drvPvt;
   pPvt->pasynUser = pasynUser;
   pr->dpvt = pPvt;

   if ((userParam == NULL) || strlen(userParam) == 0) {
      errlogPrintf("devWS3122::initCommon %s invalid userParam %s\n",
                   pr->name, userParam);
      goto bad;
   }
   pPvt->address = address;
   sscanf(userParam,"%s",command);
   for (i=0; i<MAX_WS3122_COMMANDS; i++) {
        pstring = ws3122Commands[i].commandString;
        if (epicsStrCaseCmp(command, pstring) == 0) {
            pPvt->command = ws3122Commands[i].command;
            goto found;
        }
   }
   asynPrint(pasynUser, ASYN_TRACE_ERROR,
              "devWS3122::init_common %s, unknown command=%s\n", 
              pr->name, command);
   goto bad;

found:
   asynPrint(pasynUser, ASYN_TRACE_FLOW,
             "devWS3122::initCommon name=%s; command string=%s command=%d address=%d\n",
             pr->name, command, pPvt->command, pPvt->address);

   if (pPvt->command<0 || pPvt->command >MAX_WS3122_COMMANDS) {
       asynPrint(pasynUser, ASYN_TRACE_ERROR,
                 "devWS3122::initCommon %s illegal command=%d\n",
                 pr->name, pPvt->command);
       goto bad;
   }
   return 0;

bad:
   if(pasynUser) pasynManager->freeAsynUser(pasynUser);
   if(pPvt) free(pPvt);
   pr->pact = 1;
   return 0;
}

static long initAi(aiRecord *pr)
{
   return(initCommon((dbCommon *)pr, &pr->inp));
}

static long initBi(biRecord *pr)
{
   return(initCommon((dbCommon *)pr, &pr->inp));
}

static long initMbbi(mbbiRecord *pr)
{
   return(initCommon((dbCommon *)pr, &pr->inp));
}

static long initSi(stringinRecord *pr)
{
  return(initCommon((dbCommon *)pr, &pr->inp));
}

static long initAo(aoRecord *pr)
{
   return(initCommon((dbCommon *)pr, &pr->out));
}

static long initBo(boRecord *pr)
{
   return(initCommon((dbCommon *)pr, &pr->out));
}

static long initMbbo(mbboRecord *pr)
{
   return(initCommon((dbCommon *)pr, &pr->out));
}



static int buildCommand(devWS3122Pvt *pPvt, char *pvalue)
{
    asynUser *pasynUser = pPvt->pasynUser;
    dbCommon *pr = (dbCommon *)pasynUser->userPvt;
/*
    The WS3122 commands are of the form :  
    Send:  "*IDN?xx<CR><LF>" ( <LF> is optional )
    Reply: "<ACK><CR><LF>" or "<NAK><CR><LF>"
    Send:  "<ENQ>"
    Reply: "yyyy<CR><LF>"
    
    for all 
    XX = 3 letter command and [,data if any]
    yyy = response data for the request
*/

    

    memset(pPvt->sendBuf, 0, WS3122_BUFFER_SIZE);
    strcpy(pPvt->sendBuf, pvalue);
 


    asynPrint(pPvt->pasynUser, ASYN_TRACEIO_DEVICE,
              "devWS3122::buildCommand %s len=%zd string=|%s|\n",
              pr->name, strlen(pPvt->sendBuf), pPvt->sendBuf);

     return(0);
}

static long readAi(aiRecord *pr)
{
    /* devWS3122Pvt *pPvt = (devWS3122Pvt *)pr->dpvt; */
    /* char tempcmd[5]; */
    /* int stptNo; */
    /* char *pvalue = pPvt->recBuf; */
    /* float value=0; */
    /* float value2=0; */
    /* int status=0; */

    /* if (!pr->pact) { */
    /*     /\* For setpoint readback set the correct setpoint number. */
    /*      * setpoints 1 and 2 for gauge 1 and 3 and 4 for gauge 2*\/ */
    /*     switch (pPvt->command) { */
    /*       case GetPressure: */
    /*         sprintf(tempcmd, "PR%d", pPvt->address); */
    /*         break; */
    /*       case GetSpVal1: */
    /*         stptNo = (2* pPvt->address) - 1; */
    /*         sprintf(tempcmd,"SP%d",stptNo); */
    /*         break; */
    /*       case GetSpVal2: */
    /*         stptNo = 2* pPvt->address; */
    /*         sprintf(tempcmd,"SP%d",stptNo); */
    /*         break; */
    /*       default: */
    /*         asynPrint(pPvt->pasynUser, ASYN_TRACE_ERROR, */
    /*                   "devWS3122::readAi %s Wrong record type \n", */
    /*                   pr->name); */
    /*         break; */
    /*     } */
    /*     buildCommand(pPvt, tempcmd); */
    /*     return(startIOCommon((dbCommon *)pr)); */
    /* } */

    /* switch (pPvt->command) { */
    /*     case GetPressure: */
    /* 	    sscanf(pvalue, "%d,%E", &status,&value); */
    /* 	    if (status == 4) { */
    /*     	recGblSetSevr(pr, READ_ALARM, MINOR_ALARM); */
    /* 	    }  */
    /* 	    if ( status != 0 && status != 4) { */
    /*     	recGblSetSevr(pr, READ_ALARM, INVALID_ALARM); */
    /*     	asynPrint(pPvt->pasynUser, ASYN_TRACE_ERROR, */
    /*                  	"devWS3122::readAi Gauge error %s status =[%d] \n", */
    /*                   	pr->name, status); */
    /* 	    } */
    /* 	    break; */
    /*     case GetSpVal1: */
    /*     case GetSpVal2: */
    /* 	    sscanf(pvalue, "%d,%E,%E", &status,&value,&value2); */
    /*         break; */
    /*     default: */
    /*         asynPrint(pPvt->pasynUser, ASYN_TRACE_ERROR, */
    /*                   "devWS3122::readAi %s Wrong record type \n", */
    /*                   pr->name); */
    /*        break; */
    /* } */
    /* pr->val = value; */
    /* pr->udf=0; */
    return(2);
}


static long readBi(biRecord *pr)
{
    /* devWS3122Pvt *pPvt = (devWS3122Pvt *)pr->dpvt; */
    /* char tempcmd[5]; */
    /* int stptNo; */
    /* char *pvalue = pPvt->recBuf; */
    /* int vals[3]; */
    /* int value=0; */

    /* if (!pr->pact) { */
    /*     /\* For setpoint readback set the correct setpoint number. */
    /*      * setpoints 1 and 2 for gauge 1 and 3 and 4 for gauge 2*\/ */
    /*     switch (pPvt->command) { */
    /*       case GetSpS1: */
    /*       case GetSpS2: */
    /*         strcpy(tempcmd, "SPS"); */
    /*         break; */
    /*       case GetSensor: */
    /*         strcpy(tempcmd, "SEN"); */
    /*         break; */
    /*       default: */
    /*         asynPrint(pPvt->pasynUser, ASYN_TRACE_ERROR, */
    /*                   "devWS3122::readBi %s Wrong record type \n", */
    /*                   pr->name); */
    /*         break; */
    /*     } */
    /*     buildCommand(pPvt, tempcmd); */
    /*     return(startIOCommon((dbCommon *)pr)); */
    /* } */

    /* switch (pPvt->command) { */
    /*     case GetSpS1: */
    /*         stptNo = (2* pPvt->address) - 2; */
    /*         sscanf(pvalue,"%d,%d,%d,%d",&vals[0],&vals[1],&vals[2],&vals[3]); */
    /*         value = vals[stptNo]; */
    /*         break; */
    /*     case GetSpS2: */
    /*         stptNo = (2* pPvt->address) - 1; */
    /*         sscanf(pvalue,"%d,%d,%d,%d",&vals[0],&vals[1],&vals[2],&vals[3]); */
    /*         value = vals[stptNo]; */
    /*         break; */
    /*     case GetSensor: */
    /*         sscanf(pvalue,"%d,%d",&vals[0],&vals[1]); */
    /*         value = vals[pPvt->address - 1]; */
    /*         break; */
    /*     default: */
    /*         asynPrint(pPvt->pasynUser, ASYN_TRACE_ERROR, */
    /*                   "devWS3122::readBi %s Wrong record type \n", */
    /*                   pr->name); */
    /*         break; */
    /* } */
    /* pr->rval = value; */
    /* pr->udf=0; */
    return(0);
}

static long readMbbi(mbbiRecord *pr)
{
    /* devWS3122Pvt *pPvt = (devWS3122Pvt *)pr->dpvt; */
    /* char tempcmd[5]; */
    /* char *pvalue = pPvt->recBuf; */
    /* int value=0; */

    /* if (!pr->pact) { */
    /*     switch (pPvt->command) { */
    /*       case GetUnit: */
    /*         strcpy(tempcmd, "UNI"); */
    /*         break; */
    /*       default: */
    /*         asynPrint(pPvt->pasynUser, ASYN_TRACE_ERROR, */
    /*                   "devWS3122::readMbbi %s Wrong record type \n", */
    /*                   pr->name); */
    /*         break; */
    /*     } */
    /*     buildCommand(pPvt, tempcmd); */
    /*     return(startIOCommon((dbCommon *)pr)); */
    /* } */
    /* switch (pPvt->command) { */
    /*     case GetUnit: */
    /*         sscanf(pvalue,"%d",&value); */
    /*         break; */
    /*     default: */
    /*         asynPrint(pPvt->pasynUser, ASYN_TRACE_ERROR, */
    /*                   "devWS3122::readMbbi %s Wrong record type \n", */
    /*                   pr->name); */
    /*         break; */
    /* } */
    /* pr->rval = value; */
    /* pr->udf=0; */
    return(0);
}

static long
readSi(stringinRecord *pr)
{
  devWS3122Pvt *pPvt = (devWS3122Pvt *)pr->dpvt;
  int  rtnSize;
  char pvalue[WS3122_BUFFER_SIZE];
  char tempcmd[WS3122_BUFFER_SIZE];
   
  if (!pr->pact) {
    switch (pPvt->command) {
    case GetID:
      strcpy(tempcmd, "*IDN?");
      break;
    default:
      asynPrint(pPvt->pasynUser, ASYN_TRACE_ERROR, "devWS3122::readSi %s Wrong record type \n", pr->name);
      break;
    }
    
    buildCommand(pPvt, tempcmd);
    
    return(startIOCommon((dbCommon *)pr));
  }
      
  rtnSize = strlen(pPvt->recBuf);
  switch (pPvt->command) {
  case GetID:
    strcpy(pvalue, pPvt->recBuf);
    rolfl(pvalue, "*IDN WST,WaveStation 3122,");
    break;
  }
   
  strcpy(pr->val, pvalue);
 
 
  pr->udf=0;
  return(0);
}


static long writeAo(aoRecord *pr)
{
    /* devWS3122Pvt *pPvt = (devWS3122Pvt *)pr->dpvt; */
    /* char tempcmd[30]=""; */
    /* char pvalue[15]=""; */
    /* int stptNo; */
    /* int rtnSize; */
    
    /* if (!pr->pact) { */
    /*     /\* For setting setpoint the command is of the form :SPn,s,x.xxxxE-yy,x.xxxxE-yy */
    /*      * n - setpoint number 1-4. */
    /*      * s - 0 or 1 for measued channel 1 or 2 */
    /*      * values are for lower and upper and will be set the same as pr->val field. */
    /*     *\/ */

    /*     switch (pPvt->command) { */
    /*       case SetSp1: */
    /*         stptNo = (2* pPvt->address) - 1; */
    /*         sprintf(tempcmd,"SP%d,%d,",stptNo,(pPvt->address-1)); */
    /*         sprintf(pvalue, "%8.4E", pr->val); */
    /*         strcat(tempcmd, pvalue);  /\* for lower pressure *\/ */
    /*         strcat(tempcmd, ","); */
    /*         strcat(tempcmd, pvalue);  /\* for upper pressure *\/ */
    /*         break; */
    /*       case SetSp2: */
    /*         stptNo = (2* pPvt->address); */
    /*         sprintf(tempcmd,"SP%d,%d,",stptNo,(pPvt->address-1)); */
    /*         sprintf(pvalue, "%8.4E", pr->val); */
    /*         strcat(tempcmd, pvalue);  /\* for lower pressure *\/ */
    /*         strcat(tempcmd, ","); */
    /*         strcat(tempcmd, pvalue);  /\* for upper pressure *\/ */
    /*         break; */
    /*       default: */
    /*         asynPrint(pPvt->pasynUser, ASYN_TRACE_ERROR, */
    /*                   "devWS3122::writeAo %s Wrong record type \n", */
    /*                   pr->name); */
    /*         break; */
    /*     } */
    /*     buildCommand(pPvt, tempcmd); */
    /*     return(startIOCommon((dbCommon *)pr)); */
    /* } */
    /* rtnSize = strlen(pPvt->recBuf); */
    /* if (rtnSize < 1) { */
    /*     recGblSetSevr(pr, READ_ALARM, INVALID_ALARM); */
    /*     asynPrint(pPvt->pasynUser, ASYN_TRACE_ERROR,  */
    /*               "devWS3122::writeAo reply message error %s\n", */
    /*               pr->name); */
    /*     return(2); */
    /* } */
    /* pr->udf=0; */
    return(2);
}

static long writeBo(boRecord *pr)
{
    /* devWS3122Pvt *pPvt = (devWS3122Pvt *)pr->dpvt; */
    /* char tempcmd[10]=""; */
    /* int rtnSize; */
    
    /* if (!pr->pact) { */
    /*     switch (pPvt->command) { */
    /*       case SetSensor: */
    /* 	    strcpy(tempcmd,"SEN,"); */
    /* 	    if (pPvt->address == 1) { */
    /* 		sprintf(tempcmd,"SEN,%d,0",pr->val+1); */
    /* 	    } else { */
    /* 		sprintf(tempcmd,"SEN,0,%d",pr->val+1); */
    /* 	    } */
    /*         break; */
    /*       default: */
    /*         asynPrint(pPvt->pasynUser, ASYN_TRACE_ERROR, */
    /*                   "devWS3122::writeBo %s Wrong record type \n", */
    /*                   pr->name); */
    /*         break; */
    /*     } */
    /*     buildCommand(pPvt, tempcmd); */
    /*     return(startIOCommon((dbCommon *)pr)); */
    /* } */
    /* rtnSize = strlen(pPvt->recBuf); */
    /* if (rtnSize < 1) { */
    /*     recGblSetSevr(pr, READ_ALARM, INVALID_ALARM); */
    /*     asynPrint(pPvt->pasynUser, ASYN_TRACE_ERROR,  */
    /*               "devWS3122::writeBo reply message error %s\n", */
    /*               pr->name); */
    /*     return(0); */
    /* } */
    /* pr->udf=0; */
    return(0);
}

static long writeMbbo(mbboRecord *pr)
{
    /* devWS3122Pvt *pPvt = (devWS3122Pvt *)pr->dpvt; */
    /* char tempcmd[8]; */
    /* int rtnSize; */
    
    /* if (!pr->pact) { */
    /*     switch (pPvt->command) { */
    /*       case SetUnit: */
    /*         sprintf(tempcmd, "UNI,%d", pr->rval); */
    /*         break; */
    /*       default: */
    /*         asynPrint(pPvt->pasynUser, ASYN_TRACE_ERROR, */
    /*                   "devWS3122::writeMbbo %s Wrong record type \n", */
    /*                   pr->name); */
    /*         break; */
    /*     } */
    /*     buildCommand(pPvt, tempcmd); */
    /*     return(startIOCommon((dbCommon *)pr)); */
    /* } */
    /* rtnSize = strlen(pPvt->recBuf); */
    /* if (rtnSize < 1) { */
    /*     recGblSetSevr(pr, READ_ALARM, INVALID_ALARM); */
    /*     asynPrint(pPvt->pasynUser, ASYN_TRACE_ERROR,  */
    /*               "devWS3122::writeMbbo reply message error %s\n", */
    /*               pr->name); */
    /*     return(0); */

    /* } */
    /* pr->udf=0; */
    return(0);
}


static long
startIOCommon(dbCommon* pr)
{
   devWS3122Pvt *pPvt  = (devWS3122Pvt *)pr->dpvt;
   asynUser *pasynUser = pPvt->pasynUser;
   int          status;

   pr->pact = 1;
   status   = pasynManager->queueRequest(pasynUser, 0, 0);
   if (status != asynSuccess) status = -1;
   return(status);
}



static void
devWS3122Callback(asynUser *pasynUser)
{

  dbCommon *pr       = (dbCommon *)pasynUser->userPvt;
  devWS3122Pvt *pPvt = (devWS3122Pvt *)pr->dpvt;
  rset *prset        = (rset *)(pr->rset);

  char   readBuffer[WS3122_BUFFER_SIZE]; 
  size_t nread;
  size_t nwrite;
  int    eomReason;

  char inputEos[4];
  strcpy(inputEos,"\r");
   
  pPvt->status             = pPvt->pasynOctet->setInputEos(pPvt->octetPvt, pasynUser, inputEos, strlen(inputEos));
  pPvt->pasynUser->timeout = WS3122_TIMEOUT;

  pPvt->status             = pPvt->pasynOctet->write(pPvt->octetPvt,
						     pasynUser,
						     pPvt->sendBuf,
						     strlen(pPvt->sendBuf),
						     &nwrite);
    
  asynPrint(pasynUser, ASYN_TRACEIO_DEVICE, "devWS3122::devWS3122Callback Cmd %s nwrite=%zd, output=[%s]\n", pr->name, nwrite, pPvt->sendBuf);

  pPvt->status             = pPvt->pasynOctet->read(pPvt->octetPvt,
						    pasynUser,
						    readBuffer,
						    WS3122_BUFFER_SIZE,
						    &nread,
						    &eomReason);
  
  asynPrint(pasynUser, ASYN_TRACEIO_DEVICE, "devWS3122::devWS3122Callback Cmd %s nread=%zd, input=[%s]\n", pr->name, nread, readBuffer);

    
  if ( nread < 1 ) {
    asynPrint(pasynUser, ASYN_TRACE_ERROR, "devWS3122::devWS3122Callback Cmd %s message too small=%zd\n", pr->name, nread);
    recGblSetSevr(pr, READ_ALARM, INVALID_ALARM);
  }
  
  memset(pPvt->recBuf, 0, WS3122_BUFFER_SIZE);
  strcpy(pPvt->recBuf, readBuffer);
  asynPrint(pasynUser, ASYN_TRACEIO_DEVICE, "devWS3122: %s command (%d) received (after processing) |%s|\n", pr->name, pPvt->command, pPvt->recBuf);
  
  dbScanLock((dbCommon*)pr);
  (*prset->process)(pr);
  dbScanUnlock((dbCommon*)pr);
    
}


static long WS3122Convert(dbCommon* pr,int pass)
{
  /* aiRecord* pai = (aiRecord*)pr; */
  /* pai->eslo=1.0; */
  /* pai->roff=0; */
  return 0;
}



// https://codereview.stackexchange.com/questions/67055/fastest-way-of-removing-a-substring-from-a-string
static void rolfl(char *str, const char *toRemove)
{
  if (NULL == (str = strstr(str, toRemove)))
    {
      return;
    }

  const size_t remLen = strlen(toRemove);
  char *copyEnd;
  char *copyFrom = str + remLen;
  while (NULL != (copyEnd = strstr(copyFrom, toRemove)))
    {
      memmove(str, copyFrom, copyEnd - copyFrom);
      str += copyEnd - copyFrom;
      copyFrom = copyEnd + remLen;
    }
  memmove(str, copyFrom, 1 + strlen(copyFrom));
};

