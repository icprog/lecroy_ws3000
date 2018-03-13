#ifndef WSTYPES_HH
#define WSTYPES_HH

#include <map>
#include <iostream>
#include <string>

#define MAX_ENUM_STRING_SIZE 26


typedef enum {
	      kHeaderC1,
	      kHeaderC2,
	      kHeaderM1,
	      kHeaderM2,
	      kHeaderM3,
	      kHeaderM4,
	      kHeaderM5,
	      kHeaderUnknown
} EHeaderPath_t;



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



typedef std::map < EHeaderPath_t,    std::string > HeaderPathMap;
typedef std::map < EBasicWaveType_t, std::string > BasicWaveMap;
typedef std::map < EWaveParameter_t, std::string > WaveParameterMap;


inline HeaderPathMap CreateHeaderPathMap()
{
  HeaderPathMap map;
  map[kHeaderC1]    = "C1";
  map[kHeaderC2]    = "C2";
  map[kHeaderM1]    = "M1";
  map[kHeaderM2]    = "M2";
  map[kHeaderM3]    = "M3";
  map[kHeaderM4]    = "M4";
  map[kHeaderM5]    = "M5";
  map[kHeaderUnknown] = "UNKNOWN";
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


EBasicWaveType_t GetBasicWaveType(std::string name);
std::string      GetBasicWaveType(EBasicWaveType_t type);
EWaveParameter_t GetWaveParmeter(std::string name);
std::string      GetWaveParameter(EWaveParameter_t type);

#endif
