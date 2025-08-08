// Image — отрисовка текстуры в прямоугольнике.
// ------------------------------------------------------------

// Заголовочный файл. pragma once — защита от множественного включения.
#pragma once
#include "../constantable.hpp"
#include <SFML/Graphics/RectangleShape.hpp>

// Класс Image — см. описание в заголовке файла.
class Image : virtual public Constantable
{
protected:
  sf::RectangleShape image;

public:
  // Конструктор класса.
  Image(sf::Texture *texture) { image.setTexture(texture); }

  // Установка позиции/размера (границ) и раскладка дочерних элементов.
  virtual void setBound(float x, float y, float width, float height,
                        float indent)
  {
    // Конструктор класса.
    image.setPosition(x, y);
    // Конструктор класса.
    image.setSize(sf::Vector2f(width, height));

    // Конструктор класса.
    Bound::setBound(x, y, width, height, indent);
  }

  // Отрисовка объекта на целевой поверхности.
  void draw(sf::RenderTarget &target, sf::RenderStates states) const
  {
    // Конструктор класса.
    target.draw(image, states);
  }
};