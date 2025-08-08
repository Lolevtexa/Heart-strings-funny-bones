// CNumber — отображение числа sf::Text с обновлением значения через колбэк.
// ------------------------------------------------------------

// Заголовочный файл. pragma once — защита от множественного включения.
#pragma once
#include "../../resource.hpp"
#include "../constantable.hpp"
#include <SFML/Graphics/Text.hpp>

// Класс CNumber — см. описание в заголовке файла.
class CNumber : virtual public Constantable {
protected:
  bool valueIsNumber = false;

  int value = -1;
  std::function<int()> getValue;

  sf::Text drawableNumber;

public:
  template <typename Func>
  // Конструктор: инициализация класса CNumber.
  CNumber(Func &getValue) : getValue(getValue) {
    drawableNumber.setFont(Resource::defaultFont);
    drawableNumber.setFillColor(Resource::focusedColor);
    drawableNumber.setCharacterSize(Resource::characterSize);
    drawableNumber.setLineSpacing(Resource::lineSpacing);
  }

  // Установка позиции/размера (границ) и раскладка дочерних элементов.
  virtual void setBound(float x, float y, float width, float height,
                        float indent) {
    if (value != getValue()) {
      value = getValue();
      drawableNumber.setString(std::to_string(value));
      valueIsNumber = false;
    }

    drawableNumber.setPosition(
        x + width / 2 - drawableNumber.getGlobalBounds().width / 2, y);
    float deltaY = drawableNumber.getGlobalBounds().height +
                   drawableNumber.getLineSpacing();
    float minWidth = std::min(width, drawableNumber.getGlobalBounds().width);

    Bound::setBound(x, y, minWidth, std::max(deltaY, height), indent);
  }

  // Отрисовка объекта на целевой поверхности.
  void draw(sf::RenderTarget &target, sf::RenderStates states) const {
    target.draw(drawableNumber, states);
  }
};