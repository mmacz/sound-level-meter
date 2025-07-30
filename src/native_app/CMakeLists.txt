add_executable(slm_reference
  main.cpp
)
target_link_libraries(slm_reference
  PRIVATE
    SoundLevelMeter
    AWeighting
    CWeighting
    TimeWeighting
    cxxopts::cxxopts
    SndFile::sndfile
)

