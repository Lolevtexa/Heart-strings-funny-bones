// ANumber — активируемый числовой текст (обертка над CNumber).
// ------------------------------------------------------------

// Заголовочный файл. pragma once — защита от множественного включения.
#pragma once
#include "../activatable.hpp"
#include "../constantable/number.hpp"

// Класс ANumber — см. описание в заголовке файла.
class ANumber : public Activatable, public CNumber
{
public:
  // Конструктор класса.
  template <typename Func>
  ANumber(Func &getValue) : CNumber(getValue)
  {
    // Конструктор класса.
    appearance(Resource::unfocusedColor);
  }

protected:
  // Применение темы/цветов к элементам.
  void appearance(sf::Color color) { drawableNumber.setFillColor(color); }
};