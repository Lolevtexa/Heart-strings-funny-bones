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

# --- Статический рантайм MinGW + гарантированное копирование DLL (если нужно) ---
if(MINGW)
  # 1) статический libgcc/libstdc++
  target_link_options(${PROJECT_NAME} PRIVATE -static-libgcc -static-libstdc++)

  # 2) попробовать притянуть статическую libwinpthread.a
  execute_process(
    COMMAND ${CMAKE_CXX_COMPILER} -print-file-name=libwinpthread.a
    OUTPUT_VARIABLE WINPTHREAD_A
    OUTPUT_STRIP_TRAILING_WHITESPACE
  )
  if(WINPTHREAD_A AND EXISTS "${WINPTHREAD_A}")
    message(STATUS "Using static winpthread: ${WINPTHREAD_A}")
    target_link_libraries(${PROJECT_NAME} PRIVATE "${WINPTHREAD_A}")
  else()
    message(WARNING "Static libwinpthread.a not found; will ship libwinpthread-1.dll")
  endif()
endif()

# ---------------- Деплой ресурсов (ОТДЕЛЬНАЯ ЦЕЛЬ) ----------------
# Избегаем PRE/POST_BUILD, чтобы не раздувать командную строку линковки.
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
# Мини-скрипт, который НЕ падает при пустом списке.
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

# Копируем любые runtime-DLL, которые CMake знает (обычно пусто при статике)
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

# --- План Б для MinGW: найти реальные пути к MinGW-DLL и скопировать рядом с .exe
if(MINGW)
  function(_detect_runtime_dll _soname _outvar)
    execute_process(
      COMMAND ${CMAKE_CXX_COMPILER} -print-file-name=${_soname}
      OUTPUT_VARIABLE _dll_path
      OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    if(NOT _dll_path OR _dll_path STREQUAL "${_soname}" OR NOT EXISTS "${_dll_path}")
      get_filename_component(_cc_bin "${CMAKE_CXX_COMPILER}" DIRECTORY)
      set(_dll_path "${_cc_bin}/${_soname}")
    endif()
    if(EXISTS "${_dll_path}")
      set(${_outvar} "${_dll_path}" PARENT_SCOPE)
    else()
      set(${_outvar} "" PARENT_SCOPE)
    endif()
  endfunction()

  _detect_runtime_dll("libwinpthread-1.dll" WINPTHREAD_DLL)
  _detect_runtime_dll("libstdc++-6.dll"     LIBSTDCXX_DLL)
  _detect_runtime_dll("libgcc_s_seh-1.dll"  LIBGCC_DLL)

  set(MINGW_RUNTIME_DLLS "")
  if(WINPTHREAD_DLL) list(APPEND MINGW_RUNTIME_DLLS "${WINPTHREAD_DLL}") endif()
  if(LIBSTDCXX_DLL)  list(APPEND MINGW_RUNTIME_DLLS "${LIBSTDCXX_DLL}")  endif()
  if(LIBGCC_DLL)     list(APPEND MINGW_RUNTIME_DLLS "${LIBGCC_DLL}")     endif()

  add_custom_target(deploy_mingw_runtime ALL
    COMMAND ${CMAKE_COMMAND}
            -Dsrc="${MINGW_RUNTIME_DLLS}"
            -Ddst="$<TARGET_FILE_DIR:${PROJECT_NAME}>"
            -P "${COPY_DLLS_SCRIPT}"
    COMMENT "Copy MinGW runtime DLLs if present (fallback)"
  )
  add_dependencies(deploy_mingw_runtime ${PROJECT_NAME})
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

# Простой и корректный способ установить runtime-DLL (если есть)
if (WIN32)
  install(FILES $<TARGET_RUNTIME_DLLS:${PROJECT_NAME}> DESTINATION .)
endif()

# ---------------- Сводка ----------------
message(STATUS "================= BUILD SUMMARY =================")
message(STATUS "CMAKE_BUILD_TYPE      : ${CMAKE_BUILD_TYPE}")
message(STATUS "BUILD_SHARED_LIBS     : ${BUILD_SHARED_LIBS}")
message(STATUS "Generator             : ${CMAKE_GENERATOR}")
message(STATUS "C++ Compiler          : ${CMAKE_CXX_COMPILER}")
message(STATUS "Sources found         : ${PROJECT_SOURCES}")
message(STATUS "==================================================")
