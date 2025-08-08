// CHorizontalWigets — горизонтальный компоновщик: размещает элементы в ряд, центрируя по ширине.
// ------------------------------------------------------------

// Заголовочный файл. pragma once — защита от множественного включения.
#pragma once
#include "../constantable.hpp"

template <typename T = Constantable>
class CHorizontalWigets : virtual public Constantable
{
protected:
  std::vector<T *> elements;

public:
  // Конструктор: инициализация класса CHorizontalWigets.
  CHorizontalWigets(std::vector<T *> elements) : elements(elements) {}

  // Установка позиции/размера (границ) и раскладка дочерних элементов.
  virtual void setBound(float x, float y, float width, float height,
                        float indent)
  {
    float sumWidth = 0;
    float maxHeight = 0;
    for (auto &element : elements)
    {
      element->setBound(x, y, width, height, indent);
      sumWidth += element->getBound().width + indent;
      maxHeight = std::max(maxHeight, element->getBound().height);
    }
    sumWidth -= sumWidth == 0 ? 0 : indent;

    float deltaWidth = 0;
    for (auto &element : elements)
    {
      element->setBound(x + deltaWidth + (width - sumWidth) / 2,
                        y + (maxHeight - height) / 2, element->getBound().width,
                        height, indent);
      deltaWidth += element->getBound().width + indent;
    }

    Bound::setBound(x, y, sumWidth, maxHeight, indent);
  }

  // Отрисовка объекта на целевой поверхности.
  void draw(sf::RenderTarget &target, sf::RenderStates states) const
  {
    for (auto &element : elements)
    {
      target.draw(*element, states);
    }
  }
};