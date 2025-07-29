#include "SoundLevelMeter.h"
#include "AWeighting.h"
#include "CWeighting.h"

#include <cstdint>
#include <math.h>
#include <stdexcept>

namespace slm {

uint32_t GetTimeWeightingTimeMs(TimeWeighting &tW) {
  switch (tW) {
  case TimeWeighting::FAST:
    return 125;
  case TimeWeighting::SLOW:
    return 1000;
  case TimeWeighting::IMPULSE:
    return 35;
  default:
    throw std::invalid_argument("SOUND LEVEL METER: Invalid Time Weighting");
  }
};

constexpr float GetDBLevel(const float &value, const float &referenceLevel) {
  return 20.f * std::log10f(value / referenceLevel);
}

SoundLevelMeter::SoundLevelMeter(SLMConfig &cfg)
    : mTimeWeighting(GetTimeWeightingTimeMs(cfg.tW)), mPeak(.0f),
      mReferenceLevel(cfg.referenceLevel) {
  reset(cfg);
}

void SoundLevelMeter::reset(SLMConfig &cfg) {
  switch (cfg.fW) {
  case FrequencyWeighting::A:
    mFreqWeighting.reset(new Filtering::AWeighting());
    break;
  case FrequencyWeighting::C:
    mFreqWeighting.reset(new Filtering::CWeighting());
    break;
  default:
    throw std::invalid_argument(
        "SOUND LEVEL METER: Invalid Frequency Weighting");
  }
  mTimeWeighting.set_msec(GetTimeWeightingTimeMs(cfg.tW));
  mReferenceLevel = cfg.referenceLevel;
}

MeterResults SoundLevelMeter::process(const float &sample) {
  MeterResults results;
  float tmp = .0f;
  mFreqWeighting->process(const_cast<float *>(&sample), &tmp, 1);
  if (tmp > mPeak) {
    mPeak = tmp;
  }
  mTimeWeighting.process(&tmp, &results.leq, 1);
  results.peak = mPeak;
  results.spl = GetDBLevel(sample, mReferenceLevel);
  return results;
}

} // namespace slm
