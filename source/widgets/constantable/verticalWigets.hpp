// CVerticalWigets — вертикальный компоновщик: размещает элементы столбцом с отступами.
// ------------------------------------------------------------

// Заголовочный файл. pragma once — защита от множественного включения.
#pragma once
#include "../constantable.hpp"

template <typename T = Constantable>
class CVerticalWigets : virtual public Constantable
{
protected:
  std::vector<T *> elements;

public:
  // Конструктор класса.
  CVerticalWigets(std::vector<T *> elements) : elements(elements) {}

  // Установка позиции/размера (границ) и раскладка дочерних элементов.
  virtual void setBound(float x, float y, float width, float height,
                        float indent)
  {
    float deltaY = 0;
    float maxWidth = 0;
    // Конструктор класса.
    for (auto &element : elements)
    {
      // Конструктор класса.
      element->setBound(x, y + deltaY, width, height, indent);
      // Конструктор класса.
      deltaY += element->getBound().height + indent;
      // Конструктор класса.
      maxWidth = std::max(maxWidth, element->getBound().width);
    }
    deltaY -= deltaY == 0 ? 0 : indent;

    // Конструктор класса.
    Bound::setBound(x, y, maxWidth, deltaY, indent);
  }

  // Отрисовка объекта на целевой поверхности.
  void draw(sf::RenderTarget &target, sf::RenderStates states) const
  {
    // Конструктор класса.
    for (auto &element : elements)
    {
      // Конструктор класса.
      target.draw(*element, states);
    }
  }
};