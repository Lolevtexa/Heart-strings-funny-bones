cmake_minimum_required(VERSION 3.16)

project(HeartStringsFunnyBones VERSION 0.1 LANGUAGES CXX)

# ---------------- C++ стандарт ----------------
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ---------------- Всегда бандлим зависимости из исходников ----------------
include(FetchContent)

# nlohmann/json (header-only)
FetchContent_Declare(
  nlohmann_json
  GIT_REPOSITORY https://github.com/nlohmann/json.git
  GIT_TAG        v3.11.3
  GIT_SHALLOW    TRUE
)
FetchContent_MakeAvailable(nlohmann_json)

# SFML 3.0.1
# (отключаем примеры/тесты/доки, чтобы ускорить сборку)
set(SFML_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(SFML_BUILD_TESTS    OFF CACHE BOOL "" FORCE)
set(SFML_BUILD_DOC      OFF CACHE BOOL "" FORCE)

# По умолчанию используем статические библиотеки SFML.
# Это самое беспроблемное поведение на Windows (меньше возни с DLL).
# Если захочешь shared, просто выставь BUILD_SHARED_LIBS=ON при конфигурации.
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
  SFML
  GIT_REPOSITORY https://github.com/SFML/SFML.git
  GIT_TAG        3.0.1
  GIT_SHALLOW    TRUE
)
FetchContent_MakeAvailable(SFML)

# ---------------- Исходники проекта ----------------
# Если предпочитаешь явный список файлов — замени этот блок на add_executable(...) с перечислением .cpp
file(GLOB_RECURSE PROJECT_SOURCES CONFIGURE_DEPENDS
  ${CMAKE_CURRENT_SOURCE_DIR}/source/*.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/source/*.cxx
  ${CMAKE_CURRENT_SOURCE_DIR}/source/*.cc
)

if(PROJECT_SOURCES STREQUAL "")
  message(FATAL_ERROR "Не найдены исходники в каталоге source/. Добавь .cpp файлы или поправь GLOB.")
endif()

add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})

target_include_directories(${PROJECT_NAME}
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/source
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

# ---------------- Копирование ресурсов рядом с бинарём ----------------
# При каждой сборке/запуске в VS Code ресурсы будут лежать рядом с исполняемым файлом.
add_custom_command(TARGET ${PROJECT_NAME} PRE_BUILD
  COMMAND ${CMAKE_COMMAND} -E remove_directory
          $<TARGET_FILE_DIR:${PROJECT_NAME}>/assets
  COMMAND ${CMAKE_COMMAND} -E copy_directory
          ${CMAKE_CURRENT_SOURCE_DIR}/assets
          $<TARGET_FILE_DIR:${PROJECT_NAME}>/assets
  COMMENT "Copy assets -> $<TARGET_FILE_DIR:${PROJECT_NAME}>/assets"
)

add_custom_command(TARGET ${PROJECT_NAME} PRE_BUILD
  COMMAND ${CMAKE_COMMAND} -E remove_directory
          $<TARGET_FILE_DIR:${PROJECT_NAME}>/config
  COMMAND ${CMAKE_COMMAND} -E copy_directory
          ${CMAKE_CURRENT_SOURCE_DIR}/config
          $<TARGET_FILE_DIR:${PROJECT_NAME}>/config
  COMMENT "Copy config -> $<TARGET_FILE_DIR:${PROJECT_NAME}>/config"
)

# ---------------- Установка (install) ----------------
# Делаем такую же структуру, как в логе CI: бинарь в корне префикса, рядом assets/ и config/.
include(GNUInstallDirs)

install(TARGETS ${PROJECT_NAME}
  RUNTIME DESTINATION .            # .exe / ELF рядом в корне dist/<OS>
  ARCHIVE DESTINATION lib          # если будет нужен статический архив проекта
  LIBRARY DESTINATION lib          # на случай сборки как SHARED
)

install(DIRECTORY assets config
  DESTINATION .
)

# ВАЖНО:
# 1) Мы убрали блок с fixup_bundle, т.к. он ломал установку на Windows.
# 2) Для Windows используем статическую сборку SFML (BUILD_SHARED_LIBS=OFF),
#    чтобы .exe не требовал рядом DLL от SFML — так проще публиковать артефакты.
# 3) На Linux статические sfml-*.a всё равно подтянут системные so (X11/OpenGL и т.п.) в рантайме,
#    поэтому в CI нужно ставить dev-пакеты (libx11-dev, libxi-dev, libgl1-mesa-dev, ...).
#    Это у тебя уже добавлено/мы добавляли выше.
#
# Если захочешь именно SHARED на Linux (чтобы класть .so в dist/Linux/lib и запускать на «чистой» системе),
# конфигурируй так:
#   cmake -S . -B build -G Ninja -DBUILD_SHARED_LIBS=ON
# и при необходимости добавь RPATH:
#   set(CMAKE_BUILD_WITH_INSTALL_RPATH OFF)
#   set(CMAKE_INSTALL_RPATH_USE_LINK_PATH ON)
#   set(CMAKE_INSTALL_RPATH "$ORIGIN;$ORIGIN/lib")
