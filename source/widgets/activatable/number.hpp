/**
 * @file number.hpp
 * @brief Активируемая версия числового текста (обёртка над CNumber).
 */
#pragma once
#include "../activatable.hpp"
#include "../constantable/number.hpp"

/**
 * @brief Активируемый числовой виджет: цвет меняется при фокусе.
 */
class ANumber : public Activatable, public CNumber {
public:
  /**
   * @brief Конструктор.
   * @tparam Func Функтор/лямбда, возвращающая актуальное значение int.
   * @param getValue Ссылка на функтор для получения значения.
   */
  template <typename Func> ANumber(Func &getValue) : CNumber(getValue) {
    appearance(Resource::unfocusedColor);
  }

protected:
  /**
   * @brief Установить цвет числа.
   */
  void appearance(sf::Color color) { drawableNumber.setFillColor(color); }
};
