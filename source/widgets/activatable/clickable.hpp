/**
 * @file clickable.hpp
 * @brief Декоратор «кликабельности»: отслеживает нажатие/отпускание ЛКМ и
 * выставляет activate.
 */
#pragma once
#include "../activatable.hpp"

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
    if (event.type == sf::Event::MouseButtonPressed) {
      if (event.mouseButton.button == sf::Mouse::Left) {
        started = focused;
      }
    }

    if (event.type == sf::Event::MouseButtonReleased) {
      if (event.mouseButton.button == sf::Mouse::Left) {
        activate =
            body.contains(event.mouseButton.x, event.mouseButton.y) && started;
        started = false;
      }
    }

    Activatable::eventProcessing(event);

    if (event.type == sf::Event::MouseMoved) {
      // Если ушли за пределы с зажатой ЛКМ — считаем, что всё ещё
      // взаимодействуем с элементом.
      if (!body.contains(event.mouseMove.x, event.mouseMove.y) && started) {
        focused = true;
      }
    }
  }
};
