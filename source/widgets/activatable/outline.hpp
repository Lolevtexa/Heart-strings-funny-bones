/**
 * @file outline.hpp
 * @brief Активируемая рамка-обводка (обёртка над COutline).
 */
#pragma once
#include "../activatable.hpp"
#include "../constantable/outline.hpp"

/**
 * @brief Обводка, реагирующая на фокус (меняет цвет контура).
 */
class AOutline : virtual public Activatable, public COutline {
public:
  /// Конструктор по умолчанию.
  AOutline() : COutline() {}

protected:
  /**
   * @brief Применить цвет к контуру.
   */
  void appearance(sf::Color color) { outLine.setOutlineColor(color); }
};
