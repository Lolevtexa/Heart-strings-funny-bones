// AHorizontalWigets — активируемая горизонтальная группа виджетов (оборачивает CHorizontalWigets).
// ------------------------------------------------------------

// Заголовочный файл. pragma once — защита от множественного включения.
#pragma once
#include "../activatable.hpp"
#include "../constantable/horizontalWigets.hpp"

// Класс AHorizontalWigets — см. описание в заголовке файла.
class AHorizontalWigets : virtual public Activatable,
                          public CHorizontalWigets<Activatable>
{
public:
  // Конструктор класса.
  AHorizontalWigets(std::vector<Activatable *> elements)
      // Конструктор класса.
      : CHorizontalWigets(elements)
  {
    // Конструктор класса.
    appearance(Resource::unfocusedColor);
  }

protected:
  // Применение темы/цветов к элементам.
  void appearance(sf::Color color)
  {
    // Конструктор класса.
    for (auto &element : elements)
    {
      // Конструктор класса.
      element->appearance(color);
    }
  }
};