include(FetchContent)

if(FRACTION_INCLUDE_TESTS)
  # ------------- GoogleTest --------------

  set(BUILD_GMOCK OFF CACHE BOOL "" FORCE)
  set(BUILD_GTEST ON CACHE BOOL "" FORCE)
  
  FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG        v1.14.0
  )

  FetchContent_MakeAvailable(googletest)
endif()