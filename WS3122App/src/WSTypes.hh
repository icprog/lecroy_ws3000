#ifndef WSTYPES_HH
#define WSTYPES_HH

#include <map>
#include <iostream>
#include <string>

#define MAX_ENUM_STRING_SIZE 26


// We only have the header supports on C1,C2 and upto M5.
// It must be the same order in the db file also. 
typedef enum {
	      kHeaderPathC1,
	      kHeaderPathC2,
     	      kHeaderPathM1,
      	      kHeaderPathM2,
      	      kHeaderPathM3,
      	      kHeaderPathM4,
      	      kHeaderPathM5,
	      kHeaderPathUnknown
} EHeaderPath_t;

// Selected few headers from the original header definitions from the manual,
// In the narrow definition, the header in this source is defined when
// the header path is defined. However, we only use WAVE type headers only now.
// 
// ARWV : Arbitrary Wave Command
// BSWV : Basic Wave Command
// BTWV : Burst Wave Command
// MDWV : Modulate Wave Command
// SWWV : Sweep Wave Command

typedef enum {
	      kHeaderBSWV
	      , kHeaderBTWV
	      , kHeaderARWV
	      , kHeaderMDWV
	      , kHeaderSWWV
	      //	      , kHeaderSYNC
	      //              , kHeaderOUTP
	      // , kHeaderUnknown
} EHeaderType_t;




// The following wave types are used in
// BSWV and CARR in BTWV, SWWV, and MDWV

typedef enum {
	      kWaveTypeSine,
	      kWaveTypeSquare,
	      kWaveTypeRamp,
	      kWaveTypePulse,
	      kWaveTypeNoise,
	      kWaveTypeArb,
	      kWaveTypeDc,
	      kWaveTypeUnknown
} EBasicWaveType_t;



typedef enum {
	      kWaveFreq,
	      kWaveAmpl,
	      kWaveOffset,
	      kWavePhase,
	      kWaveWidth,
	      kWaveRise,
	      kWaveFall,
	      kWaveDelay,
	      kWaveSymm,
	      kWaveStdDev,
	      kWaveMean,
	      kWaveDuty,
	      kWaveUnknown
} EWaveParameter_t;


typedef enum {
	      kCmdSymbolColon,
	      kCmdSymbolQuestion,
	      kCmdSymbolBlank,
	      kCmdSymbolComma
} ECmdSymbol_t;



typedef enum {
	      kOutputOnOff,
	      kOutputLoad,
	      kOutputPolarity,
	      kOutputUnknown
} EOutputParameter_t;




typedef enum {
	      kWaveStateOff,
	      kWaveStateOn
} EWaveState_t;


typedef enum {
	      kBurstModeGate,
	      kBurstModeNCycle,
	      kBurstModeUnknown
} EBurstMode_t;


typedef enum {
	      kTriggerSrcExternal,
	      kTriggerSrcInternal,
	      kTriggerSrcManual,
	      kTriggerSrcUnknown
} ETriggerSrc_t;


typedef enum {
	      kTriggerModeRise,
	      kTriggerModeFall,
	      kTriggerModeOff
} ETriggerMode_t;



typedef enum {
	      kEdgeModeRise,
	      kEdgeModeFall
} EEdgeModeMap_t;


typedef enum {
	      kPolarityNegative,
	      kPolarityPositive
} EPolarityMap_t;

typedef enum {
	      kBurstParPeriod,
	      kBurstParStartPhase,
	      kBurstParGateNcyc,
	      kBurstParTriggerSrc,
	      kBurstParDelay,
	      kBurstParPolarity,
	      kBurstParTriggerMode,
	      kBurstParEdge,
	      kBurstParTime,
	      kBurstParMtrig
} EBurstParameter_t;



typedef std::map < EHeaderPath_t,      std::string > HeaderPathMap;
typedef std::map < EHeaderType_t,      std::string > HeaderTypeMap; 
typedef std::map < EBasicWaveType_t,   std::string > BasicWaveMap;
typedef std::map < EWaveParameter_t,   std::string > WaveParameterMap;
typedef std::map < ECmdSymbol_t,       std::string > CmdSymbolMap;
typedef std::map < EOutputParameter_t, std::string > OutputParameterMap;

typedef std::map < EWaveState_t,       std::string > WaveStateMap;

typedef std::map < EBurstMode_t,       std::string > BurstModeMap;
typedef std::map < ETriggerSrc_t,      std::string > TriggerSrcMap;
typedef std::map < ETriggerMode_t,     std::string > TriggerModeMap;
typedef std::map < EEdgeModeMap_t,     std::string > EdgeModeMap;
typedef std::map < EPolarityMap_t,     std::string > PolarityMap;
typedef std::map < EBurstParameter_t,  std::string > BurstParameterMap;



inline HeaderPathMap CreateHeaderPathMap()
{
  HeaderPathMap map;
  map[kHeaderPathC1]    = "C1";
  map[kHeaderPathC2]    = "C2";
  map[kHeaderPathM1]    = "M1";
  map[kHeaderPathM2]    = "M2";
  map[kHeaderPathM3]    = "M3";
  map[kHeaderPathM4]    = "M4";
  map[kHeaderPathM5]    = "M5";
  map[kHeaderPathUnknown] = "UNKNOWN";
  return map;
  
};

