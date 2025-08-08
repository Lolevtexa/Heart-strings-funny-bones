// CVerticalWigets — вертикальный компоновщик: размещает элементы столбцом с
// отступами.
// ------------------------------------------------------------

// Заголовочный файл. pragma once — защита от множественного включения.
#pragma once
#include "../constantable.hpp"

template <typename T = Constantable>
class CVerticalWigets : virtual public Constantable {
protected:
  std::vector<T *> elements;

public:
  // Конструктор: инициализация класса CVerticalWigets.
  CVerticalWigets(std::vector<T *> elements) : elements(elements) {}

  // Установка позиции/размера (границ) и раскладка дочерних элементов.
  virtual void setBound(float x, float y, float width, float height,
                        float indent) {
    float deltaY = 0;
    float maxWidth = 0;
    for (auto &element : elements) {
      element->setBound(x, y + deltaY, width, height, indent);
      deltaY += element->getBound().height + indent;
      maxWidth = std::max(maxWidth, element->getBound().width);
    }
    deltaY -= deltaY == 0 ? 0 : indent;

    Bound::setBound(x, y, maxWidth, deltaY, indent);
  }

  // Отрисовка объекта на целевой поверхности.
  void draw(sf::RenderTarget &target, sf::RenderStates states) const {
    for (auto &element : elements) {
      target.draw(*element, states);
    }
  }
};