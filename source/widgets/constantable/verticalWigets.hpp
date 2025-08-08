/**
 * @file verticalWigets.hpp
 * @brief Вертикальный компоновщик: размещает элементы столбцом с отступами.
 */
#pragma once
#include "../constantable.hpp"
#include <vector>

/**
 * @brief Шаблонный вертикальный контейнер для «постоянных» (Constantable)
 * элементов.
 * @tparam T Базовый тип элементов (по умолчанию Constantable).
 */
template <typename T = Constantable>
class CVerticalWigets : virtual public Constantable {
protected:
  /// Дочерние элементы (владение — у вызывающей стороны).
  std::vector<T *> elements;

public:
  /**
   * @brief Конструктор.
   * @param elements Список дочерних элементов.
   */
  CVerticalWigets(std::vector<T *> elements) : elements(elements) {}

  /**
   * @brief Компоновка по вертикали: каждый элемент под предыдущим, через
   * indent.
   */
  virtual void setBound(float x, float y, float width, float height,
                        float indent) {
    float deltaY = 0;
    float maxWidth = 0;

    for (auto &element : elements) {
      element->setBound(x, y + deltaY, width, height, indent);
      deltaY += element->getBound().height + indent;
      maxWidth = std::max(maxWidth, element->getBound().width);
    }
    if (deltaY > 0)
      deltaY -= indent; // убрать последний лишний отступ

    Bound::setBound(x, y, maxWidth, deltaY, indent);
  }

  /**
   * @brief Отрисовать элементы сверху вниз.
   */
  void draw(sf::RenderTarget &target, sf::RenderStates states) const {
    for (auto &element : elements) {
      target.draw(*element, states);
    }
  }
};
