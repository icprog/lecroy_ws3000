
#include "WS3122Def.hh"
#include "WSTypes.hh"


BasicWave::BasicWave()
{
  Init();
  
};


BasicWave::BasicWave(const BasicWave &bwobj)
{

   
  headerPath           = bwobj.headerPath;
  waveTypeID           = bwobj.waveTypeID;
  frequencyVal         = bwobj.frequencyVal;
  amplifierVal         = bwobj.amplifierVal;
  offsetVal            = bwobj.offsetVal;
  dutyCycleVal         = bwobj.dutyCycleVal;
  phaseVal             = bwobj.phaseVal;
  stdDevVal            = bwobj.stdDevVal ;
  meanVal              = bwobj.meanVal;
  widthVal             = bwobj.widthVal;
  riseVal              = bwobj.riseVal;
  fallVal              = bwobj.fallVal;
  delayVal             = bwobj.delayVal;

  frequencyUnit        = bwobj.frequencyUnit;
  amplifierUnit        = bwobj.amplifierUnit;
  offsetUnit           = bwobj.offsetUnit;
  dutyCycleUnit        = bwobj.dutyCycleUnit;

  waveTypeString       = bwobj.waveTypeString;
  frequencyUnit        = bwobj.frequencyUnit;
  amplifierUnit        = bwobj.amplifierUnit;
  offsetUnit           = bwobj.offsetUnit;
  dutyCycleUnit        = bwobj.dutyCycleUnit;
  fullCommandStream    << bwobj.fullCommandStream;

};


BasicWave::~BasicWave()
{
};


void 
BasicWave::Init()
{

  headerPath.clear();
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


  frequency_flag  = false;
  amplifier_flag  = false;
  offset_flag     = false;
  symmetry_flag   = false;
  duty_cycle_flag = false;
  
  width_flag      = false;
  rise_flag       = false;
  fall_flag       = false;
  delay_flag      = false;

  phase_flag      = false;
  std_dev_flag    = false;
  mean_flag       = false;

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
  
};

void
BasicWave::setWaveTypeID(EBasicWaveType_t id)
{
  waveTypeID = id;
  waveTypeString = GetBasicWaveType(waveTypeID);

  frequency_flag  = false;
  amplifier_flag  = false;
  offset_flag     = false;
  symmetry_flag   = false;
  duty_cycle_flag = false;
  
  width_flag      = false;
  rise_flag       = false;
  fall_flag       = false;
  delay_flag      = false;

  phase_flag      = false;
  std_dev_flag    = false;
  mean_flag       = false;

  switch  (waveTypeID) {
  case kWaveTypeSine | kWaveTypeSquare | kWaveTypeRamp | kWaveTypeArb | kWaveTypeDc :
    frequency_flag  = true;
    amplifier_flag  = true;
    offset_flag     = true;
    phase_flag      = true;
    duty_cycle_flag = true;
    ;
    break;
  case kWaveTypePulse :
    frequency_flag  = true;
    amplifier_flag  = true;
    offset_flag     = true;
    phase_flag      = true;
    duty_cycle_flag = true;
    
    width_flag      = true;
    rise_flag       = true;
    fall_flag       = true;
    delay_flag      = true;
    ;
    break;
  case kWaveTypeNoise:
    phase_flag      = true;
    std_dev_flag    = true;
    mean_flag       = true;
  default:
    ;
    
    
  }
    
  return;
};


BasicWave & BasicWave::operator=(const BasicWave &bwobj)
{
  if (this == &bwobj)
    return *this;
  
  headerPath           = bwobj.headerPath;
  waveTypeID           = bwobj.waveTypeID;
  frequencyVal         = bwobj.frequencyVal;
  amplifierVal         = bwobj.amplifierVal;
  offsetVal            = bwobj.offsetVal;
  dutyCycleVal         = bwobj.dutyCycleVal;
  phaseVal             = bwobj.phaseVal;
  stdDevVal            = bwobj.stdDevVal ;
  meanVal              = bwobj.meanVal;
  widthVal             = bwobj.widthVal;
  riseVal              = bwobj.riseVal;
  fallVal              = bwobj.fallVal;
  delayVal             = bwobj.delayVal;

  frequencyUnit        = bwobj.frequencyUnit;
  amplifierUnit        = bwobj.amplifierUnit;
  offsetUnit           = bwobj.offsetUnit;
  dutyCycleUnit        = bwobj.dutyCycleUnit;

  waveTypeString       = bwobj.waveTypeString;
  fullCommandStream    << bwobj.fullCommandStream;
  

  return *this;
  
};




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
BasicWave::Print()
{
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
  fullCommandStream << this -> getHeaderPath();
  fullCommandStream << ":BSWV WVTP,";
  fullCommandStream << this -> getWaveTypeString();
  fullCommandStream << ",";
  fullCommandStream << "FREQ,";
  fullCommandStream << this->getFrequencyVal();
  fullCommandStream << frequencyUnit ;
  
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
