// COutline — прямоугольная рамка с настраиваемой толщиной и прозрачной
// заливкой.
// ------------------------------------------------------------

// Заголовочный файл. pragma once — защита от множественного включения.
#pragma once
#include "../../resource.hpp"
#include "../constantable.hpp"
#include <SFML/Graphics/RectangleShape.hpp>

// Класс COutline — см. описание в заголовке файла.
class COutline : virtual public Constantable {
protected:
  sf::RectangleShape outLine;

public:
  // Конструктор: инициализация класса COutline.
  COutline() {
    outLine.setOutlineThickness(3);
    outLine.setFillColor(Resource::transparentColor);
  }

  // Установка позиции/размера (границ) и раскладка дочерних элементов.
  virtual void setBound(float x, float y, float width, float height,
                        float indent) {
    outLine.setPosition(x, y);
    outLine.setSize(sf::Vector2f(width, height));

    Bound::setBound(x, y, width, height, indent);
  }

  // Отрисовка объекта на целевой поверхности.
  void draw(sf::RenderTarget &target, sf::RenderStates states) const {
    target.draw(outLine, states);
  }
};