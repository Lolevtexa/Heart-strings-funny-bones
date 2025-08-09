cmake_minimum_required(VERSION 3.16)

# Heart-strings-funny-bones
project(HeartStringsFunnyBones VERSION 0.1 LANGUAGES CXX)

# ===== C++ стандарт =====
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ===== nlohmann_json: только системная библиотека =====
# На Linux ставится пакетом: sudo apt install nlohmann-json3-dev
# На Windows/др. системах — будь то vcpkg/системный инсталлятор — пакет должен
# предоставлять целевой таргет nlohmann_json::nlohmann_json.
find_package(nlohmann_json CONFIG REQUIRED)

# ===== SFML 3.0.1 (динамические библиотеки) =====
# Если SFML установлен в /usr/local (как в твоём WSL) — добавим этот префикс.
# Не критично на системах, где SFML уже лежит в стандартных путях.
list(APPEND CMAKE_PREFIX_PATH "/usr/local")

# Важно: для SFML 3 используем CONFIG + компоненты с заглавной буквы.
find_package(SFML 3.0.1 CONFIG REQUIRED COMPONENTS Graphics Audio Window System)

# ===== Исполняемый файл =====
add_executable(${PROJECT_NAME}
  source/main.cpp
)

target_include_directories(${PROJECT_NAME}
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/source
)

# Линкуем таргеты SFML 3 и системный nlohmann_json
target_link_libraries(${PROJECT_NAME}
  PRIVATE
    SFML::Graphics
    SFML::Audio
    SFML::Window
    SFML::System
    nlohmann_json::nlohmann_json
)

# ===== Копирование ресурсов рядом с бинарём (на этапе сборки) =====
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
# $ORIGIN — папка с бинарём; можно также использовать $ORIGIN/lib при желании.
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
  include(BundleUtilities)
  set(app "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/HeartStringsFunnyBones${CMAKE_EXECUTABLE_SUFFIX}")

  # сначала ENV, если нет — тогда CMake cache
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
