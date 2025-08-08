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
  // Конструктор: инициализация класса AText.
  AText(const std::wstring &string) : CText(string)
  {
    appearance(Resource::unfocusedColor);
  }
  // Конструктор: инициализация класса AText.
  AText(const std::vector<std::string> &localizationKeys)
      : CText(localizationKeys), Activatable()
  {
    appearance(Resource::unfocusedColor);
  }

protected:
  // Применение темы/цветов к элементам.
  void appearance(sf::Color color)
  {
    textColor = color;
    for (auto &line : drawableText)
    {
      line.setFillColor(textColor);
    }
  }
};