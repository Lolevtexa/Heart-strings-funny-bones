/**
 * @file activatable.hpp
 * @brief Базовый интерфейс «активируемых» элементов: реагирует на курсор/клики,
 * управляет фокусом/цветом.
 */
#pragma once
#include "../resource.hpp"
#include "constantable.hpp"
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>

/**
 * @brief Базовый класс для элементов, меняющих внешний вид при
 * наведении/взаимодействии.
 *
 * Контракт:
 *  - eventProcessing(): обновляет focused по движениям мыши;
 *  - update(): вызывает appearance() с нужным цветом в зависимости от focused.
 */
class Activatable : virtual public Constantable {
protected:
  /// Находится ли курсор в пределах body (обновляется в eventProcessing()).
  bool focused = false;

public:
  /**
   * @brief Обработка событий SFML для базового фокуса.
   */
  virtual void eventProcessing(sf::Event event) {
    if (auto * m = event.getIf<sf::Event::MouseMoved>()) {
      focused = body.contains(sf::Vector2f(m->position.x, m->position.y));
    }
    if (auto * l = event.getIf<sf::Event::MouseLeft>()) {
      focused = false;
    }
  }

  /**
   * @brief Обновление визуального состояния (цветов) на основе focused.
   */
  virtual void update() {
    appearance(focused ? Resource::focusedColor : Resource::unfocusedColor);
  }

  friend class Button;
  friend class AHorizontalWigets;
  friend class AVerticalWigets;

protected:
  /**
   * @brief Применить тему/цвета к элементу.
   * @param color Цвет текста/обводки и т.п.
   */
  virtual void appearance(sf::Color color) = 0;
};
