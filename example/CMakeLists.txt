fetch_github_content(discover_sources CesarBerriot 1.0.0 INCLUDE_DEFAULT)

add_executable(dial_example WIN32)
dial_link_target(dial_example)
discover_sources(dial_example)
target_compile_definitions(dial_example PRIVATE MODULE_NAME="DIAL Example")
if(MINGW)
  target_link_options(dial_example PRIVATE -static)
endif()