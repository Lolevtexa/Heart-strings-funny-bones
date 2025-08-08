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
    // Конструктор класса.
    if (event.type == sf::Event::MouseButtonPressed)
    {
      // Конструктор класса.
      if (event.mouseButton.button == sf::Mouse::Left)
      {
        started = focused;
      }
    }

    // Конструктор класса.
    if (event.type == sf::Event::MouseButtonReleased)
    {
      // Конструктор класса.
      if (event.mouseButton.button == sf::Mouse::Left)
      {
        activate =
            // Конструктор класса.
            body.contains(event.mouseButton.x, event.mouseButton.y) && started;
        started = false;
      }
    }

    // Конструктор класса.
    Activatable::eventProcessing(event);

    // Конструктор класса.
    if (event.type == sf::Event::MouseMoved)
    {
      // Конструктор класса.
      if (!body.contains(event.mouseMove.x, event.mouseMove.y) && started)
      {
        focused = true;
      }
    }
  }
};