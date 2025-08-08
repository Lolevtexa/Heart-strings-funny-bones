cmake_minimum_required(VERSION 3.16)

# Heart‑strings‑funny‑bones
#
# This CMake configuration builds the C++ project located in this repository.
# The project is a simple SFML application that demonstrates a menu system,
# localization support and basic UI widgets.  It compiles the single
# translation unit (``source/main.cpp``) and links against SFML and
# nlohmann_json.  The ``assets`` directory is copied into the build
# directory so that fonts, textures, music and localisation files can be
# found at run time.

# Set the project name and version
project(HeartStringsFunnyBones VERSION 0.1 LANGUAGES CXX)

# Require at least C++17 for std::filesystem and structured bindings
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Optionally allow the user to choose whether to use a system-installed
# nlohmann_json package.  By default we fetch it via FetchContent.
option(USE_SYSTEM_JSON "Use system provided nlohmann_json if available" OFF)

# Use FetchContent to download nlohmann_json if requested
include(FetchContent)
if(NOT USE_SYSTEM_JSON)
  FetchContent_Declare(
    nlohmann_json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG v3.11.2
  )
  FetchContent_MakeAvailable(nlohmann_json)
endif()

# Find SFML.  We require the graphics, audio, window and system components.
set(SFML_STATIC_LIBRARIES OFF CACHE BOOL "Build with SFML static libraries")
find_package(SFML 2.5 COMPONENTS graphics audio window system REQUIRED)

# Define the executable target.  Only one .cpp file exists in this project;
# all other implementation resides in header files.
add_executable(${PROJECT_NAME}
  source/main.cpp
)

# Add the ``source`` directory to the list of include directories so that
# headers such as ``widgets/activatable/clickable/button.hpp`` are found
# correctly.  CMake will pass this directory via ``-I`` to the compiler.
target_include_directories(${PROJECT_NAME}
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/source
)

# Link SFML and nlohmann_json to the executable.  When USE_SYSTEM_JSON is
# enabled, the appropriate imported target is expected to exist; otherwise,
# FetchContent_MakeAvailable creates ``nlohmann_json::nlohmann_json``.
target_link_libraries(${PROJECT_NAME}
  PRIVATE
    sfml-graphics
    sfml-audio
    sfml-window
    sfml-system
    nlohmann_json::nlohmann_json
)

# Copy the ``assets`` directory into the build output directory.  This
# ensures that resources such as fonts, textures, localisation files and
# music are available relative to the executable when it runs.  The
# ``PRE_BUILD`` phase is used so that the copy occurs every time the target
# is built (useful when assets change).
add_custom_command(TARGET ${PROJECT_NAME} PRE_BUILD
  COMMAND ${CMAKE_COMMAND} -E remove_directory
          $<TARGET_FILE_DIR:${PROJECT_NAME}>/assets
  COMMAND ${CMAKE_COMMAND} -E copy_directory
          ${CMAKE_CURRENT_SOURCE_DIR}/assets
          $<TARGET_FILE_DIR:${PROJECT_NAME}>/assets
  COMMENT "Cleaning and copying assets directory to build output"
)

# Copy the ``config`` directory into the build output directory.
add_custom_command(TARGET ${PROJECT_NAME} PRE_BUILD
  COMMAND ${CMAKE_COMMAND} -E remove_directory
          $<TARGET_FILE_DIR:${PROJECT_NAME}>/config
  COMMAND ${CMAKE_COMMAND} -E copy_directory
          ${CMAKE_CURRENT_SOURCE_DIR}/config
          $<TARGET_FILE_DIR:${PROJECT_NAME}>/config
  COMMENT "Cleaning and copying config directory to build output"
)
