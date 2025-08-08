// Clickable — добавляет логику клика (нажатие/отпускание ЛКМ) поверх Activatable.
// ------------------------------------------------------------

// Заголовочный файл. pragma once — защита от множественного включения.
#pragma once
#include "../activatable.hpp"

// Класс Clickable — см. описание в заголовке файла.
class Clickable : virtual public Activatable
{
protected:
  bool started = false;
  bool activate = false;

public:
  // Обработка ввода/событий SFML (мышь/клавиатура/окно).
  virtual void eventProcessing(sf::Event event)
  {
    if (event.type == sf::Event::MouseButtonPressed)
    {
      if (event.mouseButton.button == sf::Mouse::Left)
      {
        started = focused;
      }
    }

    if (event.type == sf::Event::MouseButtonReleased)
    {
      if (event.mouseButton.button == sf::Mouse::Left)
      {
        activate =
            body.contains(event.mouseButton.x, event.mouseButton.y) && started;
        started = false;
      }
    }

    Activatable::eventProcessing(event);

    if (event.type == sf::Event::MouseMoved)
    {
      if (!body.contains(event.mouseMove.x, event.mouseMove.y) && started)
      {
        focused = true;
      }
    }
  }
};