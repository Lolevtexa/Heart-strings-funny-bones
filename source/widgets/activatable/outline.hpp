// AOutline — активируемая рамка-обводка (обертка над COutline).
// ------------------------------------------------------------

// Заголовочный файл. pragma once — защита от множественного включения.
#pragma once
#include "../activatable.hpp"
#include "../constantable/outline.hpp"

// Класс AOutline — см. описание в заголовке файла.
class AOutline : virtual public Activatable, public COutline
{
public:
  // Конструктор класса.
  AOutline() : COutline() {}

protected:
  // Применение темы/цветов к элементам.
  void appearance(sf::Color color) { outLine.setOutlineColor(color); }
};