// COutline — прямоугольная рамка с настраиваемой толщиной и прозрачной заливкой.
// ------------------------------------------------------------

// Заголовочный файл. pragma once — защита от множественного включения.
#pragma once
#include "../../resource.hpp"
#include "../constantable.hpp"
#include <SFML/Graphics/RectangleShape.hpp>

// Класс COutline — см. описание в заголовке файла.
class COutline : virtual public Constantable
{
protected:
  sf::RectangleShape outLine;

public:
  // Конструктор класса.
  COutline()
  {
    // Конструктор класса.
    outLine.setOutlineThickness(3);
    // Конструктор класса.
    outLine.setFillColor(Resource::transparentColor);
  }

  // Установка позиции/размера (границ) и раскладка дочерних элементов.
  virtual void setBound(float x, float y, float width, float height,
                        float indent)
  {
    // Конструктор класса.
    outLine.setPosition(x, y);
    // Конструктор класса.
    outLine.setSize(sf::Vector2f(width, height));

    // Конструктор класса.
    Bound::setBound(x, y, width, height, indent);
  }

  // Отрисовка объекта на целевой поверхности.
  void draw(sf::RenderTarget &target, sf::RenderStates states) const
  {
    // Конструктор класса.
    target.draw(outLine, states);
  }
};