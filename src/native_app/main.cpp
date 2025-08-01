#include <string>
#include <iostream>
#include <vector>

#include "cxxopts.hpp"
#include "sndfile.hh"
#include "SoundLevelMeter.h"

using namespace slm;

static constexpr size_t READ_BLOCK_SIZE = 512;

struct CLIOpts {
  std::string filename;
  slm::TimeWeighting tw;
  slm::FrequencyWeighting fw;
  float referenceLevel;

  CLIOpts()
      : filename("")
      , fw(slm::FrequencyWeighting::A)
      , tw(slm::TimeWeighting::SLOW)
      , referenceLevel(2e-5) {}
};


static FrequencyWeighting parseFw(const std::string &s) {
  if (s == "a")
    return FrequencyWeighting::A;
  if (s == "c")
    return FrequencyWeighting::C;
  if (s == "z")
    return FrequencyWeighting::Z;
  throw std::invalid_argument("Invalid frequency weighting: " + s);
}

static TimeWeighting parseTw(const std::string &s) {
  if (s == "fast")
    return TimeWeighting::FAST;
  if (s == "slow")
    return TimeWeighting::SLOW;
  if (s == "impulse")
    return TimeWeighting::IMPULSE;
  throw std::invalid_argument("Invalid time weighting: " + s);
}

static CLIOpts parseOpts(int &argc, char **argv) {
  try {
    std::string appName = argv[0];
    #ifdef _WIN32
      appName = appName.substr(appName.find_last_of("\\/") + 1);
    #else
      appName = appName.substr(appName.find_last_of("/\\") + 1);
    #endif
    CLIOpts opts;
    cxxopts::Options options(appName.c_str(), "Sound Level Meter Test Tool");
    options.add_options()
      ( "f,freq", "Frequency weighting (a/c/z)", cxxopts::value<std::string>()->default_value("a"))
      ( "t,time", "Time weighting (fast/slow/impulse)", cxxopts::value<std::string>()->default_value("slow"))
      ( "input", "Input WAV file", cxxopts::value<std::string>())
      ( "r,reference", "Reference level in sample value", cxxopts::value<float>()->default_value("2e-5"))
      ( "h,help", "Show help");

    options.parse_positional({"input"});
    auto result = options.parse(argc, argv);

    if (result.count("help") || !result.count("input")) {
      std::cout << options.help({""}) << "\n";
      exit(0);
    }

    std::string fwStr = result["freq"].as<std::string>();
    std::string twStr = result["time"].as<std::string>();
    opts.filename = result["input"].as<std::string>();
    opts.referenceLevel = result["reference"].as<float>();
    opts.fw = parseFw(fwStr);
    opts.tw = parseTw(twStr);
    return opts;

  } catch (const std::exception &e) {
    std::cerr << "Validation error: " << e.what() << "\n";
    exit(1);
  }
}

int main(int argc, char **argv) {
  CLIOpts opts = parseOpts(argc, argv);

  SLMConfig cfg;
  cfg.referenceLevel = opts.referenceLevel;
  cfg.fW = opts.fw;
  cfg.tW = opts.tw;
  SoundLevelMeter meter(cfg);

  SndfileHandle file(opts.filename.c_str());
  if (file.error()) {
    std::cerr << "Failed to open file: " << opts.filename << "\n";
    return 1;
  }
  if ((file.format() & SF_FORMAT_TYPEMASK) != SF_FORMAT_WAV) {
    std::cerr << "Unsupported file format. Must be WAV.\n";
    return 1;
  }
  std::cout << "Channels: " << file.channels() << "\n";
  std::cout << "Sample rate: " << file.samplerate() << "\n";
  std::cout << "Frames: " << file.frames() << "\n";

  if (file.channels() != 1) {
    std::cerr << "Input file must be Mono WAV file" << "\n";
    return 1;
  }

  std::vector<float> buffer(READ_BLOCK_SIZE);
  sf_count_t readFrames = 0;

  MeterResults results;
  while ((readFrames = file.readf(buffer.data(), READ_BLOCK_SIZE)) > 0) {
    for (auto& sample : buffer) {
      results = meter.process(sample);
    }
  }

  std::cout << "Peak: " << results.peak << "\n";
  std::cout << "Leq: " << results.leq << "\n";
  std::cout << "SPL: " << results.spl << "\n";

  return 0;
}
