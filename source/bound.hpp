/**
 * @file bound.hpp
 * @brief Базовый класс для хранения прямоугольной области виджета (границ).
 *
 * Содержит общий sf::FloatRect и метод setBound(), который задаёт положение и
 * размер. Наследники используют body для расчётов фокуса/отрисовки/компоновки.
 */
#pragma once
#include <SFML/Graphics/Rect.hpp>

/**
 * @brief Базовые границы виджета.
 */
class Bound {
protected:
  /// Прямоугольная область виджета в оконных координатах.
  sf::FloatRect body;

public:
  /**
   * @brief Установить границы элемента.
   * @param x      Левая координата.
   * @param y      Верхняя координата.
   * @param width  Ширина.
   * @param height Высота.
   * @param indent Внутренний отступ (для совместимости с интерфейсом, здесь не
   * используется).
   *
   * Замечание: метод виртуальный, чтобы наследники могли дополнять компоновкой
   * дочерних элементов.
   */
  virtual void setBound(float x, float y, float width, float height,
                        float indent) {
    (void)indent; // в базовой реализации не нужен
    body.height = height;
    body.width = width;
    body.top = y;
    body.left = x;
  }

  /**
   * @brief Получить текущие границы.
   */
  sf::FloatRect getBound() { return body; }
};
