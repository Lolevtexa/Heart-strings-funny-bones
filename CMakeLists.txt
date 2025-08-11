cmake_minimum_required(VERSION 3.21) # для $<TARGET_RUNTIME_DLLS:>

project(HeartStringsFunnyBones VERSION 0.1 LANGUAGES CXX)

# ---------------- C++ стандарт ----------------
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ---------------- Статическая линковка библиотек ----------------
# Цель: максимально убрать внешние DLL, встроив код библиотек в .exe
# Включаем статическую линковку как на Windows, так и на Linux.
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)

# ---------------- Тянем зависимости из исходников ----------------
include(FetchContent)

# nlohmann/json — header-only
FetchContent_Declare(
  nlohmann_json
  GIT_REPOSITORY https://github.com/nlohmann/json.git
  GIT_TAG        v3.11.3
  GIT_SHALLOW    TRUE
)
FetchContent_MakeAvailable(nlohmann_json)

# SFML 3.0.1
set(SFML_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(SFML_BUILD_TESTS    OFF CACHE BOOL "" FORCE)
set(SFML_BUILD_DOC      OFF CACHE BOOL "" FORCE)
set(SFML_INSTALL        OFF CACHE BOOL "" FORCE)
# Важно: при BUILD_SHARED_LIBS=OFF SFML собирается статически.

FetchContent_Declare(
  SFML
  GIT_REPOSITORY https://github.com/SFML/SFML.git
  GIT_TAG        3.0.1
  GIT_SHALLOW    TRUE
)
FetchContent_MakeAvailable(SFML)

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

# Если линкуемся со статической SFML на Windows, нужен дефайн SFML_STATIC
target_compile_definitions(${PROJECT_NAME}
  PRIVATE
    $<$<BOOL:${WIN32}>:SFML_STATIC>
)

# ---------------- Линковка ----------------
target_link_libraries(${PROJECT_NAME}
  PRIVATE
    SFML::Graphics
    SFML::Audio
    SFML::Window
    SFML::System
    nlohmann_json::nlohmann_json
)

# ---------------- Копирование ресурсов для запуска ----------------
foreach(dir assets config)
  add_custom_command(TARGET ${PROJECT_NAME} PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -E remove_directory
            $<TARGET_FILE_DIR:${PROJECT_NAME}>/${dir}
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_CURRENT_SOURCE_DIR}/${dir}
            $<TARGET_FILE_DIR:${PROJECT_NAME}>/${dir}
    COMMENT "Copy ${dir} -> $<TARGET_FILE_DIR:${PROJECT_NAME}>/${dir}"
  )
endforeach()

# ---------------- Автокопирование DLL (Windows, на всякий случай) ----------------
# При статической SFML DLL почти не будет, но если останутся внешние (например OpenAL/sndfile),
# и CMake их сможет распознать как runtime-зависимости, они будут скопированы.
if (WIN32)
  add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_RUNTIME_DLLS:${PROJECT_NAME}>
            $<TARGET_FILE_DIR:${PROJECT_NAME}>
    COMMAND_EXPAND_LISTS
  )
endif()

# ---------------- RPATH (для Linux, если когда-то соберём SHARED) ----------------
if (UNIX AND NOT APPLE)
  set(CMAKE_BUILD_WITH_INSTALL_RPATH OFF)
  set(CMAKE_INSTALL_RPATH_USE_LINK_PATH ON)
  set(CMAKE_INSTALL_RPATH "$ORIGIN;$ORIGIN/lib")
endif()

# ---------------- Установка ----------------
include(GNUInstallDirs)

install(TARGETS ${PROJECT_NAME}
  RUNTIME DESTINATION .
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
)

install(DIRECTORY assets config DESTINATION .)

if (WIN32)
  install(FILES $<TARGET_RUNTIME_DLLS:${PROJECT_NAME}> DESTINATION .)
endif()

# ---------------- Вывод сводки ----------------
message(STATUS "================= BUILD SUMMARY =================")
message(STATUS "CMAKE_BUILD_TYPE      : ${CMAKE_BUILD_TYPE}")
message(STATUS "BUILD_SHARED_LIBS     : ${BUILD_SHARED_LIBS}")
message(STATUS "Generator             : ${CMAKE_GENERATOR}")
message(STATUS "C++ Compiler          : ${CMAKE_CXX_COMPILER}")
message(STATUS "Sources found         : ${PROJECT_SOURCES}")
message(STATUS "==================================================")
