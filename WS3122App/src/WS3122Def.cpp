
#include "WS3122Def.hh"
#include "WSTypes.hh"


BasicWave::BasicWave()
{
  Init();
  
};


// BasicWave::BasicWave(const BasicWave &bwobj)
// {

   
//   headerPath           = bwobj.headerPath;
//   waveTypeID           = bwobj.waveTypeID;
//   frequencyVal         = bwobj.frequencyVal;
//   amplifierVal         = bwobj.amplifierVal;
//   offsetVal            = bwobj.offsetVal;
//   dutyCycleVal         = bwobj.dutyCycleVal;
//   phaseVal             = bwobj.phaseVal;
//   stdDevVal            = bwobj.stdDevVal ;
//   meanVal              = bwobj.meanVal;
//   widthVal             = bwobj.widthVal;
//   riseVal              = bwobj.riseVal;
//   fallVal              = bwobj.fallVal;
//   delayVal             = bwobj.delayVal;

//   frequencyUnit        = bwobj.frequencyUnit;
//   amplifierUnit        = bwobj.amplifierUnit;
//   offsetUnit           = bwobj.offsetUnit;
//   dutyCycleUnit        = bwobj.dutyCycleUnit;

//   waveTypeString       = bwobj.waveTypeString;
//   frequencyUnit        = bwobj.frequencyUnit;
//   amplifierUnit        = bwobj.amplifierUnit;
//   offsetUnit           = bwobj.offsetUnit;
//   dutyCycleUnit        = bwobj.dutyCycleUnit;
//   fullCommandStream    << bwobj.fullCommandStream;

//   waveParameter
// };


BasicWave::~BasicWave()
{
};


void 
BasicWave::Init()
{

  waveTypeID   = kWaveTypeSquare;
  frequencyVal = 0.0;
  amplifierVal = 0.0;
  offsetVal    = 0;
  dutyCycleVal = 0.0;
  phaseVal     = 0;
  stdDevVal    = 0.0;
  meanVal      = 0.0;
  widthVal     = 0.0;
  riseVal      = 0.0;
  fallVal      = 0.0;
  delayVal     = 0.0;


  frequency_flag  = true;
  amplifier_flag  = true;
  offset_flag     = true;
  symmetry_flag   = true;
  duty_cycle_flag = true;
  
  width_flag      = true;
  rise_flag       = true;
  fall_flag       = true;
  delay_flag      = true;

  phase_flag      = true;
  std_dev_flag    = true;
  mean_flag       = true;

  
  
  waveTypeString.clear();
  frequencyUnit = "HZ";
  amplifierUnit = "V";
  offsetUnit    = "V";
  dutyCycleUnit = "percent";

  // frequencyUnit.clear();
  // amplifierUnit.clear();
  // offsetUnit.clear();
  // dutyCycleUnit.clear();

  clearCommand();

  waveTypeMap     = CreateBasicWaveMap();
  waveParamterMap = CreateWaveParameterMap();
  headerPathMap   = CreateHeaderPathMap();
  
};

void
BasicWave::setWaveTypeID(EBasicWaveType_t id)
{
  waveTypeID = id;
  waveTypeString = waveTypeMap[id];
  this -> set_flags(id);

  return;
};

