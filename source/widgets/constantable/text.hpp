// CText — отрисовка многострочного текста: перенос по словам под заданную ширину; поддержка локализации.
// ------------------------------------------------------------

// Заголовочный файл. pragma once — защита от множественного включения.
#pragma once
#include "../../resource.hpp"
#include "../constantable.hpp"
#include <SFML/Graphics/Text.hpp>

#define TEXT_BY_KEY 0
#define TEXT_BY_STRING 1

// Класс CText — см. описание в заголовке файла.
class CText : virtual public Constantable
{
protected:
  int type;

  std::vector<std::string> localizationKeys;
  std::wstring text;

  sf::Color textColor = Resource::focusedColor;
  std::vector<sf::Text> drawableText;

public:
  // Конструктор класса.
  CText(const std::wstring &string)
      // Конструктор класса.
      : text(removeOverspaces(string + L" ")), type(TEXT_BY_STRING)
  {
  }

  // Конструктор класса.
  CText(const std::vector<std::string> &localizationKeys)
      // Конструктор класса.
      : localizationKeys(localizationKeys), type(TEXT_BY_KEY),
        // Конструктор класса.
        text(removeOverspaces(unpackingLocalization(localizationKeys) + L" "))
  {
  }

  // Установка позиции/размера (границ) и раскладка дочерних элементов.
  virtual void setBound(float x, float y, float width, float height,
                        float indent)
  {
    // Конструктор класса.
    if (body.width != width)
    {
      body.width = width;
      // Конструктор класса.
      updateDrawableText();
    }

    float deltaY = 0;
    float minWidth = 0;

    // Конструктор класса.
    for (auto &line : drawableText)
    {
      // Конструктор класса.
      line.setPosition(x + width / 2 - line.getGlobalBounds().width / 2,
                       y + deltaY);
      // Конструктор класса.
      deltaY += line.getGlobalBounds().height + line.getLineSpacing();
      // Конструктор класса.
      minWidth = std::max(minWidth, line.getGlobalBounds().width);
    }

    // Конструктор класса.
    Bound::setBound(x, y, minWidth, std::max(deltaY, height), indent);
  }

  // Конструктор класса.
  void resetString()
  {
    // Конструктор класса.
    if (type != TEXT_BY_KEY)
    {
      return;
    }

    // Конструктор класса.
    text = removeOverspaces(unpackingLocalization(localizationKeys) + L" ");
    // Конструктор класса.
    updateDrawableText();
  }

  // Отрисовка объекта на целевой поверхности.
  void draw(sf::RenderTarget &target, sf::RenderStates states) const
  {
    // Конструктор класса.
    for (auto &line : drawableText)
    {
      // Конструктор класса.
      target.draw(line, states);
    }
  }

protected:
  // Конструктор класса.
  static std::wstring removeOverspaces(const std::wstring &string)
  {
    std::wstring stringWithoutSpaces;
    bool isWord = false;

    // Конструктор класса.
    for (auto &c : string)
    {
      // Конструктор класса.
      if (c != L' ' && c != L'\n')
      {
        stringWithoutSpaces += c;
        isWord = true;
        // Конструктор класса.
      }
      else if ((c == L' ' || c == L'\n') && isWord)
      {
        stringWithoutSpaces += L' ';
        isWord = false;
      }
    }

    return stringWithoutSpaces;
  }

  // Конструктор класса.
  static bool doStringPlaced(const std::wstring &string, const float &width)
  {
    sf::Text tmp;
    // Конструктор класса.
    tmp.setFont(Resource::defaultFont);
    // Конструктор класса.
    tmp.setCharacterSize(Resource::characterSize);
    // Конструктор класса.
    tmp.setString(string);

    // Конструктор класса.
    return tmp.getGlobalBounds().width <= width;
  }

  static std::wstring
  // Конструктор класса.
  unpackingLocalization(const std::vector<std::string> &localizationKeys)
  {
    nlohmann::json j = Resource::localization;

    // Конструктор класса.
    for (auto &key : localizationKeys)
    {
      j = j[key];
    }

    // Конструктор класса.
    return utf8_to_wstring(j);
  }

  // Конструктор класса.
  void drawableTextEmplaceBack(const std::wstring &string)
  {
    // Конструктор класса.
    drawableText.emplace_back();
    // Конструктор класса.
    drawableText.rbegin()->setFont(Resource::defaultFont);
    // Конструктор класса.
    drawableText.rbegin()->setFillColor(textColor);
    // Конструктор класса.
    drawableText.rbegin()->setCharacterSize(Resource::characterSize);
    // Конструктор класса.
    drawableText.rbegin()->setLineSpacing(Resource::lineSpacing);
    // Конструктор класса.
    drawableText.rbegin()->setString(string);
  }

  // Конструктор класса.
  void updateDrawableText()
  {
    std::wstring word, string;
    // Конструктор класса.
    drawableText.clear();

    // Конструктор класса.
    for (auto &c : text)
    {
      // Конструктор класса.
      if (c != L' ')
      {
        word += c;
      }
      // Конструктор класса.
      if (c == L' ')
      {
        // Конструктор класса.
        if (doStringPlaced(string + L" " + word, body.width))
        {
          // Конструктор класса.
          string += (string.size() ? L" " : L"") + word;
          // Конструктор класса.
          word.clear();
        }
        else
        {
          // Конструктор класса.
          if (!string.size())
          {
            string = word;
            // Конструктор класса.
            word.clear();
          }

          // Конструктор класса.
          drawableTextEmplaceBack(string);

          string = word;
          // Конструктор класса.
          word.clear();
        }
      }
    }
    // Конструктор класса.
    if (string.size())
    {
      // Конструктор класса.
      drawableTextEmplaceBack(string);
    }
  }
};