/**
 * @file image.hpp
 * @brief Простая картинка-текстура, растягиваемая в заданные границы.
 */
#pragma once
#include "../constantable.hpp"
#include <SFML/Graphics/RectangleShape.hpp>

/**
 * @brief Отрисовка текстуры в прямоугольнике.
 */
class Image : virtual public Constantable {
protected:
  sf::RectangleShape image;

public:
  /**
   * @brief Конструктор.
   * @param texture Указатель на текстуру (владение не передаётся).
   */
  Image(sf::Texture *texture) { image.setTexture(texture); }

  /**
   * @brief Установить границы и растянуть текстуру под них.
   */
  virtual void setBound(float x, float y, float width, float height,
                        float indent) {
    image.setPosition(x, y);
    image.setSize(sf::Vector2f(width, height));
    Bound::setBound(x, y, width, height, indent);
  }

  /**
   * @brief Отрисовать текстуру.
   */
  void draw(sf::RenderTarget &target, sf::RenderStates states) const {
    target.draw(image, states);
  }
};
