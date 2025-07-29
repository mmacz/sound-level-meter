add_library(SoundLevelMeter STATIC
    SoundLevelMeter.cpp include/SoundLevelMeter.h
)
target_include_directories(SoundLevelMeter PUBLIC include)
target_link_libraries(SoundLevelMeter
  PUBLIC
    AWeighting
    CWeighting
    TimeWeighting
)

