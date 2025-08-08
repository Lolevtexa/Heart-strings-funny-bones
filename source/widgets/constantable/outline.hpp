/**
 * @file outline.hpp
 * @brief Прямоугольная рамка (толстая обводка, прозрачная заливка).
 */
#pragma once
#include "../../resource.hpp"
#include "../constantable.hpp"
#include <SFML/Graphics/RectangleShape.hpp>

/**
 * @brief Непрозрачная рамка с настраиваемой толщиной контура.
 */
class COutline : virtual public Constantable {
protected:
  sf::RectangleShape outLine;

public:
  /// Конструктор: толщина 3, заливка — прозрачная.
  COutline() {
    outLine.setOutlineThickness(3);
    outLine.setFillColor(Resource::transparentColor);
  }

  /**
   * @brief Установить прямоугольник рамки.
   */
  virtual void setBound(float x, float y, float width, float height,
                        float indent) {
    outLine.setPosition(x, y);
    outLine.setSize(sf::Vector2f(width, height));
    Bound::setBound(x, y, width, height, indent);
  }

  /**
   * @brief Отрисовать рамку.
   */
  void draw(sf::RenderTarget &target, sf::RenderStates states) const {
    target.draw(outLine, states);
  }
};
