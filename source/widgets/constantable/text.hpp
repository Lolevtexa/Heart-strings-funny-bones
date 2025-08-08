/**
 * @file text.hpp
 * @brief Многострочный текст с переносами по словам под заданную ширину;
 * поддержка локализации.
 *
 * Алгоритм:
 *  - хранит «сырую» строку (wstring) либо путь по ключам локализации;
 *  - при смене ширины пересобирает drawableText, укладывая слова в строки;
 *  - отрисовывает построчно, центрируя каждую строку по ширине блока.
 */
#pragma once
#include "../../resource.hpp"
#include "../constantable.hpp"
#include <SFML/Graphics/Text.hpp>

#define TEXT_BY_KEY 0
#define TEXT_BY_STRING 1

/**
 * @brief Текстовый виджет с переносом по словам.
 */
class CText : virtual public Constantable {
protected:
  int type; ///< TEXT_BY_KEY или TEXT_BY_STRING.

  std::vector<std::string>
      localizationKeys; ///< Путь в JSON (последовательность ключей).
  std::wstring text;    ///< Итоговая строка (с нормализованными пробелами).

  sf::Color textColor = Resource::focusedColor;
  std::vector<sf::Text> drawableText; ///< Построчное представление для вывода.

public:
  /**
   * @brief Конструктор со строкой.
   */
  CText(const std::wstring &string)
      : text(removeOverspaces(string + L" ")), type(TEXT_BY_STRING) {}

  /**
   * @brief Конструктор по ключам локализации.
   *
   * Немедленно разворачивает ключи в строку по текущей Resource::localization.
   */
  CText(const std::vector<std::string> &localizationKeys)
      : localizationKeys(localizationKeys), type(TEXT_BY_KEY),
        text(removeOverspaces(unpackingLocalization(localizationKeys) + L" ")) {
  }

  /**
   * @brief Компоновка и пересборка построчного текста при смене ширины.
   *
   * Каждая строка центрируется по горизонтали внутри [x, x+width].
   */
  virtual void setBound(float x, float y, float width, float height,
                        float indent) {
    // При изменении доступной ширины пересобираем переносы
    if (body.width != width) {
      body.width = width;
      updateDrawableText();
    }

    float deltaY = 0;
    float minWidth = 0;

    // Разложить строки вертикально и вычислить минимальную требуемую ширину
    // блока
    for (auto &line : drawableText) {
      line.setPosition(x + width / 2 - line.getGlobalBounds().width / 2,
                       y + deltaY);
      deltaY += line.getGlobalBounds().height + line.getLineSpacing();
      minWidth = std::max(minWidth, line.getGlobalBounds().width);
    }

    Bound::setBound(x, y, minWidth, std::max(deltaY, height), indent);
  }

  /**
   * @brief Переинициализировать строку из локализации (при смене языка).
   */
  void resetString() {
    if (type != TEXT_BY_KEY)
      return;
    text = removeOverspaces(unpackingLocalization(localizationKeys) + L" ");
    updateDrawableText();
  }

  /**
   * @brief Отрисовать все строки.
   */
  void draw(sf::RenderTarget &target, sf::RenderStates states) const {
    for (auto &line : drawableText) {
      target.draw(line, states);
    }
  }

protected:
  /**
   * @brief Удалить повторные пробелы и нормализовать переносы (свести к
   * одиночным пробелам).
   */
  static std::wstring removeOverspaces(const std::wstring &string) {
    std::wstring stringWithoutSpaces;
    bool isWord = false;

    for (auto &c : string) {
      if (c != L' ' && c != L'\n') {
        stringWithoutSpaces += c;
        isWord = true;
      } else if ((c == L' ' || c == L'\n') && isWord) {
        stringWithoutSpaces += L' ';
        isWord = false;
      }
    }
    return stringWithoutSpaces;
  }

  /**
   * @brief Проверить, помещается ли строка в указанную ширину с текущими
   * параметрами шрифта.
   */
  static bool doStringPlaced(const std::wstring &string, const float &width) {
    sf::Text tmp;
    tmp.setFont(Resource::defaultFont);
    tmp.setCharacterSize(Resource::characterSize);
    tmp.setString(string);
    return tmp.getGlobalBounds().width <= width;
  }

  /**
   * @brief Развернуть ключи локализации в строку.
   */
  static std::wstring
  unpackingLocalization(const std::vector<std::string> &localizationKeys) {
    nlohmann::json j = Resource::localization;
    for (auto &key : localizationKeys) {
      j = j[key];
    }
    return utf8_to_wstring(j);
  }

  /**
   * @brief Добавить строку в drawableText с нужными атрибутами шрифта/цвета.
   */
  void drawableTextEmplaceBack(const std::wstring &string) {
    drawableText.emplace_back();
    drawableText.rbegin()->setFont(Resource::defaultFont);
    drawableText.rbegin()->setFillColor(textColor);
    drawableText.rbegin()->setCharacterSize(Resource::characterSize);
    drawableText.rbegin()->setLineSpacing(Resource::lineSpacing);
    drawableText.rbegin()->setString(string);
  }

  /**
   * @brief Пересобрать вектор строк drawableText, укладывая слова в строки по
   * ширине body.width.
   *
   * Логика:
   *  - накапливаем слово до пробела;
   *  - проверяем, поместится ли текущая строка + слово в ширину;
   *  - если нет — фиксируем текущую строку в drawableText и начинаем новую.
   */
  void updateDrawableText() {
    std::wstring word, string;
    drawableText.clear();

    for (auto &c : text) {
      if (c != L' ') {
        word += c;
      }
      if (c == L' ') {
        if (doStringPlaced(string + L" " + word, body.width)) {
          string += (string.size() ? L" " : L"") + word;
          word.clear();
        } else {
          if (!string.size()) {
            // крайний случай — слово длиннее ширины: помещаем как есть
            string = word;
            word.clear();
          }
          drawableTextEmplaceBack(string);
          string.clear();

          string = word;
          word.clear();
        }
      }
    }
    if (string.size()) {
      drawableTextEmplaceBack(string);
    }
  }
};
