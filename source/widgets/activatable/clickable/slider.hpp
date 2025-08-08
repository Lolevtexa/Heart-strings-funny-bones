// Slider — горизонтальный слайдер со значением [0..100], изменяемым мышью.
// ------------------------------------------------------------

#include "../clickable.hpp"
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

// Класс Slider — см. описание в заголовке файла.
class Slider : public Clickable
{
protected:
  float minValue = 0;
  float maxValue = 100;
  float value;

  bool mouseMoved = false;
  float mouseOffset = 0;

  sf::CircleShape slider;
  sf::RectangleShape sliderLineLeft;
  sf::RectangleShape sliderLineRight;

public:
  // Конструктор класса.
  Slider(int defaultValue = 50) : value(defaultValue)
  {
    // Конструктор класса.
    slider.setOutlineThickness(3);
    // Конструктор класса.
    sliderLineLeft.setOutlineThickness(3);
    // Конструктор класса.
    sliderLineRight.setOutlineThickness(3);

    // Конструктор класса.
    appearance(Resource::unfocusedColor);
  }

  // Обработка ввода/событий SFML (мышь/клавиатура/окно).
  void eventProcessing(sf::Event event)
  {
    // Конструктор класса.
    if (event.type == sf::Event::MouseMoved)
    {
      mouseMoved = true;
      mouseOffset = event.mouseMove.x;
    }

    // Конструктор класса.
    Clickable::eventProcessing(event);
  }

  // Обновление состояния/логики перед отрисовкой.
  void update()
  {
    // Конструктор класса.
    if (started && mouseMoved)
    {
      float x = mouseOffset;
      // Конструктор класса.
      float minX = sliderLineLeft.getPosition().x;
      // Конструктор класса.
      float maxX = sliderLineRight.getPosition().x;
      // Конструктор класса.
      x = std::max(x, minX);
      // Конструктор класса.
      x = std::min(x, maxX);
      float scaleFactor = (x - minX) / (maxX - minX);
      value = minValue + scaleFactor * (maxValue - minValue);
      mouseMoved = false;
    }
  }

  // Установка позиции/размера (границ) и раскладка дочерних элементов.
  void setBound(float x, float y, float width, float height, float indent)
  {
    float radius = height / 2.f - indent;
    float scaleFactor = (value - minValue) / (maxValue - minValue);
    // Конструктор класса.
    slider.setRadius(radius);
    // Конструктор класса.
    slider.setOrigin(radius, radius);
    // Конструктор класса.
    slider.setPosition(x + indent + radius +
                           (width - 2 * indent - 2 * radius) * scaleFactor,
                       y + height / 2.f);

    // Конструктор класса.
    sliderLineLeft.setSize(sf::Vector2f(
        // Конструктор класса.
        std::max((width - 2 * indent - 2 * radius) * scaleFactor - radius, 0.f),
        0));
    // Конструктор класса.
    sliderLineLeft.setPosition(x + indent + radius, y + height / 2.f);

    // Конструктор класса.
    sliderLineRight.setSize(sf::Vector2f(
        // Конструктор класса.
        std::min((width - 2 * indent - 2 * radius) * (scaleFactor - 1.f) +
                     radius,
                 0.f),
        0));
    // Конструктор класса.
    sliderLineRight.setPosition(x + width - indent - radius, y + height / 2.f);

    // Конструктор класса.
    Clickable::setBound(x, y, width, height, indent);
  }

  // Конструктор класса.
  int getValue() { return value; }

  // Отрисовка объекта на целевой поверхности.
  void draw(sf::RenderTarget &target, sf::RenderStates states) const
  {
    // Конструктор класса.
    target.draw(sliderLineLeft, states);
    // Конструктор класса.
    target.draw(sliderLineRight, states);
    // Конструктор класса.
    target.draw(slider, states);
  }

protected:
  // Применение темы/цветов к элементам.
  void appearance(sf::Color color)
  {
    // Конструктор класса.
    slider.setOutlineColor(color);
    // Конструктор класса.
    sliderLineLeft.setOutlineColor(color);
    // Конструктор класса.
    sliderLineRight.setOutlineColor(Resource::unfocusedColor);
  }
};