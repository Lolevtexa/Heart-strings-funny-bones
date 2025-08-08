// Activatable — базовый интерфейс для активируемых элементов (реагирующих на курсор/клики). Управляет фокусом и цветом.
// ------------------------------------------------------------

// Заголовочный файл. pragma once — защита от множественного включения.
#pragma once
#include "../resource.hpp"
#include "constantable.hpp"
#include <SFML/Window/Event.hpp>

// Класс Activatable — см. описание в заголовке файла.
class Activatable : virtual public Constantable
{
protected:
  bool focused = false;

public:
  // Обработка ввода/событий SFML (мышь/клавиатура/окно).
  virtual void eventProcessing(sf::Event event)
  {
    if (event.type == sf::Event::MouseMoved)
    {
      if (body.contains(event.mouseMove.x, event.mouseMove.y))
      {
        focused = true;
      }
      else
      {
        focused = false;
      }
    }

    if (event.type == sf::Event::MouseLeft)
    {
      focused = false;
    }
  }

  // Обновление состояния/логики перед отрисовкой.
  virtual void update()
  {
    appearance(focused ? Resource::focusedColor : Resource::unfocusedColor);
  }

  friend class Button;
  friend class AHorizontalWigets;
  friend class AVerticalWigets;

protected:
  // Применение темы/цветов к элементам.
  virtual void appearance(sf::Color color) = 0;
};