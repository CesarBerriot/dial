cmake_minimum_required(VERSION 3.31)
project(dial)

if(${CMAKE_CURRENT_SOURCE_DIR} STREQUAL ${CMAKE_SOURCE_DIR})
  set(IS_TOP_LEVEL_PROJECT TRUE)
else()
  set(IS_TOP_LEVEL_PROJECT FALSE)
endif()

option(DIAL_EXAMPLE "Generate DIAL's example target." ${IS_TOP_LEVEL_PROJECT})

set(REQUIRED_CXX_STANDARD 20)
if(DEFINED CMAKE_CXX_STANDARD)
  if(${CMAKE_CXX_STANDARD} LESS ${REQUIRED_CXX_STANDARD})
    message(FATAL_ERROR "DIAL requires at least C++ " ${REQUIRED_CXX_STANDARD} ".")
  endif()
else()
  set(CMAKE_CXX_STANDARD ${REQUIRED_CXX_STANDARD})
endif()
set(BUILD_SHARED_LIBS OFF)

include(FetchContent)
FetchContent_Declare(fetch_github_content GIT_REPOSITORY https://github.com/CesarBerriot/fetch_github_content.git GIT_TAG 1.0.0)
FetchContent_MakeAvailable(fetch_github_content)
include(${fetch_github_content_SOURCE_DIR}/fetch_github_content.cmake)

block()
  set(SDL_SHARED OFF)
  set(SDL_STATIC ON)
  fetch_github_content(
    make_string CesarBerriot 2.2.3
    hard_assert CesarBerriot 1.2.3
    pre_main_runner CesarBerriot 1.0.0
    SDL libsdl-org release-3.2.10
    imgui ocornut v1.91.9
    ImGuiFileDialog aiekick v0.6.7
  )
  file(GLOB sources ${imgui_SOURCE_DIR}/*.cpp)
  add_library(DearImGui ${sources} ${imgui_SOURCE_DIR}/backends/imgui_impl_sdl3.cpp ${imgui_SOURCE_DIR}/backends/imgui_impl_sdlrenderer3.cpp)
  target_link_libraries(DearImGui PRIVATE SDL3-static)
  target_include_directories(DearImGui PUBLIC ${imgui_SOURCE_DIR} ${imgui_SOURCE_DIR}/backends)
  target_link_libraries(ImGuiFileDialog PRIVATE DearImGui)
endblock()

add_library(dial)
block()
  file(GLOB_RECURSE sources CONFIGURE_DEPENDS source/*.cpp)
  foreach(source ${sources})
    get_filename_component(source_file_name ${source} NAME)
    if(NOT source_file_name STREQUAL "main.cpp")
      target_sources(dial PRIVATE ${source})
    endif()
  endforeach()
endblock()
target_include_directories(dial PUBLIC include)
target_include_directories(dial PRIVATE include/dial source)
target_link_libraries(
  dial PUBLIC
  SDL3-static
  DearImGui
  ImGuiFileDialog
  pre_main_runner
  PRIVATE
  hard_assert
  make_string
)
target_compile_definitions(dial PRIVATE MODULE_NAME="App Base")
if(MINGW)
  target_link_options(dial PUBLIC -static)
  target_compile_options(dial PUBLIC -Wno-write-strings)
elseif(MSVC)
  target_compile_options(dial PUBLIC /Zc:strictStrings-)
endif()

function(dial_link_target target)
  target_link_libraries(${target} PRIVATE dial)
  get_target_property(dial_source_directory dial SOURCE_DIR)
  target_sources(${target} PRIVATE ${dial_source_directory}/source/main.cpp)
endfunction()

if(DIAL_EXAMPLE)
  add_subdirectory(example)
endif()