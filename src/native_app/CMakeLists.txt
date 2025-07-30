add_executable(slm_reference
  main.cpp
)
target_link_libraries(slm_reference
  PRIVATE
    SoundLevelMeter
    cxxopts::cxxopts
    SndFile::sndfile
)