inline HeaderTypeMap CreateHeaderMap()
{
  HeaderTypeMap map;
  map[kHeaderBSWV]    = "BSWV";
  map[kHeaderBTWV]    = "BTWV";
  map[kHeaderMDWV]    = "MDWV";
  map[kHeaderSWWV]    = "SWWV";
  map[kHeaderARWV]    = "ARWV";
  // map[kHeaderSYNC]    = "SYNC";
  // map[kHeaderOUTP]    = "OUTP";
  // map[kHeaderUnknown] = "UNKNOWN";
  return map;
  
};


inline BasicWaveMap CreateBasicWaveMap()
{
  BasicWaveMap map;
  map[kWaveTypeSine]    = "SINE";
  map[kWaveTypeSquare]  = "SQUARE";
  map[kWaveTypeRamp]    = "RAMP";
  map[kWaveTypePulse]   = "PULSE";
  map[kWaveTypeNoise]   = "NOISE";
  map[kWaveTypeArb]     = "ARB";
  map[kWaveTypeDc]      = "DC";
  map[kWaveTypeUnknown] = "UNKNOWN";
  return map;
  
};

inline WaveParameterMap CreateWaveParameterMap()
{
  WaveParameterMap map;
  map[kWaveFreq]    = "FRQ";
  map[kWaveAmpl]    = "AMP";
  map[kWaveOffset]  = "OFST";
  map[kWavePhase]   = "PHSE";
  map[kWaveWidth]   = "WIDTH";
  map[kWaveRise]    = "RISE";
  map[kWaveFall]    = "FALL";
  map[kWaveDelay]   = "DLY";
  map[kWaveSymm]    = "SYM";
  map[kWaveStdDev]  = "STDEV";
  map[kWaveMean]    = "MEAN";
  map[kWaveDuty]    = "DUTY";
  map[kWaveUnknown] = "UNKNOWN";
  return map;
  
};



inline CmdSymbolMap CreateCmdSymbolMap()
{
  
  CmdSymbolMap map;
  map[kCmdSymbolColon]    = ":";
  map[kCmdSymbolQuestion] = "?";
  map[kCmdSymbolBlank]    = " ";
  map[kCmdSymbolComma]    = ",";
  return map;
  
};



inline OutputParameterMap CreateOutputParameterMap()
{
  
  OutputParameterMap map;
  map[kOutputOnOff]    = "";
  map[kOutputLoad]     = "LOAD";
  map[kOutputPolarity] = "PLRT";
  map[kOutputUnknown]  = "UNKNOWN";
  return map;
  
};



inline WaveStateMap CreateWaveStateMap()
{
  WaveStateMap map;
  map[kWaveStateOn]  = "ON";
  map[kWaveStateOff] = "OFF";
  return map;
};



inline BurstModeMap CreateBurstModeMap()
{
  BurstModeMap map;
  map[kBurstModeGate]   = "GATE";
  map[kBurstModeNCycle] = "NCYC";
  map[kBurstModeUnknown] = "UNKNOWN";
  return map;
};




// BTWV, SWWV
inline TriggerSrcMap CreateTriggerSrcMap()
{
  TriggerSrcMap map;
  map[kTriggerSrcExternal] = "EXT";
  map[kTriggerSrcInternal] = "INT";
  map[kTriggerSrcManual]   = "MAN";
  map[kTriggerSrcUnknown]  = "UNKNOWN";
  return map;
};


inline TriggerModeMap CreateTriggerModeMap()
{
  TriggerModeMap map;
  map[kTriggerModeRise] = "RISE";
  map[kTriggerModeFall] = "FALL";
  map[kTriggerModeOff]  = "OFF";
  return map;
};



inline EdgeModeMap CreateEdgeModeMap()
{
  EdgeModeMap map;
  map[kEdgeModeRise] = "RISE";
  map[kEdgeModeFall] = "FALL";
  return map;
};


inline PolarityMap CreatePolarityMap()
{
  PolarityMap map;
  map[kPolarityNegative] = "NEG";
  map[kPolarityPositive] = "POS";
  return map;
};

inline BurstParameterMap CreateBurstParameterMap()
{
  BurstParameterMap map;
  map[kBurstParPeriod]      = "PRD";
  map[kBurstParStartPhase]  = "STPS";
  map[kBurstParGateNcyc]    = "GATE_NCYC";
  map[kBurstParTriggerSrc]  = "TRSR";
  map[kBurstParDelay]       = "DLAY";
  map[kBurstParPolarity]    = "PLRT";
  map[kBurstParTriggerMode] = "TRMD";
  map[kBurstParEdge]        = "EDGE";
  map[kBurstParTime]        = "TIME";
  map[kBurstParMtrig]       = "MTRIG";
  return map;
};


EBasicWaveType_t GetBasicWaveType (std::string       name);
EWaveParameter_t GetWaveParmeter  (std::string       name);

std::string      GetHeaderPath    (EHeaderPath_t     type);
std::string      GetHeaderType    (EHeaderType_t     type);
std::string      GetBasicWaveType (EBasicWaveType_t  type);
//std::string      GetWaveParameter (EWaveParameter_t  type);
std::string      GetCmdSymbol     (ECmdSymbol_t      type);

std::string      GetWaveState     (EWaveState_t      type);

std::string      GetBurstMode     (EBurstMode_t      type);
std::string      GetTriggerSrc    (ETriggerSrc_t     type);
std::string      GetTriggerMode   (ETriggerMode_t    type);
std::string      GetEdgeMode      (EEdgeModeMap_t    type);
std::string      GetPolarity      (EPolarityMap_t    type);
std::string      GetBurstParameter(EBurstParameter_t type);

#endif
