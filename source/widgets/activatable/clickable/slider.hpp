/**
 * @file slider.hpp
 * @brief Горизонтальный слайдер значения [min..max] (по умолчанию 0..100).
 *
 * Реагирует на перетаскивание ЛКМ: позиция курсора проецируется на интервал
 * значения.
 */
#include "../clickable.hpp"
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

/**
 * @brief Слайдер значения. Визуально: круглый ползунок и две линии
 * слева/справа.
 */
class Slider : public Clickable {
protected:
  float minValue = 0;
  float maxValue = 100;
  float value;

  bool mouseMoved = false;
  float mouseOffset = 0; ///< Последняя X-координата курсора (при MouseMoved).

  sf::CircleShape slider;
  sf::RectangleShape sliderLineLeft;
  sf::RectangleShape sliderLineRight;

public:
  /// @param defaultValue Стартовое значение (0..100).
  Slider(int defaultValue = 50) : value(defaultValue) {
    slider.setOutlineThickness(3);
    sliderLineLeft.setOutlineThickness(3);
    sliderLineRight.setOutlineThickness(3);
    appearance(Resource::unfocusedColor);
  }

  /// Фиксируем перемещение курсора — дорабатываем в update() (когда ЛКМ
  /// зажата).
  void eventProcessing(sf::Event event) {
    if (event.type == sf::Event::MouseMoved) {
      mouseMoved = true;
      mouseOffset = event.mouseMove.x;
    }
    Clickable::eventProcessing(event);
  }

  /// Пересчёт value на основании положения курсора между краями линий.
  void update() {
    if (started && mouseMoved) {
      float x = mouseOffset;
      float minX = sliderLineLeft.getPosition().x;
      float maxX = sliderLineRight.getPosition().x;
      x = std::max(x, minX);
      x = std::min(x, maxX);

      const float scaleFactor = (x - minX) / (maxX - minX);
      value = minValue + scaleFactor * (maxValue - minValue);
      mouseMoved = false;
    }
  }

  /**
   * @brief Компоновка: рассчитываем радиус ползунка и позиции линий
   * относительно значения.
   *
   * Важно: линии рисуются нулевой высоты с толстой обводкой — выглядит как
   * линия.
   */
  void setBound(float x, float y, float width, float height, float indent) {
    const float radius = height / 2.f - indent;
    const float scaleFactor = (value - minValue) / (maxValue - minValue);

    slider.setRadius(radius);
    slider.setOrigin(radius, radius);
    slider.setPosition(x + indent + radius +
                           (width - 2 * indent - 2 * radius) * scaleFactor,
                       y + height / 2.f);

    // Левая «линия-отрезок»: от левого края до ползунка
    sliderLineLeft.setSize(sf::Vector2f(
        std::max((width - 2 * indent - 2 * radius) * scaleFactor - radius, 0.f),
        0));
    sliderLineLeft.setPosition(x + indent + radius, y + height / 2.f);

    // Правая «линия-отрезок»: от правого края до ползунка (отрицательная ширина
    // интерпретируется как 0)
    sliderLineRight.setSize(sf::Vector2f(
        std::min((width - 2 * indent - 2 * radius) * (scaleFactor - 1.f) +
                     radius,
                 0.f),
        0));
    sliderLineRight.setPosition(x + width - indent - radius, y + height / 2.f);

    Clickable::setBound(x, y, width, height, indent);
  }

  /// @return Текущее значение (округление на вызывающей стороне при
  /// необходимости).
  int getValue() { return value; }

  /// Отрисовка двух линий и ползунка.
  void draw(sf::RenderTarget &target, sf::RenderStates states) const {
    target.draw(sliderLineLeft, states);
    target.draw(sliderLineRight, states);
    target.draw(slider, states);
  }

protected:
  /// Применение цветовой схемы: ползунок и левая линия — заданный цвет; правая
  /// — «неактивная».
  void appearance(sf::Color color) {
    slider.setOutlineColor(color);
    sliderLineLeft.setOutlineColor(color);
    sliderLineRight.setOutlineColor(Resource::unfocusedColor);
  }
};
