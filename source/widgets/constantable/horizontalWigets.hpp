/**
 * @file horizontalWigets.hpp
 * @brief Горизонтальный компоновщик: размещает элементы в ряд, центрируя набор
 * по ширине.
 */
#pragma once
#include "../constantable.hpp"
#include <vector>

/**
 * @brief Шаблонный горизонтальный контейнер для «постоянных» (Constantable)
 * элементов.
 * @tparam T Базовый тип элементов (по умолчанию Constantable).
 */
template <typename T = Constantable>
class CHorizontalWigets : virtual public Constantable {
protected:
  /// Дочерние элементы (владение — у вызывающей стороны).
  std::vector<T *> elements;

public:
  /**
   * @brief Конструктор.
   * @param elements Набор дочерних элементов для размещения по горизонтали.
   */
  CHorizontalWigets(std::vector<T *> elements) : elements(elements) {}

  /**
   * @brief Компоновка по горизонтали с равными отступами и центрированием всего
   * блока.
   *
   * Схема:
   * 1) Вызываем setBound() у всех детей, чтобы они рассчитали желаемые размеры.
   * 2) Суммируем ширины + отступы, берём максимальную высоту.
   * 3) Вторым проходом выставляем детям финальные X с учётом центрирования.
   */
  virtual void setBound(float x, float y, float width, float height,
                        float indent) {
    float sumWidth = 0;
    float maxHeight = 0;

    // 1-й проход: собрать фактические размеры детей
    for (auto &element : elements) {
      element->setBound(x, y, width, height, indent);
      sumWidth += element->getBound().width + indent;
      maxHeight = std::max(maxHeight, element->getBound().height);
    }
    if (sumWidth > 0)
      sumWidth -= indent; // убрать последний лишний отступ

    // 2-й проход: разложить с центрированием
    float deltaWidth = 0;
    for (auto &element : elements) {
      element->setBound(x + deltaWidth + (width - sumWidth) / 2,
                        y + (maxHeight - height) / 2, element->getBound().width,
                        height, indent);
      deltaWidth += element->getBound().width + indent;
    }

    Bound::setBound(x, y, sumWidth, maxHeight, indent);
  }

  /**
   * @brief Отрисовать детей слева направо.
   */
  void draw(sf::RenderTarget &target, sf::RenderStates states) const {
    for (auto &element : elements) {
      target.draw(*element, states);
    }
  }
};
