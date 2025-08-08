/**
 * @file text.hpp
 * @brief Активируемый текст: локализованный либо произвольная строка (обёртка
 * над CText).
 */
#pragma once
#include "../activatable.hpp"
#include "../constantable/text.hpp"

/**
 * @brief Текстовый виджет, меняющий цвет при наведении/фокусе.
 */
class AText : public Activatable, public CText {
public:
  /**
   * @brief Конструктор со строкой.
   * @param string Текст (wstring).
   */
  AText(const std::wstring &string) : CText(string) {
    appearance(Resource::unfocusedColor);
  }

  /**
   * @brief Конструктор по ключам локализации.
   * @param localizationKeys Путь по JSON (последовательность ключей).
   */
  AText(const std::vector<std::string> &localizationKeys)
      : CText(localizationKeys), Activatable() {
    appearance(Resource::unfocusedColor);
  }

protected:
  /**
   * @brief Применить цвет ко всем строкам отрисовываемого текста.
   */
  void appearance(sf::Color color) {
    textColor = color;
    for (auto &line : drawableText) {
      line.setFillColor(textColor);
    }
  }
};
