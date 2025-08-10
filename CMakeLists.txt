cmake_minimum_required(VERSION 3.21)

project(HeartStringsFunnyBones VERSION 0.1 LANGUAGES CXX)

# ---------------- C++ стандарт ----------------
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ---------------- Хотим статические либы (в т.ч. SFML) ----------------
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)

# ---------------- MSVC: статический рантайм (/MT, /MTd) ----------------
if (MSVC)
  # CMake >= 3.15
  set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif()

# ---------------- MinGW (локальные сборки): статлинк рантаймов ----------------
if (MINGW)
  add_link_options(-static-libstdc++ -static-libgcc)
endif()

# ---------------- Зависимости через FetchContent ----------------
include(FetchContent)

# nlohmann/json — header-only
FetchContent_Declare(
  nlohmann_json
  GIT_REPOSITORY https://github.com/nlohmann/json.git
  GIT_TAG        v3.11.3
  GIT_SHALLOW    TRUE
)
FetchContent_MakeAvailable(nlohmann_json)

# SFML 3.0.1 — собираем статически (BUILD_SHARED_LIBS=OFF уже проставлен)
set(SFML_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(SFML_BUILD_TESTS    OFF CACHE BOOL "" FORCE)
set(SFML_BUILD_DOC      OFF CACHE BOOL "" FORCE)
set(SFML_INSTALL        OFF CACHE BOOL "" FORCE)
FetchContent_Declare(
  SFML
  GIT_REPOSITORY https://github.com/SFML/SFML.git
  GIT_TAG        3.0.1
  GIT_SHALLOW    TRUE
)
FetchContent_MakeAvailable(SFML)

# Для совместимости со статической SFML (в 3.x чаще не обязателен, но ок)
add_compile_definitions(SFML_STATIC)

# ---------------- Исходники проекта ----------------
file(GLOB_RECURSE PROJECT_SOURCES CONFIGURE_DEPENDS
  ${CMAKE_CURRENT_SOURCE_DIR}/source/*.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/source/*.cxx
  ${CMAKE_CURRENT_SOURCE_DIR}/source/*.cc
)
if(PROJECT_SOURCES STREQUAL "")
  message(FATAL_ERROR "Не найдены исходники в каталоге source/.")
endif()

add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})

target_include_directories(${PROJECT_NAME}
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/source
)

# ---------------- Линковка ----------------
if (WIN32)
  target_link_libraries(${PROJECT_NAME}
    PRIVATE
      SFML::Graphics
      SFML::Audio
      SFML::Window
      SFML::System
      nlohmann_json::nlohmann_json
      # Системные либы
      winmm
      gdi32
      ws2_32
      opengl32
  )
else()
  target_link_libraries(${PROJECT_NAME}
    PRIVATE
      SFML::Graphics
      SFML::Audio
      SFML::Window
      SFML::System
      nlohmann_json::nlohmann_json
  )
endif()

# ---------------- Вменяемые предупреждения ----------------
if (MSVC)
  target_compile_options(${PROJECT_NAME} PRIVATE /permissive- /Zc:preprocessor)
else()
  target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)
endif()

# ---------------- Копирование ресурсов рядом с бинарём ----------------
foreach(dir assets config)
  add_custom_command(TARGET ${PROJECT_NAME} PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -E remove_directory
            "$<TARGET_FILE_DIR:${PROJECT_NAME}>/${dir}"
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${CMAKE_CURRENT_SOURCE_DIR}/${dir}"
            "$<TARGET_FILE_DIR:${PROJECT_NAME}>/${dir}"
    COMMENT "Copy ${dir} -> $<TARGET_FILE_DIR:${PROJECT_NAME}>/${dir}"
  )
endforeach()

# ---------------- Без общего copy_runtime_dlls (он и не нужен при статике) ----
# Опционально: точечно скопировать openal32.dll, если вдруг она всё же DLL
if (WIN32)
  find_file(OPENAL_DLL NAMES openal32.dll
    PATHS
      "${CMAKE_BINARY_DIR}"
      "$ENV{SystemRoot}/System32" "$ENV{SystemRoot}/SysWOW64"
      "${SFML_SOURCE_DIR}/extlibs/bin/x86" "${SFML_SOURCE_DIR}/extlibs/bin/x64"
    NO_DEFAULT_PATH
  )
  if (OPENAL_DLL)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy_if_different
              "${OPENAL_DLL}"
              "$<TARGET_FILE_DIR:${PROJECT_NAME}>")
  endif()
endif()

# ---------------- Установка: только то, что нужно для запуска ----------------
include(GNUInstallDirs)

install(TARGETS ${PROJECT_NAME}
  RUNTIME DESTINATION .
)

install(DIRECTORY assets config DESTINATION .)

# RPATH — на случай Unix c SHARED (здесь всё статикой, но не мешает)
if (UNIX AND NOT APPLE)
  set(CMAKE_BUILD_WITH_INSTALL_RPATH OFF)
  set(CMAKE_INSTALL_RPATH_USE_LINK_PATH ON)
  set(CMAKE_INSTALL_RPATH "$ORIGIN;$ORIGIN/lib")
endif()

# ---------------- Вывод сводки ----------------
message(STATUS "================= BUILD SUMMARY =================")
message(STATUS "CMAKE_BUILD_TYPE      : ${CMAKE_BUILD_TYPE}")
message(STATUS "BUILD_SHARED_LIBS     : ${BUILD_SHARED_LIBS}")
message(STATUS "Generator             : ${CMAKE_GENERATOR}")
message(STATUS "C++ Compiler          : ${CMAKE_CXX_COMPILER}")
message(STATUS "Sources found         : ${PROJECT_SOURCES}")
message(STATUS "==================================================")
