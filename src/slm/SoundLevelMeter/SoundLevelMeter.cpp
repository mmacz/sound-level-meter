#include "SoundLevelMeter.h"
#include "AWeighting.h"
#include "CWeighting.h"

#include <cstdint>
#include <cmath>
#include <stdexcept>

namespace slm {

uint32_t GetTimeWeightingTimeMs(TimeWeighting tW) {
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
  return 20.f * log10f(value / referenceLevel);
}

SoundLevelMeter::SoundLevelMeter(const SLMConfig &cfg)
    : mTimeWeighting(GetTimeWeightingTimeMs((TimeWeighting)cfg.tW))
    , mPeak(.0f)
    , mReferenceLevel(cfg.referenceLevel) {
  reset(cfg);
}

void SoundLevelMeter::reset(const SLMConfig &cfg) {
  SLMConfig _c{cfg};
  switch (_c.fW) {
  case FrequencyWeighting::A:
    mFreqWeighting.reset(new Filtering::AWeighting());
    break;
  case FrequencyWeighting::C:
    mFreqWeighting.reset(new Filtering::CWeighting());
    break;
  case FrequencyWeighting::Z:
    mFreqWeighting.reset(nullptr);
    break;
  default:
    throw std::invalid_argument(
        "SOUND LEVEL METER: Invalid Frequency Weighting");
  }
  mTimeWeighting.set_msec(GetTimeWeightingTimeMs(_c.tW));
  mReferenceLevel = _c.referenceLevel;
}

MeterResults SoundLevelMeter::process(const float &sample) {
  MeterResults results;
  float fWeighted = sample;
  float tWeighted = .0f;
  if (mFreqWeighting) {
    mFreqWeighting->process(const_cast<float *>(&sample), &fWeighted, 1);
  }
  if (fWeighted > mPeak) {
    mPeak = fWeighted;
  }
  mTimeWeighting.process(&fWeighted, &tWeighted, 1);

  results.leq = GetDBLevel(tWeighted, mReferenceLevel);
  results.peak = GetDBLevel(mPeak, mReferenceLevel);
  results.spl = GetDBLevel(sample, mReferenceLevel);
  return results;
}

} // namespace slm
