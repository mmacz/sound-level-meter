#include <string>
#include <iostream>

#include "SoundLevelMeter.h"
#include "cxxopts.hpp"

using namespace slm;

struct CLIOpts {
  std::string filename;
  slm::TimeWeighting tw;
  slm::FrequencyWeighting fw;
  float referenceLevel;

  CLIOpts()
      : filename(""), fw(slm::FrequencyWeighting::A),
        tw(slm::TimeWeighting::SLOW), referenceLevel(94.f) {}
};


FrequencyWeighting parseFw(const std::string &s) {
  if (s == "a")
    return FrequencyWeighting::A;
  if (s == "c")
    return FrequencyWeighting::C;
  if (s == "z")
    return FrequencyWeighting::Z;
  throw std::invalid_argument("Invalid frequency weighting: " + s);
}

TimeWeighting parseTw(const std::string &s) {
  if (s == "fast")
    return TimeWeighting::FAST;
  if (s == "slow")
    return TimeWeighting::SLOW;
  if (s == "impulse")
    return TimeWeighting::IMPULSE;
  throw std::invalid_argument("Invalid time weighting: " + s);
}

CLIOpts parseOpts(int &argc, char **argv) {
  try {
    CLIOpts opts;
    cxxopts::Options options("slm", "Sound Level Meter Test Tool");
    options.add_options()
      ( "f,freq", "Frequency weighting (a/c/z)", cxxopts::value<std::string>()->default_value("a"))
      ( "t,time", "Time weighting (fast/slow/impulse)", cxxopts::value<std::string>()->default_value("fast"))
      ( "input", "Input WAV file", cxxopts::value<std::string>())
      ("r,reference", "Reference level in dB", cxxopts::value<float>()->default_value("94"))
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
  return 0;
}