void
BasicWave::set_flags(EBasicWaveType_t id)
{
  switch (id)
    {
    case kWaveTypeSine:
      frequency_flag  = true;    amplifier_flag  = true;
      offset_flag     = true;    phase_flag      = true;
      duty_cycle_flag = true;
      width_flag      = false;    rise_flag      = false;
      fall_flag       = false;    delay_flag     = false;
      symmetry_flag   = false;
      std_dev_flag    = false;    mean_flag      = false;
      break;
    case kWaveTypeSquare:
      frequency_flag  = true;    amplifier_flag  = true;
      offset_flag     = true;    phase_flag      = true;
      duty_cycle_flag = true;
      width_flag      = false;    rise_flag      = false;
      fall_flag       = false;    delay_flag     = false;
      symmetry_flag   = false;
      std_dev_flag    = false;    mean_flag      = false;
      break;
    case kWaveTypeRamp:
      frequency_flag  = true;    amplifier_flag  = true;
      offset_flag     = true;    phase_flag      = true;
      duty_cycle_flag = true;
      width_flag      = false;    rise_flag      = false;
      fall_flag       = false;    delay_flag     = false;
      symmetry_flag   = true;
      std_dev_flag    = false;    mean_flag      = false;
      break;
    case kWaveTypePulse:
      frequency_flag  = true;    amplifier_flag  = true;
      offset_flag     = true;    phase_flag      = true;
      duty_cycle_flag = true;
      width_flag      = true;    rise_flag       = true;
      fall_flag       = true;    delay_flag      = true;
      symmetry_flag   = false;
      std_dev_flag    = false;   mean_flag       = false;
      break;
    case kWaveTypeNoise:
      frequency_flag  = false;   amplifier_flag  = false;
      offset_flag     = false;   phase_flag      = false;
      duty_cycle_flag = true;
      width_flag      = false;    rise_flag      = false;
      fall_flag       = false;    delay_flag     = false;
      symmetry_flag   = false;
      std_dev_flag    = true;     mean_flag      = true;
      break;
    case kWaveTypeArb:
      frequency_flag  = true;    amplifier_flag  = true;
      offset_flag     = true;    phase_flag      = true;
      duty_cycle_flag = true;
      width_flag      = false;    rise_flag      = false;
      fall_flag       = false;    delay_flag     = false;
      symmetry_flag   = false;
      std_dev_flag    = false;    mean_flag      = false;
      break;
    case kWaveTypeDc:
      frequency_flag  = true;    amplifier_flag  = true;
      offset_flag     = true;    phase_flag      = true;
      duty_cycle_flag = true;
      width_flag      = false;    rise_flag      = false;
      fall_flag       = false;    delay_flag     = false;
      symmetry_flag   = false;
      std_dev_flag    = false;    mean_flag      = false;
      break;
    default:
      frequency_flag  = true;    amplifier_flag  = true;
      offset_flag     = true;    phase_flag      = true;
      duty_cycle_flag = true;
      width_flag      = false;    rise_flag      = false;
      fall_flag       = false;    delay_flag     = false;
      symmetry_flag   = false;
      std_dev_flag    = false;    mean_flag      = false;
      break;
      
    }
  
  return;
};

// BasicWave & BasicWave::operator=(const BasicWave &bwobj)
// {
//   if (this == &bwobj)
//     return *this;
  
//   headerPath           = bwobj.headerPath;
//   waveTypeID           = bwobj.waveTypeID;
//   frequencyVal         = bwobj.frequencyVal;
//   amplifierVal         = bwobj.amplifierVal;
//   offsetVal            = bwobj.offsetVal;
//   dutyCycleVal         = bwobj.dutyCycleVal;
//   phaseVal             = bwobj.phaseVal;
//   stdDevVal            = bwobj.stdDevVal ;
//   meanVal              = bwobj.meanVal;
//   widthVal             = bwobj.widthVal;
//   riseVal              = bwobj.riseVal;
//   fallVal              = bwobj.fallVal;
//   delayVal             = bwobj.delayVal;

//   frequencyUnit        = bwobj.frequencyUnit;
//   amplifierUnit        = bwobj.amplifierUnit;
//   offsetUnit           = bwobj.offsetUnit;
//   dutyCycleUnit        = bwobj.dutyCycleUnit;

//   waveTypeString       = bwobj.waveTypeString;
//   fullCommandStream    << bwobj.fullCommandStream;
  

//   return *this;
  
// };




std::ostream& operator<<(std::ostream& os, const BasicWave &bwobj)
{
  int width = 30;
  os << std::setiosflags(std::ios::right);
  os << "\n HeaderPath    : ";
  os << std::setw(width) << bwobj.headerPath;
  os << "\n WaveTypeID    : ";
  os << std::setw(width) << bwobj.waveTypeID;
  os << "\n frequencyVal  : ";
  os << std::setw(width) << bwobj.frequencyVal;
  os << "\n amplifierVal  : ";
  os << std::setw(width) << bwobj.amplifierVal;
  os << "\n offsetVal     : ";
  os << std::setw(width) << bwobj.offsetVal;
  os << "\n dutyCycleVal  : ";
  os << std::setw(width) << bwobj.dutyCycleVal;
  os << "\n phaseVal      : ";
  os << std::setw(width) << bwobj.phaseVal;
  os << "\n standardDeviationVal : ";
  os << std::setw(width) << bwobj.stdDevVal ;
  os << "\n meanVal       : ";
  os << std::setw(width) << bwobj.meanVal;
  os << "\n widthVal      : ";
  os << std::setw(width) << bwobj.widthVal;
  os << "\n riseVal       : ";
  os << std::setw(width) << bwobj.riseVal;
  os << "\n fallVal       : ";
  os << std::setw(width) << bwobj.fallVal;
  os << "\n delayVal      : ";
  os << std::setw(width) << bwobj.delayVal;
  os << "\n frequencyUnit : ";
  os << std::setw(width) << bwobj.frequencyUnit;
  os << "\n amplifierUnit : ";
  os << std::setw(width) << bwobj.amplifierUnit;
  os << "\n offsetUnit    : ";
  os << std::setw(width) << bwobj.offsetUnit;
  os << "\n dutyCycleUnit : ";
  os << std::setw(width) << bwobj.dutyCycleUnit;
  os << "\n Full Cmd      : ";
  os << std::setw(width) << bwobj.fullCommandStream.str();
  return os;
  
}



