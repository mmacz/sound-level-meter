#pragma once
#include "FilterInterface.h"
#include "TimeWeighting.h"
#include <memory>

namespace slm {

enum class FrequencyWeighting { A, C };

enum class TimeWeighting {
  FAST,
  SLOW,
  IMPULSE,
};

struct MeterResults {
  float peak;
  float leq;
  float spl;
};

struct SLMConfig {
  FrequencyWeighting fW;
  TimeWeighting tW;
  float referenceLevel;
};

class SoundLevelMeter {
public:
  SoundLevelMeter(SLMConfig &cfg);
  ~SoundLevelMeter() = default;

  void reset(SLMConfig &cfg);
  MeterResults process(const float &sample);

private:
  std::unique_ptr<Filtering::FilterInterface> mFreqWeighting;
  Filtering::TimeWeighting mTimeWeighting;
  float mPeak;
  float mReferenceLevel;
};

} // namespace slm
