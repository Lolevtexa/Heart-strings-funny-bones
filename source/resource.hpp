/**
 * @file resource.hpp
 * @brief Единая точка доступа к ресурсам: шрифт, цвета, фон, музыка, JSON
 * (локализация/настройки).
 *
 * Содержит пути, статические синглтон-подобные объекты и утилиты:
 *  - listLocalizations(): список файлов локализаций;
 *  - loadJson(): безопасная загрузка JSON;
 *  - updateJson(): запись userSettings на диск.
 */
#pragma once
#include <SFML/Audio/Music.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Color.hpp>
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>

/**
 * @brief Менеджер ресурсов приложения.
 *
 * Замечание: статические поля инициализируются ниже файла «функциональными
 * статическими» лямбдами.
 */
class Resource {
public:
  // Параметры отрисовки текста
  static const int characterSize = 24;
  static const int lineSpacing = characterSize / 2;

  // Пути к ресурсам/конфигам
  inline static const std::string PATH_SETTINGS = "config/settings.json";
  inline static const std::string PATH_LOCALIZATIONS_DIR =
      "assets/localizations";
  inline static const std::string PATH_FONT = "assets/fonts/arial.ttf";
  inline static const std::string PATH_MUSIC =
      "assets/music/hound-dog-jazz.mp3";
  inline static const std::string PATH_BACKGROUND =
      "assets/images/mainMenu/background.png";

  // Ресурсы/данные, доступные глобально
  static const sf::Font defaultFont;

  static nlohmann::json localization; ///< Текущая локализация.
  static nlohmann::json userSettings; ///< Пользовательские настройки (в т.ч.
                                      ///< язык/режим экрана/громкость).

  static const sf::Color focusedColor;
  static const sf::Color unfocusedColor;
  static const sf::Color transparentColor;

  static sf::Texture background;

  /**
   * @brief Загрузить музыку из файла PATH_MUSIC.
   * @return Указатель на созданный sf::Music (владелец — вызывающая сторона).
   */
  static sf::Music *loadBackgroundMusic() {
    sf::Music *music = new sf::Music();
    if (!music->openFromFile(PATH_MUSIC)) {
      throw std::runtime_error("Cannot load music");
    }
    return music;
  }

  /**
   * @brief Получить список путей JSON-локализаций в директории
   * PATH_LOCALIZATIONS_DIR.
   * @return Вектор полных путей к .json файлам.
   *
   * Ошибки файловой системы печатаются в stderr, но не приводят к исключениям.
   */
  static std::vector<std::string> listLocalizations() {
    std::vector<std::string> jsonFiles;
    try {
      if (!std::filesystem::exists(PATH_LOCALIZATIONS_DIR) ||
          !std::filesystem::is_directory(PATH_LOCALIZATIONS_DIR)) {
        std::cerr << "Directory does not exist or is not a directory."
                  << std::endl;
        return jsonFiles;
      }

      for (const auto &entry :
           std::filesystem::directory_iterator(PATH_LOCALIZATIONS_DIR)) {
        if (entry.is_regular_file()) {
          auto path = entry.path();
          if (path.extension() == ".json") {
            jsonFiles.push_back(PATH_LOCALIZATIONS_DIR + "/" +
                                path.filename().string());
          }
        }
      }
    } catch (const std::filesystem::filesystem_error &e) {
      std::cerr << "Filesystem error: " << e.what() << std::endl;
    } catch (const std::exception &e) {
      std::cerr << "Standard exception: " << e.what() << std::endl;
    }
    return jsonFiles;
  }

  /**
   * @brief Безопасно загрузить JSON из файла.
   * @param filename Полный путь к JSON.
   * @return Содержимое JSON (пустой объект при ошибке).
   */
  static nlohmann::json loadJson(const std::string &filename) {
    nlohmann::json j;
    try {
      j = nlohmann::json();
      std::ifstream file(filename);
      file >> j;
    } catch (const std::exception &e) {
      std::cerr << "Cannot load localization: " << filename << " " << e.what()
                << std::endl;
    }
    return j;
  }

  /**
   * @brief Записать текущие userSettings в PATH_SETTINGS.
   *
   * Вызывать периодически (например, раз в кадр) допустимо — файл небольшой.
   */
  static void updateJson() {
    try {
      std::ofstream file(PATH_SETTINGS);
      file << userSettings;
    } catch (const std::exception &e) {
      std::cerr << "Cannot write localization: " << PATH_SETTINGS << " "
                << e.what() << std::endl;
    }
  }
};

// ===== Инициализация статических полей =====

const sf::Font Resource::defaultFont = []() {
  sf::Font font;
  if (!font.openFromFile(PATH_FONT)) {
    throw std::runtime_error("Cannot load font");
  }
  return font;
}();

nlohmann::json Resource::userSettings = Resource::loadJson(PATH_SETTINGS);
nlohmann::json Resource::localization =
    Resource::loadJson(Resource::userSettings["localization"]);

const sf::Color Resource::focusedColor = []() { return sf::Color(0, 0, 0); }();
const sf::Color Resource::unfocusedColor = []() {
  return sf::Color(95, 95, 95);
}();
const sf::Color Resource::transparentColor = []() {
  return sf::Color(0, 0, 0, 0);
}();

sf::Texture Resource::background = []() {
  sf::Texture texture;
  if (!texture.loadFromFile(PATH_BACKGROUND)) {
    throw std::runtime_error("Cannot load texture");
  }
  return texture;
}();

/**
 * @brief Утилита: преобразовать UTF-8 строку в std::wstring (для SFML заголовка
 * окна).
 */
static std::wstring utf8_to_wstring(const std::string &str) {
  std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
  return converter.from_bytes(str);
}
