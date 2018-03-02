#ifndef INC_WS3122Def_HH
#define INC_WS3122Def_HH


#include <iostream>
#include <iomanip>
#include <sstream>
#include <cstdio>
#include <string>
#include <vector>

#define NUM_HEADER_PATH         10
#define NUM_HEADER              26
#define NUM_BSWV_WAVETYPE_NUM    7
#define NUM_BSWV_PARAMS         13
#define NUM_BTWV_PARAMS         22
//#define NUM_MDWV_PARAMS         35
#define NUM_OUTP_PARAMS          4
//#define NUM_SWWV_PARAMS         12

#define MAX_ENUM_STRING_SIZE 26


typedef struct {
    int value;
    char string[MAX_ENUM_STRING_SIZE];
} enumStruct_t;



static const char * headerPathStrings[NUM_HEADER_PATH] =
  {
   "C1"
   ,"C2"
   ,"M1"
   ,"M2"
   ,"M3"
   /* */
   ,"M4"
   ,"M5"
   ,"M6"
   ,"M7"
   ,"M8"
  };
							  
// enum Header
//   {
//    HEADER_IDN,
//    HEADER_CLS,
//    HEADER_ESE,
//    HEADER_ESR,
//    HEADER_RET,
//    HEADER_SRE,
//    HEADER_STB,
//    HEADER_TST,
//    HEADER_WAI,
//    HEADER_ARWV,
//    HEADER_BSWV,
//    HEADER_BTWV,
//    HEADER_BUZZ,
//    HEADER_CHDR,
//    HEADER_INVT,
//    HEADER_MDWV,
//    HEADER_OUTP,
//    HEADER_PACP,
//    HEADER_ROSC,
//    HEADER_SCFG,
//    HEADER_SCSV,
//    HEADER_STL,
//    HEADER_SWWV,
//    HEADER_SYNC,
//    HEADER_VKEY,
//    HEADER_WVCSV
//   };

static const char * headerStrings[NUM_HEADER] =
  {
   "*IDN"
   , "*CLS"
   , "*ESE"
   , "*ESR"
   , "*RET"
   /* */
   , "*SRE"
   , "*STB"
   , "*TST"
   , "*WAI"
   , "ARWV"
   /* */
   , "BSWV"
   , "BTWV"
   , "BUZZ"
   , "CHDR"
   , "INVT"
   /* */
   , "MDWV"
   , "OUTP"
   , "PACP"
   , "ROSC"
   , "SCFG"
   /* */
   , "SCSV"
   , "STL"
   , "SWWV"
   , "SYNC"
   , "VKEY"
   /* */
   , "WVCSV"
  };

 
enum BSWVType
  {
   BSWV_SINE,
   BSWV_SQUARE,
   BSWV_RAMP,
   BSWV_PULSE,
   BSWV_NOISE,
   BSWV_ARB,
   BSWV_DC
  };


static const char *bswv_waveTypeStrings[NUM_BSWV_WAVETYPE_NUM] =
  {
   "SINE",
   "SQUARE",
   "RAMP",
   "PULSE",
   "NOISE",
   "ARB",
   "DC"	      
  };

static const int convertbswvWaveTypeValues[NUM_BSWV_WAVETYPE_NUM] =
  {
   BSWV_SINE,
   BSWV_SQUARE,
   BSWV_RAMP,
   BSWV_PULSE,
   BSWV_NOISE,
   BSWV_ARB,
   BSWV_DC
};

  

enum BSWVParameter
  {
   BSWVPARM_WVTP,
   BSWVPARM_FRQ,
   BSWVPARM_AMP,
   BSWVPARM_OFST,
   BSWVPARM_SYM,
   BSWVPARM_DUTY,
   BSWVPARM_PHSE,
   BSWVPARM_STDEV,
   BSWVPARM_MEAN,
   BSWVPARM_WIDTH,
   BSWVPARM_RISE,
   BSWVPARM_FALL,
   BSWVPARM_DLY
  };
  
  
