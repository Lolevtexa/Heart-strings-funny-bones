cmake_minimum_required(VERSION 3.16)

# Heart-strings-funny-bones
project(HeartStringsFunnyBones VERSION 0.1 LANGUAGES CXX)

# ===== C++ стандарт =====
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ===== nlohmann_json: через FetchContent по умолчанию =====
option(USE_SYSTEM_JSON "Use system provided nlohmann_json if available" OFF)
include(FetchContent)
if(NOT USE_SYSTEM_JSON)
  FetchContent_Declare(
    nlohmann_json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG v3.11.2
  )
  FetchContent_MakeAvailable(nlohmann_json)
endif()

# ===== SFML (динамические библиотеки) =====
set(SFML_STATIC_LIBRARIES OFF CACHE BOOL "Build with SFML static libraries")
find_package(SFML 2.5 COMPONENTS graphics audio window system REQUIRED)

# ===== Исполняемый файл =====
add_executable(${PROJECT_NAME}
  source/main.cpp
)

target_include_directories(${PROJECT_NAME}
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/source
)

target_link_libraries(${PROJECT_NAME}
  PRIVATE
    sfml-graphics
    sfml-audio
    sfml-window
    sfml-system
    nlohmann_json::nlohmann_json
)

# ===== Копирование ресурсов в папку рядом с бинарём (во время сборки) =====
add_custom_command(TARGET ${PROJECT_NAME} PRE_BUILD
  COMMAND ${CMAKE_COMMAND} -E remove_directory
          $<TARGET_FILE_DIR:${PROJECT_NAME}>/assets
  COMMAND ${CMAKE_COMMAND} -E copy_directory
          ${CMAKE_CURRENT_SOURCE_DIR}/assets
          $<TARGET_FILE_DIR:${PROJECT_NAME}>/assets
  COMMENT "Cleaning and copying assets directory to build output"
)

add_custom_command(TARGET ${PROJECT_NAME} PRE_BUILD
  COMMAND ${CMAKE_COMMAND} -E remove_directory
          $<TARGET_FILE_DIR:${PROJECT_NAME}>/config
  COMMAND ${CMAKE_COMMAND} -E copy_directory
          ${CMAKE_CURRENT_SOURCE_DIR}/config
          $<TARGET_FILE_DIR:${PROJECT_NAME}>/config
  COMMENT "Cleaning and copying config directory to build output"
)

# ===== RPATH для Linux, чтобы .so искались рядом с бинарём =====
set(CMAKE_BUILD_WITH_INSTALL_RPATH OFF)
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH ON)
# $ORIGIN — папка с бинарём; при желании можно класть библиотеки в ./lib и оставить обе записи
set(CMAKE_INSTALL_RPATH "$ORIGIN;$ORIGIN/lib")

# ===== Установка (staging) в дистрибутивную папку =====
include(GNUInstallDirs)
install(TARGETS ${PROJECT_NAME}
  RUNTIME DESTINATION .    # exe/elf в корень установленной папки
)
install(DIRECTORY assets config
  DESTINATION .
)

# ===== Автоматическое подтягивание зависимостей (DLL/SO) рядом с бинарём =====
# ВАЖНО: BundleUtilities подключаем ТОЛЬКО в install(CODE), не на configure!
install(CODE [[
  message(STATUS "Running fixup_bundle for ${CMAKE_PROJECT_NAME}")

  # Тут уже выполняется "на установке", поэтому можно подключить модуль:
  include(BundleUtilities)

  # Путь к установленному исполняемому файлу
  set(app "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/HeartStringsFunnyBones${CMAKE_EXECUTABLE_SUFFIX}")
  if(NOT EXISTS "${app}")
    message(FATAL_ERROR "Executable not found at install path: ${app}")
  endif()

  # Пути, где искать зависимости (передаём из CI через -DFIXUP_SEARCH_DIRS=...).
  set(_dirs "")
  if(DEFINED FIXUP_SEARCH_DIRS)
    set(_dirs "${FIXUP_SEARCH_DIRS}")
    message(STATUS "FIXUP_SEARCH_DIRS='${_dirs}'")
  else()
    message(WARNING "FIXUP_SEARCH_DIRS is not defined; fixup_bundle may miss some libs")
  endif()

  # Копируем зависимые dll/so рядом с бинарём и чиним rpath при необходимости.
  fixup_bundle("${app}" "" "${_dirs}")

  message(STATUS "fixup_bundle completed")
]])
