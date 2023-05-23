cmake_minimum_required(VERSION 3.1)
project(lineout-player VERSION 0.9.0)

set(CMAKE_CXX_STANDARD 20)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(PkgConfig REQUIRED)

pkg_check_modules(LIBAV REQUIRED IMPORTED_TARGET
    libavformat
    libavcodec
    libavutil
    libswresample
)

pkg_check_modules(LIBAO REQUIRED IMPORTED_TARGET
    ao
)

add_executable(lineout-player
    src/AudioPlayer.cpp
    src/main.cpp)

target_link_libraries(lineout-player PkgConfig::LIBAV PkgConfig::LIBAO)

install(TARGETS lineout-player DESTINATION bin)
