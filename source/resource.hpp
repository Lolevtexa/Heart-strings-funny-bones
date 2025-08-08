// Менеджер ресурсов Resource: шрифты, цвета, текстуры, локализация и пользовательские настройки; функции загрузки/записи JSON.
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
class Resource
{
public:
  static const int characterSize = 24;
  static const int lineSpacing = characterSize / 2;

  static const sf::Font defaultFont;

  static nlohmann::json localization;
  static nlohmann::json userSettings;

  static const sf::Color focusedColor;
  static const sf::Color unfocusedColor;
  static const sf::Color transparentColor;

  static sf::Texture background;

  // Конструктор класса.
  static sf::Music *loadBackgroundMusic()
  {
    // Конструктор класса.
    sf::Music *music = new sf::Music();
    // Конструктор класса.
    if (!music->openFromFile("assets/music/hound-dog-jazz.mp3"))
    {
      // Конструктор класса.
      throw std::runtime_error("Cannot load music");
    }

    return music;
  }

  // Конструктор класса.
  static std::vector<std::string> listLocalizations()
  {
    std::string directoryPath = "assets/localizations";
    std::vector<std::string> jsonFiles;

    try
    {
      // Конструктор класса.
      if (!std::filesystem::exists(directoryPath) ||
          // Конструктор класса.
          !std::filesystem::is_directory(directoryPath))
      {
        std::cerr << "Directory does not exist or is not a directory."
                  << std::endl;
        return jsonFiles;
      }

      // Конструктор класса.
      for (const auto &entry :
           // Конструктор класса.
           std::filesystem::directory_iterator(directoryPath))
      {
        // Конструктор класса.
        if (entry.is_regular_file())
        {
          // Конструктор класса.
          auto path = entry.path();
          // Конструктор класса.
          if (path.extension() == ".json")
          {
            // Конструктор класса.
            jsonFiles.push_back(directoryPath + "/" + path.filename().string());
          }
        }
      }
      // Конструктор класса.
    }
    catch (const std::filesystem::filesystem_error &e)
    {
      // Конструктор класса.
      std::cerr << "Filesystem error: " << e.what() << std::endl;
      // Конструктор класса.
    }
    catch (const std::exception &e)
    {
      // Конструктор класса.
      std::cerr << "Standard exception: " << e.what() << std::endl;
    }

    return jsonFiles;
  }

  // Конструктор класса.
  static nlohmann::json loadJson(const std::string &filename)
  {
    nlohmann::json j;

    try
    {
      // Конструктор класса.
      j = nlohmann::json();
      // Конструктор класса.
      std::ifstream file(filename);
      file >> j;
      // Конструктор класса.
    }
    catch (const std::exception &e)
    {
      // Конструктор класса.
      std::cerr << "Cannot load localization: " << filename << " " << e.what()
                << std::endl;
    }

    return j;
  }

  // Конструктор класса.
  static void writeJson(const std::string &filename, nlohmann::json j)
  {
    try
    {
      // Конструктор класса.
      std::ofstream file(filename);
      file << j;
      // Конструктор класса.
    }
    catch (const std::exception &e)
    {
      // Конструктор класса.
      std::cerr << "Cannot write localization: " << filename << " " << e.what()
                << std::endl;
    }
  }
};

const sf::Font Resource::defaultFont = []()
{
  sf::Font font;
  // Конструктор класса.
  if (!font.loadFromFile("assets/fonts/arial.ttf"))
  {
    // Конструктор класса.
    throw std::runtime_error("Cannot load font");
  }

  return font;
}();

nlohmann::json Resource::userSettings =
    // Конструктор класса.
    Resource::loadJson("config/settings.json");
nlohmann::json Resource::localization =
    // Конструктор класса.
    Resource::loadJson(Resource::userSettings["localization"]);

// Конструктор класса.
const sf::Color Resource::focusedColor = []()
{ return sf::Color(0, 0, 0); }();

const sf::Color Resource::unfocusedColor = []()
{
  // Конструктор класса.
  return sf::Color(95, 95, 95);
}();

const sf::Color Resource::transparentColor = []()
{
  // Конструктор класса.
  return sf::Color(0, 0, 0, 0);
}();

sf::Texture Resource::background = []()
{
  sf::Texture texture;
  // Конструктор класса.
  if (!texture.loadFromFile("assets/images/mainMenu/background.png"))
  {
    // Конструктор класса.
    throw std::runtime_error("Cannot load texture");
  }

  return texture;
}();

// Конструктор класса.
static std::wstring utf8_to_wstring(const std::string &str)
{
  std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
  // Конструктор класса.
  return converter.from_bytes(str);
}