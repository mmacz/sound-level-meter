add_library(CWeighting STATIC
    CWeighting.cpp include/CWeighting.h
)
target_include_directories(CWeighting PUBLIC include)
target_link_libraries(CWeighting PUBLIC
    BiquadDFII
    FilterInterface
)
