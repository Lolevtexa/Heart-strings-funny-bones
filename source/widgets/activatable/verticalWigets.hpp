// AVerticalWigets — активируемая вертикальная группа виджетов (оборачивает CVerticalWigets).
// ------------------------------------------------------------

// Заголовочный файл. pragma once — защита от множественного включения.
#pragma once
#include "../activatable.hpp"
#include "../constantable/verticalWigets.hpp"

// Класс AVerticalWigets — см. описание в заголовке файла.
class AVerticalWigets : virtual public Activatable,
                        public CVerticalWigets<Activatable>
{
public:
  // Конструктор класса.
  AVerticalWigets(std::vector<Activatable *> elements)
      // Конструктор класса.
      : CVerticalWigets(elements)
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