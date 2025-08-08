// Менеджер ресурсов Resource: шрифты, цвета, текстуры, локализация и
// пользовательские настройки; функции загрузки/записи JSON.
// ------------------------------------------------------------

// Заголовочный файл. pragma once — защита от множественного включения.
#pragma once
#include <SFML/Audio/Music.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>

// Класс Resource — см. описание в заголовке файла.
class Resource {
public:
  static const int characterSize = 24;
  static const int lineSpacing = characterSize / 2;

  inline static const std::string PATH_SETTINGS = "config/settings.json";
  inline static const std::string PATH_LOCALIZATIONS_DIR =
      "assets/localizations";
  inline static const std::string PATH_FONT = "assets/fonts/arial.ttf";
  inline static const std::string PATH_MUSIC =
      "assets/music/hound-dog-jazz.mp3";
  inline static const std::string PATH_BACKGROUND =
      "assets/images/mainMenu/background.png";

  static const sf::Font defaultFont;

  static nlohmann::json localization;
  static nlohmann::json userSettings;

  static const sf::Color focusedColor;
  static const sf::Color unfocusedColor;
  static const sf::Color transparentColor;

  static sf::Texture background;

  static sf::Music *loadBackgroundMusic() {
    sf::Music *music = new sf::Music();
    if (!music->openFromFile(PATH_MUSIC        )) {
      throw std::runtime_error("Cannot load music");
    }

    return music;
  }

  static std::vector<std::string> listLocalizations() {
    std::vector<std::string> jsonFiles;

    try {
      if (!std::filesystem::exists(PATH_LOCALIZATIONS_DIR ) ||
          !std::filesystem::is_directory(PATH_LOCALIZATIONS_DIR )) {
        std::cerr << "Directory does not exist or is not a directory."
                  << std::endl;
        return jsonFiles;
      }

      for (const auto &entry :
           std::filesystem::directory_iterator(PATH_LOCALIZATIONS_DIR )) {
        if (entry.is_regular_file()) {
          auto path = entry.path();
          if (path.extension() == ".json") {
            jsonFiles.push_back(PATH_LOCALIZATIONS_DIR  + "/" + path.filename().string());
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

  static void updateJson() {
    try {
      std::ofstream file(PATH_SETTINGS);
      file << userSettings;
    } catch (const std::exception &e) {
      std::cerr << "Cannot write localization: " << PATH_SETTINGS << " " << e.what()
                << std::endl;
    }
  }
};

const sf::Font Resource::defaultFont = []() {
  sf::Font font;
  if (!font.loadFromFile(PATH_FONT         )) {
    throw std::runtime_error("Cannot load font");
  }

  return font;
}();

nlohmann::json Resource::userSettings =
    Resource::loadJson(PATH_SETTINGS);
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

static std::wstring utf8_to_wstring(const std::string &str) {
  std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
  return converter.from_bytes(str);
}