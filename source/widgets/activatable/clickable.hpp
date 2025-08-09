/**
 * @file clickable.hpp
 * @brief Декоратор «кликабельности»: отслеживает нажатие/отпускание ЛКМ и
 * выставляет activate.
 */
#pragma once
#include "../activatable.hpp"
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Mouse.hpp>

/**
 * @brief Добавляет логику клика (press/release) поверх Activatable.
 *
 * Паттерн: по press фиксируем started, по release проверяем, что указатель ещё
 * в пределах body.
 */
class Clickable : virtual public Activatable {
protected:
  bool started = false;  ///< ЛКМ нажата в фокусе.
  bool activate = false; ///< Флаг «сработать действию» в update() наследника.

public:
  /**
   * @brief Обработка нажатий/отпусканий ЛКМ и перемещений мыши.
   *
   * Детали:
   *  - ЛКМ press: помечаем started=true, только если курсор был над элементом
   * (focused=true).
   *  - ЛКМ release: activate=true, если курсор всё ещё над элементом, и мы
   * стартовали внутри.
   *  - MouseMoved вне body при зажатой ЛКМ поддерживает «клик с уходом»
   * (оставляем focused=true).
   */
  virtual void eventProcessing(sf::Event event) {
    if (auto * m = event.getIf<sf::Event::MouseButtonPressed>()) {
      if (m->button == sf::Mouse::Button::Left) {
        started = focused;
      }
    }

    if (auto * r = event.getIf<sf::Event::MouseButtonReleased>()) {
      if (r->button == sf::Mouse::Button::Left) {
        activate =
            body.contains(sf::Vector2f(r->position.x, r->position.y)) && started;
        started = false;
      }
    }

    Activatable::eventProcessing(event);

    if (auto * m = event.getIf<sf::Event::MouseMoved>()) {
      // Если ушли за пределы с зажатой ЛКМ — считаем, что всё ещё
      // взаимодействуем с элементом.
      if (!body.contains(sf::Vector2f(m->position.x, m->position.y)) && started) {
        focused = true;
      }
    }
  }
};