static const char * bswv_parametersStrings[NUM_BSWV_PARAMS] =
  {
   "WVTP"
   , "FRQ"
   , "AMP"
   , "OFST"
   , "SYM"
   /* */
   , "DUTY"
   , "PHSE"
   , "STDEV"
   , "MEAN"
   , "WIDTH"
   /* */
   , "RISE"
   , "FALL"
   , "DLY"
  };


static const char * btwv_parametersStrings[NUM_BTWV_PARAMS] =
  {
   "STATE"
   , "PRD"
   , "STPS"
   , "GATE_SYNC"
   , "TRSR"
   /* */
   , "DLAY"
   , "PLRT"
   , "TRMD"
   , "EDGE"
   , "TIME"
   /* */
   , "CARR,WVTP"
   , "CARR,FRQ"
   , "CARR,AMP"
   , "CARR,OFST"
   , "CARR,SYM"
   /* */
   , "CARR,DUTY"
   , "CARR,PHSE"
   , "CARR,RISE"
   , "CARR,FALL"
   , "CARR,DLY"
   /* */
   , "CARR,STDEV"
   , "CARR,MEAN"
  };


// static const char * mdwv_parametersStrings[NUM_MDWV_PARAMS] =
//   {
//    "STATE"
//    , "AM,SRC"
//    , "AM,MDSP"
//    , "AM,FRQ"
//    , "AM,DEPTH"
//    /* */
//    , "DSBAM,SRC"
//    , "DSBAM,MDSP"
//    , "DSBAM,FRQ"
//    , "FM,SRC"
//    , "FM,MDSP"
//    /* */
//    , "FM,FRQ"
//    , "FM,DEVI"
//    , "PM,SRC"
//    , "PM,MDSP"
//    , "PM,FRQ"
//    /* */
//    , "PM,DEVI"
//    , "PWM,FRQ"
//    , "PWM,DEVI"
//    , "PWM,MDSP"
//    , "PWM,SRC"
//    /* */
//    , "ASK,SRC"
//    , "ASK,KFRQ"
//    , "FSK,KFRQ"
//    , "FSK,HFRQ"
//    , "FSK,SRC"
//    /* */
//    , "CARR,WVTP"
//    , "CARR,FRQ"
//    , "CARR,AMP"
//    , "CARR,OFST"
//    , "CARR,SYM"
//    /* */
//    , "CARR,DUTY"
//    , "CARR,PHSE"
//    , "CARR,RISE"
//    , "CARR,FALL"
//    , "CARR,DLY"
//      /* */
//   };


static const char *outp_parametersStrings[NUM_OUTP_PARAMS] =
  {
   "ON"
   ,"OFF"
   ,"LOAD"
   ,"PLRT"
  };

// static const char *swwv_parametersStrings[NUM_SWWV_PARAMS] =
//   {
//    "STATE"
//    , "TIME"
//    , "STOP"
//    , "START"
//    , "TRSR"
//    /* */
//    , "TRMD"
//    , "SWMD"
//    , "DIR"
//    , "EDGE"
//    , "MTRIG"
//    /* */
//    , "CARR,WVTP"
//    , "CARR,FRQ"
//    , "CARR,AMP"
//    , "CARR,OFST"
//    , "CARR,SYM"
//    /* */
//    , "CARR,DUTY"
//    , "CARR,PHSE"
//   };




static const unsigned int maxStringLength = 512;
static const unsigned int maxUnitStringLength = 10;

class BasicWave
{

public:
  BasicWave();
  BasicWave(const BasicWave& bwv);
  virtual ~BasicWave();

  friend std::ostream& operator<<(std::ostream& os, const BasicWave &bwv);
  
  BasicWave & operator=(const BasicWave &bwv);
  
  void Init();

  const std::string     getFullCommand()             { return fullCommandStream.str();};
  const char*           getCharFullCommand()         { return fullCommandStream.str().c_str();};
  
  const unsigned int    getChannelID()               { return channelID; };
  const unsigned int    getWaveTypeID()              { return waveTypeID; };
  
  const long            getFrequencyVal()            { return frequencyVal; };
  const long            getAmplifierVal()            { return amplifierVal; };
  const long            getOffsetVal()               { return offsetVal; };
  