void
BasicWave::Print(std::string in)
{
  std::cout << " >>> " << in << std::endl;
  int width = 40;
  std::cout
    <<
    std::setiosflags(std::ios::right) <<
    "\n HeaderPath    : " <<
    std::setw(width) << headerPath <<
    "\n WaveTypeID    : " <<
    std::setw(width) << waveTypeID <<
    "\n WaveTypeString: " <<
    std::setw(width) << waveTypeString <<
    "\n frequencyVal  : " <<
    std::setw(width) << frequencyVal <<
    "\n amplifierVal  : " <<
    std::setw(width) << amplifierVal <<
    "\n offsetVal     : " <<
    std::setw(width) << offsetVal <<
    "\n dutyCycleVal  : " <<
    std::setw(width) << dutyCycleVal <<
    "\n phaseVal      : " <<
    std::setw(width) << phaseVal <<
    "\n StandardDeVal : " <<
    std::setw(width) << stdDevVal  <<
    "\n meanVal       : " <<
    std::setw(width) << meanVal <<
    "\n widthVal      : " <<
    std::setw(width) << widthVal <<
    "\n riseVal       : " <<
    std::setw(width) << riseVal <<
    "\n fallVal       : " <<
    std::setw(width) << fallVal <<
    "\n delayVal      : " <<
    std::setw(width) << delayVal <<
    "\n frequencyUnit : " <<
    std::setw(width) << frequencyUnit <<
    "\n amplifierUnit : " <<
    std::setw(width) << amplifierUnit <<
    "\n offsetUnit    : " <<
    std::setw(width) << offsetUnit <<
    "\n dutyCycleUnit : " <<
    std::setw(width) << dutyCycleUnit <<
    "\n Full Cmd      : " <<
    std::setw(width) << fullCommandStream.str() <<std::endl;
  return;
  
}



void
BasicWave::buildCommand()
{

  bool front_comma = true;
  
  fullCommandStream << headerPathMap[headerPathID];
  fullCommandStream << ":BSWV WVTP,";
  fullCommandStream << waveTypeMap[waveTypeID];
  fullCommandStream << getWaveCmdSnip(frequency_flag,  kWaveFreq,    frequencyVal, front_comma);
  fullCommandStream << getWaveCmdSnip(amplifier_flag,  kWaveAmpl,    amplifierVal, front_comma);
  fullCommandStream << getWaveCmdSnip(offset_flag,     kWaveOffset,  offsetVal,    front_comma);
  fullCommandStream << getWaveCmdSnip(phase_flag,      kWavePhase,   phaseVal,     front_comma);
  fullCommandStream << getWaveCmdSnip(duty_cycle_flag, kWaveDuty,    dutyCycleVal, front_comma);
  fullCommandStream << getWaveCmdSnip(width_flag,      kWaveWidth,   widthVal,     front_comma);
  fullCommandStream << getWaveCmdSnip(rise_flag,       kWaveRise,    riseVal,      front_comma);
  fullCommandStream << getWaveCmdSnip(fall_flag,       kWaveFall,    fallVal,      front_comma);
  fullCommandStream << getWaveCmdSnip(delay_flag,      kWaveDelay,   delayVal,     front_comma);
  fullCommandStream << getWaveCmdSnip(symmetry_flag,   kWaveSymm,    symmetryVal,  front_comma);
  fullCommandStream << getWaveCmdSnip(std_dev_flag,    kWaveStdDev,  stdDevVal,    front_comma);
  fullCommandStream << getWaveCmdSnip(mean_flag,       kWaveMean,    meanVal,      front_comma);
  
  return;
};

void
BasicWave::clearCommand()
{
  fullCommandStream.str("");
  fullCommandStream.clear();
  //  std::cout << "Is it clean? " << fullCommandStream.str() << std::endl;
  return;
}

const std::string
BasicWave::getWaveCmdSnip(bool cmd_flag, EWaveParameter_t id, double value, bool front_prefix)
{
  std::ostringstream result;
  std::string        unit;

  if(cmd_flag) {
    if (front_prefix) result << ",";
    result << waveParamterMap[id];
    result << ",";
    result << value;
    switch (id)
      {
      case kWaveFreq:   unit=frequencyUnit; break;
      case kWaveAmpl:   unit=amplifierUnit; break;
      case kWaveOffset: unit=offsetUnit;    break;
      case kWaveDuty:   unit=dutyCycleUnit; break;
      default: unit.clear(); break;
	
      }
    result << unit;
  }
  else {
    result.str("");
    result.clear();
  }

  return result.str();
};
