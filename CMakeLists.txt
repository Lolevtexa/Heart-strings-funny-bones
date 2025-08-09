cmake_minimum_required(VERSION 3.16)

project(HeartStringsFunnyBones VERSION 0.1 LANGUAGES CXX)

# --- C++ ---
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# --- Опции сборки зависимостей (оставлены для контроля, но по умолчанию бандлим) ---
option(BUNDLE_SFML "Build SFML from source" ON)
option(BUNDLE_JSON "Build nlohmann_json from source" ON)

# --- FetchContent ---
include(FetchContent)

# ===== nlohmann/json (из исходников) =====
if(BUNDLE_JSON)
  FetchContent_Declare(
    nlohmann_json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG        v3.11.3
    GIT_SHALLOW    TRUE
  )
  FetchContent_MakeAvailable(nlohmann_json)
endif()

# ===== SFML 3.0.1 (из исходников) =====
if(BUNDLE_SFML)
  # Параметры сборки самой SFML: отключаем лишнее
  set(SFML_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
  set(SFML_BUILD_TESTS    OFF CACHE BOOL "" FORCE)
  set(SFML_BUILD_DOC      OFF CACHE BOOL "" FORCE)

  # Прим.: на Linux всё равно нужны системные dev-библиотеки X11/OpenGL/ALSA/… (см. workflow)
  FetchContent_Declare(
    SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG        3.0.1
    GIT_SHALLOW    TRUE
  )
  FetchContent_MakeAvailable(SFML)
endif()

# ===== Твой исполняемый файл =====
add_executable(${PROJECT_NAME}
  source/main.cpp
)

target_include_directories(${PROJECT_NAME}
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/source
)

# ===== Линковка =====
# таргет json создаётся как nlohmann_json::nlohmann_json
# таргеты SFML 3 — SFML::Graphics/Audio/Window/System
target_link_libraries(${PROJECT_NAME}
  PRIVATE
    SFML::Graphics
    SFML::Audio
    SFML::Window
    SFML::System
    nlohmann_json::nlohmann_json
)

# ===== Ресурсы рядом с бинарём =====
add_custom_command(TARGET ${PROJECT_NAME} PRE_BUILD
  COMMAND ${CMAKE_COMMAND} -E rm -rf
          $<TARGET_FILE_DIR:${PROJECT_NAME}>/assets
  COMMAND ${CMAKE_COMMAND} -E copy_directory
          ${CMAKE_CURRENT_SOURCE_DIR}/assets
          $<TARGET_FILE_DIR:${PROJECT_NAME}>/assets
  COMMENT "Copy assets"
)

add_custom_command(TARGET ${PROJECT_NAME} PRE_BUILD
  COMMAND ${CMAKE_COMMAND} -E rm -rf
          $<TARGET_FILE_DIR:${PROJECT_NAME}>/config
  COMMAND ${CMAKE_COMMAND} -E copy_directory
          ${CMAKE_CURRENT_SOURCE_DIR}/config
          $<TARGET_FILE_DIR:${PROJECT_NAME}>/config
  COMMENT "Copy config"
)

# ===== RPATH для Linux =====
set(CMAKE_BUILD_WITH_INSTALL_RPATH OFF)
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH ON)
set(CMAKE_INSTALL_RPATH "$ORIGIN;$ORIGIN/lib")

# ===== Установка (stage) =====
include(GNUInstallDirs)
install(TARGETS ${PROJECT_NAME}
  RUNTIME DESTINATION .
)
install(DIRECTORY assets config
  DESTINATION .
)

# Автоподкладка зависимостей рядом с бинарём при install()
install(CODE [[
  message(STATUS "Running fixup_bundle for ${CMAKE_PROJECT_NAME}")
  include(BundleUtilities)
  set(app "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/HeartStringsFunnyBones${CMAKE_EXECUTABLE_SUFFIX}")

  if(DEFINED ENV{FIXUP_SEARCH_DIRS})
    set(_dirs "$ENV{FIXUP_SEARCH_DIRS}")
  elseif(DEFINED FIXUP_SEARCH_DIRS)
    set(_dirs "${FIXUP_SEARCH_DIRS}")
  else()
    set(_dirs "")
  endif()
  message(STATUS "FIXUP_SEARCH_DIRS='${_dirs}'")

  fixup_bundle("${app}" "" "${_dirs}")
  message(STATUS "fixup_bundle completed")
]])