  const unsigned int    getSymmetryVal()             { return symmetryVal; };
  
  const long            getDutyCycleVal()            { return dutyCycleVal; };
  
  const unsigned int    getPhaseVal()                { return phaseVal; };
  const long            getStandardDeviationVal()    { return standardDeviationVal; };
  const long            getMeanVal()                 { return meanVal; };
  
  const long            getWidthVal()                { return widthVal; };
  const long            getRiseVal()                 { return riseVal; };
  const long            getFallVall()                { return fallVal; };
  const long            getDelayVal()                { return delayVal; };
  

  void    setChannelID(unsigned int id)        { channelID = id; };
  void    setWaveTypeID(unsigned int id)       { waveTypeID = id; };
  
  void    setFrequencyVal(long val)            { frequencyVal = val; };
  void    setAmplifierVal(long val)            { amplifierVal = val; };
  void    setOffsetVal(long val)               { offsetVal = val; };
  
  void    setSymmetryVal(unsigned int val)     { symmetryVal = val; };
  
  void    setDutyCycleVal(long val)            { dutyCycleVal = val; };
  
  void    setPhaseVal(unsigned int val)        { phaseVal = val; };
  void    setStandardDeviationVal(long val)    { standardDeviationVal = val; };
  void    setMeanVal(long val)                 { meanVal = val; };
  
  void    setWidthVal(long val)                { widthVal = val; };
  void    setRiseVal(long val)                 { riseVal = val; };
  void    setFallVall(long val)                { fallVal = val; };
  void    setDelayVal(long val)                { delayVal = val; };
  
  
private:

  unsigned int        channelID;
  unsigned int        waveTypeID;           //SINE, SQUARE, RAMP, PULSE, NOISE, ARB, DC


  
  long                frequencyVal;         //SINE[O], SQUARE[O], RAMP[O], PULSE[O], NOISE[X], ARB[O], DC[O]
  long                amplifierVal;         //SINE[O], SQUARE[O], RAMP[O], PULSE[O], NOISE[X], ARB[O], DC[O]
  long                offsetVal;            //SINE[O], SQUARE[O], RAMP[O], PULSE[O], NOISE[X], ARB[O], DC[O]
  
  unsigned int        symmetryVal;          //SINE[X], SQUARE[X], RAMP[O], PULSE[X], NOISE[X], ARB[X], DC[X]
  
  long                dutyCycleVal;         //SINE[X], SQUARE[O], RAMP[X], PULSE[O], NOISE[X], ARB[X], DC[X]
  
  unsigned int        phaseVal;             //SINE[X], SQUARE[X], RAMP[X], PULSE[X], NOISE[O], ARB[X], DC[X]       
  long                standardDeviationVal; //SINE[X], SQUARE[X], RAMP[X], PULSE[X], NOISE[O], ARB[X], DC[X]
  long                meanVal;              //SINE[X], SQUARE[X], RAMP[X], PULSE[X], NOISE[O], ARB[X], DC[X]
  
  long                widthVal;             //SINE[X], SQUARE[X], RAMP[X], PULSE[O], NOISE[X], ARB[X], DC[X]
  long                riseVal;              //SINE[X], SQUARE[X], RAMP[X], PULSE[O], NOISE[X], ARB[X], DC[X]
  long                fallVal;              //SINE[X], SQUARE[X], RAMP[X], PULSE[O], NOISE[X], ARB[X], DC[X]
  long                delayVal;             //SINE[X], SQUARE[X], RAMP[X], PULSE[O], NOISE[X], ARB[X], DC[X]
  

  std::string waveTypeString;
  std::string frequencyUnit;  // Hz, [1e-6, depends on the model]
  std::string amplifierUnit;  // V   [2mVpp, 20Vpp] for WS3000
  std::string offsetUnit;  // V   [, depends on the model]
  std::string dutyCycleUnit;  // percent if WVTP SQUARE, [20,80], if WVTP PULSE [0.1,99.9]
  //  char standardDeviationUnit [maxUnitStringLength]; // V [0.5m, 1.599] valid only when WVTP NOISE
  
  std::ostringstream fullCommandStream;

};


#endif

