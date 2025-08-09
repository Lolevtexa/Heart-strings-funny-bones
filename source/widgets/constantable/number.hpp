/**
 * @file number.hpp
 * @brief Отображение числа с автообновлением через колбэк.
 */
#pragma once
#include "../../resource.hpp"
#include "../constantable.hpp"
#include <SFML/Graphics/Text.hpp>

/**
 * @brief Числовой виджет: хранит колбэк для получения значения и отрисовывает
 * sf::Text.
 */
class CNumber : virtual public Constantable {
protected:
  bool valueIsNumber = false; ///< Зарезервировано (на будущее форматирование).

  int value = -1;                ///< Последнее отображённое значение.
  std::function<int()> getValue; ///< Функтор получения актуального значения.

  sf::Text drawableNumber; ///< Текстовое представление числа.

public:
  /**
   * @brief Конструктор.
   * @tparam Func Функтор/лямбда, возвращающая int по запросу.
   * @param getValue Ссылка на функтор.
   */
  template <typename Func> CNumber(Func &getValue) : getValue(getValue), drawableNumber(Resource::defaultFont, L"", Resource::characterSize) {
    drawableNumber.setFillColor(Resource::focusedColor);
    drawableNumber.setLineSpacing(Resource::lineSpacing);
  }

  /**
   * @brief Компоновка и обновление текста при изменении значения.
   *
   * Центрирует надпись по горизонтали в пределах [x, x+width].
   */
  virtual void setBound(float x, float y, float width, float height,
                        float indent) {
    // Обновить строку, если значение поменялось
    if (value != getValue()) {
      value = getValue();
      drawableNumber.setString(std::to_string(value));
      valueIsNumber = false;
    }

    // Центрирование по ширине
    drawableNumber.setPosition({
        x + width / 2 - drawableNumber.getGlobalBounds().size.x / 2, y
    });

    const float deltaY = drawableNumber.getGlobalBounds().size.y +
                         drawableNumber.getLineSpacing();
    const float minWidth =
        std::min(width, drawableNumber.getGlobalBounds().size.x);

    Bound::setBound(x, y, minWidth, std::max(deltaY, height), indent);
  }

  /**
   * @brief Отрисовать числовую строку.
   */
  void draw(sf::RenderTarget &target, sf::RenderStates states) const {
    target.draw(drawableNumber, states);
  }
};
