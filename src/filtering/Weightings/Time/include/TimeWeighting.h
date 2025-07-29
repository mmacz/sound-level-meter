#pragma once
#include "FilterInterface.h"

namespace Filtering {
class TimeWeighting : FilterInterface {
public:
  TimeWeighting() = delete;
  TimeWeighting(unsigned int sampleRate, unsigned int mSec);
  ~TimeWeighting() = default;

  TimeWeighting(const TimeWeighting &) = delete;
  TimeWeighting(TimeWeighting &&) = delete;

  TimeWeighting &operator=(const TimeWeighting &) = delete;
  TimeWeighting &operator=(TimeWeighting &&) = delete;

  int process(float *inSamples, float *outSamples,
              unsigned int nSamples) override;
  void clear_state() override;
  void set_msec(unsigned int mSec);

private:
  float mTau;
  float mAlpha;
  float mState;
  unsigned int mSampleRate;
};
} // namespace Filtering
