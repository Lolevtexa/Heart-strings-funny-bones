// AVerticalWigets — активируемая вертикальная группа виджетов (оборачивает
// CVerticalWigets).
// ------------------------------------------------------------

// Заголовочный файл. pragma once — защита от множественного включения.
#pragma once
#include "../activatable.hpp"
#include "../constantable/verticalWigets.hpp"

// Класс AVerticalWigets — см. описание в заголовке файла.
class AVerticalWigets : virtual public Activatable,
                        public CVerticalWigets<Activatable> {
public:
  // Конструктор: инициализация класса AVerticalWigets.
  AVerticalWigets(std::vector<Activatable *> elements)
      : CVerticalWigets(elements) {
    appearance(Resource::unfocusedColor);
  }

protected:
  // Применение темы/цветов к элементам.
  void appearance(sf::Color color) {
    for (auto &element : elements) {
      element->appearance(color);
    }
  }
};