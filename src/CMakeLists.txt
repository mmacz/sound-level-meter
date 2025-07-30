add_subdirectory(filtering)
add_subdirectory(slm)

if (NATIVE_BUILD)
  add_subdirectory(native_app)
endif()

