/**
 * @file horizontalWigets.hpp
 * @brief Активируемая горизонтальная группа виджетов (обёртка над
 * CHorizontalWigets).
 */
#pragma once
#include "../activatable.hpp"
#include "../constantable/horizontalWigets.hpp"

/**
 * @brief Горизонтальный контейнер активируемых элементов.
 *
 * Наследует логику компоновки из CHorizontalWigets и добавляет окраску через
 * Activatable.
 */
class AHorizontalWigets : virtual public Activatable,
                          public CHorizontalWigets<Activatable> {
public:
  /**
   * @brief Конструктор.
   * @param elements Дочерние активируемые элементы (неизменяемое владение
   * здесь).
   */
  AHorizontalWigets(std::vector<Activatable *> elements)
      : CHorizontalWigets(elements) {
    appearance(Resource::unfocusedColor);
  }

protected:
  /**
   * @brief Применить цветовую схему ко всем дочерним элементам.
   */
  void appearance(sf::Color color) {
    for (auto &element : elements) {
      element->appearance(color);
    }
  }
};
