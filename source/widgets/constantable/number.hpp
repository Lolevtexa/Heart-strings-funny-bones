// CNumber — отображение числа sf::Text с обновлением значения через колбэк.
// ------------------------------------------------------------

// Заголовочный файл. pragma once — защита от множественного включения.
#pragma once
#include "../../resource.hpp"
#include "../constantable.hpp"
#include <SFML/Graphics/Text.hpp>

// Класс CNumber — см. описание в заголовке файла.
class CNumber : virtual public Constantable
{
protected:
  bool valueIsNumber = false;

  int value = -1;
  // Конструктор класса.
  std::function<int()> getValue;

  sf::Text drawableNumber;

public:
  // Конструктор класса.
  template <typename Func>
  CNumber(Func &getValue) : getValue(getValue)
  {
    // Конструктор класса.
    drawableNumber.setFont(Resource::defaultFont);
    // Конструктор класса.
    drawableNumber.setFillColor(Resource::focusedColor);
    // Конструктор класса.
    drawableNumber.setCharacterSize(Resource::characterSize);
    // Конструктор класса.
    drawableNumber.setLineSpacing(Resource::lineSpacing);
  }

  // Установка позиции/размера (границ) и раскладка дочерних элементов.
  virtual void setBound(float x, float y, float width, float height,
                        float indent)
  {
    // Конструктор класса.
    if (value != getValue())
    {
      // Конструктор класса.
      value = getValue();
      // Конструктор класса.
      drawableNumber.setString(std::to_string(value));
      valueIsNumber = false;
    }

    // Конструктор класса.
    drawableNumber.setPosition(
        // Конструктор класса.
        x + width / 2 - drawableNumber.getGlobalBounds().width / 2, y);
    // Конструктор класса.
    float deltaY = drawableNumber.getGlobalBounds().height +
                   // Конструктор класса.
                   drawableNumber.getLineSpacing();
    // Конструктор класса.
    float minWidth = std::min(width, drawableNumber.getGlobalBounds().width);

    // Конструктор класса.
    Bound::setBound(x, y, minWidth, std::max(deltaY, height), indent);
  }

  // Отрисовка объекта на целевой поверхности.
  void draw(sf::RenderTarget &target, sf::RenderStates states) const
  {
    // Конструктор класса.
    target.draw(drawableNumber, states);
  }
};