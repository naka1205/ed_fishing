
cmake_minimum_required (VERSION 3.5)
project(Fishing)

include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/src)

file(GLOB_RECURSE SRC_DIR_LIST "src/*.cpp" "engine/*.cpp")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG 	${PROJECT_SOURCE_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE 	${PROJECT_SOURCE_DIR}/bin)

add_subdirectory(box2dx)

add_executable(Fishing
  ${SRC_DIR_LIST}
  main.cpp
)

if (EMSCRIPTEN)
  set(CMAKE_MAKE_PROGRAM ninja)
  # set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")

  target_link_libraries(Fishing PRIVATE box2dx)

  target_link_libraries(Fishing PUBLIC "-s USE_SDL=2")
  target_link_libraries(Fishing PUBLIC "-s USE_ZLIB=1")
  target_link_libraries(Fishing PUBLIC "-s USE_SDL_TTF=2")
  target_link_libraries(Fishing PUBLIC "-s USE_SDL_GFX=2")
  target_link_libraries(Fishing PUBLIC "-s USE_SDL_MIXER=2")
  target_link_libraries(Fishing PUBLIC "-s USE_SDL_IMAGE=2")
  target_link_libraries(Fishing PUBLIC "-s SDL2_IMAGE_FORMATS=png")
  target_link_libraries(Fishing PUBLIC "-s DISABLE_EXCEPTION_CATCHING=0")
  target_link_libraries(Fishing PUBLIC "-s STACK_SIZE=231072")
  target_link_libraries(Fishing PUBLIC "-s MALLOC=emmalloc")
  target_link_libraries(Fishing PUBLIC "-s EXPORT_ALL=1")
  target_link_libraries(Fishing PUBLIC "-s WASM=1")
  target_link_libraries(Fishing PUBLIC "-s WASM_BIGINT")
  target_link_libraries(Fishing PUBLIC "-Wall")

  set_property(TARGET Fishing APPEND_STRING PROPERTY LINK_FLAGS "--preload-file ${PROJECT_SOURCE_DIR}/bin/resources/@/resources/")

else()

  find_package(ZLIB REQUIRED)
  find_package(SDL2 CONFIG REQUIRED)
  find_package(SDL2_image CONFIG REQUIRED)
  find_package(SDL2_ttf CONFIG REQUIRED)
  find_package(sdl2-gfx CONFIG REQUIRED)
  find_package(SDL2_mixer CONFIG REQUIRED)

  message(STATUS "SDL2 found: SDL2_INCLUDE_DIRS: ${SDL2_INCLUDE_DIRS}, SDL2_LIBRARIES: ${SDL2_LIBRARIES}")

  target_link_libraries(Fishing PRIVATE box2dx ZLIB::ZLIB SDL2::SDL2main SDL2::SDL2_gfx SDL2::SDL2-static SDL2_image::SDL2_image-static SDL2_ttf::SDL2_ttf-static SDL2_mixer::SDL2_mixer-static )

endif()

target_include_directories(Fishing PRIVATE box2dx)

configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/Asset.hpp.in
  ${CMAKE_CURRENT_BINARY_DIR}/src/Asset.hpp
)

target_include_directories(Fishing
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src
  PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/src
)