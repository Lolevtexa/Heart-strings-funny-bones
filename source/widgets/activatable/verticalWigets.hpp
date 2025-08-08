/**
 * @file verticalWigets.hpp
 * @brief Активируемая вертикальная группа виджетов (обёртка над
 * CVerticalWigets).
 */
#pragma once
#include "../activatable.hpp"
#include "../constantable/verticalWigets.hpp"

/**
 * @brief Вертикальный контейнер активируемых элементов.
 */
class AVerticalWigets : virtual public Activatable,
                        public CVerticalWigets<Activatable> {
public:
  /**
   * @brief Конструктор.
   * @param elements Дочерние активируемые элементы (неизменяемое владение
   * здесь).
   */
  AVerticalWigets(std::vector<Activatable *> elements)
      : CVerticalWigets(elements) {
    appearance(Resource::unfocusedColor);
  }

protected:
  /**
   * @brief Применить цвет ко всем дочерним элементам.
   */
  void appearance(sf::Color color) {
    for (auto &element : elements) {
      element->appearance(color);
    }
  }
};
