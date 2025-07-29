#pragma once

namespace Filtering {

class FilterInterface {
public:
  FilterInterface();
  virtual ~FilterInterface() {};

  FilterInterface(const FilterInterface &) = delete;
  FilterInterface(FilterInterface &&) = delete;

  FilterInterface &operator=(const FilterInterface &) = delete;
  FilterInterface &operator=(FilterInterface &&) = delete;

  virtual int process(float *in, float *out, unsigned int nSamples) = 0;
  virtual void clear_state() = 0;
};

} // namespace Filtering
