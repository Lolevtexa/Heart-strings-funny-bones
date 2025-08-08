// AText — активируемый текст: локализованный или произвольная строка (обертка над CText).
// ------------------------------------------------------------

// Заголовочный файл. pragma once — защита от множественного включения.
#pragma once
#include "../activatable.hpp"
#include "../constantable/text.hpp"

// Класс AText — см. описание в заголовке файла.
class AText : public Activatable, public CText
{
public:
  // Конструктор класса.
  AText(const std::wstring &string) : CText(string)
  {
    // Конструктор класса.
    appearance(Resource::unfocusedColor);
  }
  // Конструктор класса.
  AText(const std::vector<std::string> &localizationKeys)
      // Конструктор класса.
      : CText(localizationKeys), Activatable()
  {
    // Конструктор класса.
    appearance(Resource::unfocusedColor);
  }

protected:
  // Применение темы/цветов к элементам.
  void appearance(sf::Color color)
  {
    textColor = color;
    // Конструктор класса.
    for (auto &line : drawableText)
    {
      // Конструктор класса.
      line.setFillColor(textColor);
    }
  }
};