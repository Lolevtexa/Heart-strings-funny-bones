cmake_minimum_required(VERSION 3.21)

project(HeartStringsFunnyBones VERSION 0.1 LANGUAGES CXX)

# ---------------- C++ стандарт ----------------
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ---------------- Статическая линковка библиотек ----------------
# Максимально уменьшаем внешние DLL: SFML и др. статически.
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)

# ---------------- Зависимости ----------------
include(FetchContent)

# nlohmann/json — header-only
FetchContent_Declare(
  nlohmann_json
  GIT_REPOSITORY https://github.com/nlohmann/json.git
  GIT_TAG        v3.11.3
  GIT_SHALLOW    TRUE
)
FetchContent_MakeAvailable(nlohmann_json)

# SFML 3.0.1 (при BUILD_SHARED_LIBS=OFF соберётся статически)
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
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/source
)

# Для статической SFML на Windows нужен дефайн SFML_STATIC
target_compile_definitions(${PROJECT_NAME}
  PRIVATE $<$<BOOL:${WIN32}>:SFML_STATIC>
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

# --- Статический рантайм MinGW (только для сборок MinGW/Win32) ---
if(MINGW)
  # Попробовать полностью статически, если это допустимо для твоего проекта:
  # (Если начнутся конфликты с другими стат. библиотеками — вернись к более мягкому варианту)
  target_link_options(${PROJECT_NAME} PRIVATE -static -static-libgcc -static-libstdc++)
  target_link_options(${PROJECT_NAME} PRIVATE -Wl,-Bstatic)
  target_link_options(${PROJECT_NAME} PRIVATE -lwinpthread)
  target_link_options(${PROJECT_NAME} PRIVATE -Wl,-Bdynamic)
endif()

# ---------------- Деплой ресурсов (ОТДЕЛЬНАЯ ЦЕЛЬ) ----------------
add_custom_target(deploy_assets ALL
  COMMAND ${CMAKE_COMMAND} -E rm -rf
          $<TARGET_FILE_DIR:${PROJECT_NAME}>/assets
  COMMAND ${CMAKE_COMMAND} -E copy_directory
          ${CMAKE_CURRENT_SOURCE_DIR}/assets
          $<TARGET_FILE_DIR:${PROJECT_NAME}>/assets
  COMMAND ${CMAKE_COMMAND} -E rm -rf
          $<TARGET_FILE_DIR:${PROJECT_NAME}>/config
  COMMAND ${CMAKE_COMMAND} -E copy_directory
          ${CMAKE_CURRENT_SOURCE_DIR}/config
          $<TARGET_FILE_DIR:${PROJECT_NAME}>/config
  COMMENT "Deploy assets/ and config/ to runtime dir"
)
add_dependencies(deploy_assets ${PROJECT_NAME})

# ---------------- Безопасное копирование runtime-DLL (если вдруг есть) ----------------
# Пишем мини-скрипт, который НЕ падает при пустом списке DLL.
set(COPY_DLLS_SCRIPT "${CMAKE_BINARY_DIR}/copy_runtime_dlls.cmake")
file(WRITE "${COPY_DLLS_SCRIPT}" "
if(NOT DEFINED src OR src STREQUAL \"\")
  message(STATUS \"No runtime DLLs to copy\")
  return()
endif()
if(NOT DEFINED dst OR dst STREQUAL \"\")
  message(FATAL_ERROR \"Destination directory 'dst' not set\")
endif()
file(MAKE_DIRECTORY \"\${dst}\")
foreach(f IN LISTS src)
  if(EXISTS \"\${f}\")
    file(COPY \"\${f}\" DESTINATION \"\${dst}\")
  endif()
endforeach()
")

if (WIN32)
  add_custom_target(deploy_runtime_dlls ALL
    COMMAND ${CMAKE_COMMAND}
            -Dsrc="$<TARGET_RUNTIME_DLLS:${PROJECT_NAME}>"
            -Ddst="$<TARGET_FILE_DIR:${PROJECT_NAME}>"
            -P "${COPY_DLLS_SCRIPT}"
    COMMENT "Copy runtime DLLs (if any)"
  )
  add_dependencies(deploy_runtime_dlls ${PROJECT_NAME})
endif()

# План Б: копирование MinGW runtime, если статическая линковка чего-то не убрала
if(MINGW)
  get_filename_component(MINGW_BIN "${CMAKE_CXX_COMPILER}" DIRECTORY)

  # Популярные места в MSYS2/MinGW и при кросс-сборке на Ubuntu/Debian
  set(_candidate_bins
    "${MINGW_BIN}"
    "$ENV{MSYSTEM_PREFIX}/bin"                          # MSYS2
    "/usr/x86_64-w64-mingw32/bin" "/usr/i686-w64-mingw32/bin"   # Linux cross
    "/usr/lib/gcc/x86_64-w64-mingw32" "/usr/lib/gcc/i686-w64-mingw32"
  )

  unset(WINPTHREAD_DLL CACHE)
  find_file(WINPTHREAD_DLL
    NAMES libwinpthread-1.dll
    PATHS ${_candidate_bins}
    NO_DEFAULT_PATH)

  if(WINPTHREAD_DLL)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy_if_different
              "${WINPTHREAD_DLL}" "$<TARGET_FILE_DIR:${PROJECT_NAME}>"
      COMMENT "Copy libwinpthread-1.dll"
    )
  else()
    message(WARNING "libwinpthread-1.dll не найден ни в одном из известных путей.
Будет надежда на статическую линковку; иначе EXE потребует DLL рядом.")
  endif()
endif()

# ---------------- RPATH (Linux) ----------------
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

# install для DLL — тоже безопасно (ничего не делает при пустом списке)
if (WIN32)
  install(CODE [[
    set(dlls "$<TARGET_RUNTIME_DLLS:HeartStringsFunnyBones>")
    if(dlls)
      foreach(f IN LISTS dlls)
        if(EXISTS "${f}")
          file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}" TYPE FILE FILES "${f}")
        endif()
      endforeach()
    endif()
  ]])
endif()

# ---------------- Сводка ----------------
message(STATUS "================= BUILD SUMMARY =================")
message(STATUS "CMAKE_BUILD_TYPE      : ${CMAKE_BUILD_TYPE}")
message(STATUS "BUILD_SHARED_LIBS     : ${BUILD_SHARED_LIBS}")
message(STATUS "Generator             : ${CMAKE_GENERATOR}")
message(STATUS "C++ Compiler          : ${CMAKE_CXX_COMPILER}")
message(STATUS "Sources found         : ${PROJECT_SOURCES}")
message(STATUS "==================================================")
