add_library(TimeWeighting STATIC 
    TimeWeighting.cpp include/TimeWeighting.h
)
target_include_directories(TimeWeighting PUBLIC include)
target_link_libraries(TimeWeighting 
    PUBLIC
        FilterInterface
)
