add_library(AWeighting STATIC
    AWeighting.cpp include/AWeighting.h
)
target_include_directories(AWeighting PUBLIC include)
target_link_libraries(AWeighting PUBLIC
    BiquadDFII
    FilterInterface
)
