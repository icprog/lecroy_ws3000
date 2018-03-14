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
	      kHeaderARWV
	      , kHeaderBSWV
	      , kHeaderBTWV
	      , kHeaderMDWV
	      , kHeaderSWWV
	      //	      , kHeaderSYNC
	      //              , kHeaderOUTP
	      //	      , kHeaderUnknown
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
	      kOutputOnOff,
	      kOutputLoad,
	      kOutputPolarity,
	      kOutputUnknown
} EOutputParameter_t;



typedef enum {
	      kCmdSymbolColon,
	      kCmdSymbolQuestion,
	      kCmdSymbolBlank,
	      kCmdSymbolComma
} ECmdSymbol_t;



typedef std::map < EHeaderPath_t,      std::string > HeaderPathMap;
typedef std::map < EHeaderType_t,      std::string > HeaderTypeMap; 
typedef std::map < EBasicWaveType_t,   std::string > BasicWaveMap;
typedef std::map < EWaveParameter_t,   std::string > WaveParameterMap;
typedef std::map < EOutputParameter_t, std::string > OutputParameterMap;
typedef std::map < ECmdSymbol_t,       std::string > CmdSymbolMap;


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
  map[kHeaderARWV]    = "ARWV";
  map[kHeaderBSWV]    = "BSWV";
  map[kHeaderBTWV]    = "BTWV";
  map[kHeaderMDWV]    = "MDWV";
  map[kHeaderSWWV]    = "SWWV";
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


inline OutputParameterMap CreateOutputParameterMap()
{
  
  OutputParameterMap map;
  map[kOutputOnOff]    = "";
  map[kOutputLoad]     = "LOAD";
  map[kOutputPolarity] = "PLRT";
  map[kOutputUnknown]  = "UNKNOWN";
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



EBasicWaveType_t GetBasicWaveType(std::string name);
std::string      GetBasicWaveType(EBasicWaveType_t type);
EWaveParameter_t GetWaveParmeter(std::string name);
std::string      GetWaveParameter(EWaveParameter_t type);
std::string      GetHeaderType   (EHeaderType_t type);
std::string      GetCmdSymbol    (ECmdSymbol_t type);
#endif
