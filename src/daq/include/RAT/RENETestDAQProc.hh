#ifndef __RAT_RENETestDAQProc__
#define __RAT_RENETestDAQProc__

#include <RAT/DB.hh>
#include <RAT/Digitizer.hh>
#include <RAT/Processor.hh>
#include <RAT/WaveformAnalysis.hh>
#include <string>

namespace RAT {

class RENETestDAQProc : public Processor {
 public:
  RENETestDAQProc();
  virtual ~RENETestDAQProc(){};
  virtual Processor::Result DSEvent(DS::Root *ds);
  void SetD(std::string param, double value);
  void SetI(std::string param, int value);

  void BeginOfRun(DS::Run *run);

 protected:
  int fEventCounter;
  double fPulseWidth;
  double fTriggerThreshold;
  double fTriggerWindow;
  double fPmtLockout;
  double fTriggerLockout;
  double fTriggerResolution;
  double fLookback;
  double fMaxHitTime;
  bool fDigitize;
  bool fAnalyze;

  int fTriggerOnNoise;
  DBLinkPtr ldaq;

  Digitizer *fDigitizer;
  std::string fDigitizerType;

  WaveformAnalysis *fWaveformAnalysis;
};

}  // namespace RAT

#endif
