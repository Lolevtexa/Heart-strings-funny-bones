# Heart‑strings‑funny‑bones

## Описание

**Heart‑strings‑funny‑bones** — небольшое приложение/меню на C++ с использованием [SFML]. Оно создаёт окно, загружает ресурсы из каталога `assets` (фон, шрифты, музыка, локализации) и отображает меню с кнопками/ползунками/переключателями. Виджеты лежат в `source/widgets/...`, сцена — в `source/mainScene.hpp`, точка входа — `source/main.cpp`.

## Структура репозитория

```
assets/
  fonts/arial.ttf
  images/background.png
  localizations/*.json
  music/hound-dog-jazz.mp3
  settings.json
source/
  main.cpp
  mainScene.hpp
  resource.hpp
  bound.hpp
  widgets/...
.clangd
.gitignore
README.md
```

## Требования

- C++17 или новее
- SFML (graphics, window, system, audio) — пакет `libsfml-dev` в Linux
- nlohmann/json (заголовочная библиотека) — пакет `nlohmann-json3-dev` в Debian/Ubuntu
- CMake ≥ 3.16 (для описанного ниже варианта сборки)
- (опционально) Clang/Clang++ в качестве компилятора

## Сборка (WSL / Linux, Clang + CMake)

```bash
sudo apt update
sudo apt install -y clang cmake build-essential libsfml-dev nlohmann-json3-dev

git clone https://github.com/Lolevtexa/Heart-strings-funny-bones.git
cd Heart-strings-funny-bones

# Использовать Clang
export CC=clang
export CXX=clang++

# Генерация и сборка
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release --parallel
```

Запуск исполняемого файла из `build/` (имя цели см. в CMakeLists, по умолчанию `heart_strings_funny_bones`).

> **Примечание о графике в WSL:** в Windows 11 с WSLg окна SFML работают «из коробки». Если WSLg нет — установить X‑server (VcXsrv/Xming) и выставить `DISPLAY`.

## Сборка (Windows нативно)

Вариант 1: MSYS2/MinGW
1) Установить MSYS2, затем пакеты: `pacman -S --needed mingw-w64-ucrt-x86_64-{cmake,clang,make,sfml}`  
2) Открыть «MSYS2 UCRT64» терминал и выполнить те же команды `cmake -S . -B build` …

Вариант 2: Visual Studio + vcpkg
1) Установить VS с компонентами C++ и CMake  
2) Через vcpkg: `vcpkg install sfml nlohmann-json` и подключить toolchain к CMake

## Запуск

Убедитесь, что при запуске бинарника текущая рабочая директория — корень репозитория (или рядом лежит папка `assets`), иначе загрузка шрифтов/текстур/музыки и JSON‑файлов не сработает.

## Настройка/локализация

- `assets/settings.json` — хранит настройки (например, режим экрана и громкость). Приложение перезаписывает файл при изменениях в меню.
- `assets/localizations/*.json` — словари локализаций. Файл выбирается по ключу в `settings.json`.

## Благодарности / ресурсы

- Иконки: набор *Fluency Systems Filled* — **Icons8** (см. условия Icons8 для использования).
- Музыка: композиции с **Pixabay** (см. условия лицензии конкретных треков).

## Лицензия

Проект распространяется по лицензии **MIT**. См. файл `LICENSE` (добавьте в корень репозитория). 
Обратите внимание: сторонние медиа‑ресурсы в папке `assets/` могут иметь отдельные условия использования; включайте соответствующие атрибуции и соблюдайте их лицензии.
