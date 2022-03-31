if(TARGET igl::core)
    return()
endif()

include(FetchContent)
FetchContent_Declare(
    libigl
    GIT_REPOSITORY https://github.com/libigl/libigl.git
    GIT_TAG a59d15af2b95bfb0ad7b6d9c062dd14bd76cee8d
)
FetchContent_MakeAvailable(libigl)
