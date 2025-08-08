// SubButtons — вспомогательный класс для управления набором внутренних кнопок и выбранным индексом.
// ------------------------------------------------------------

// Заголовочный файл. pragma once — защита от множественного включения.
#pragma once
#include "button.hpp"

// Класс SubButtons — см. описание в заголовке файла.
class SubButtons : virtual public Clickable
{
protected:
  bool selected = false;
  bool unselect = true;

  int buttonNumber = 0;
  std::vector<Button *> subButtons;

public:
  // Конструктор класса.
  SubButtons(std::vector<Button *> subButtons) : subButtons(subButtons)
  {
    // Конструктор класса.
    for (int i = 0; i < this->subButtons.size(); i++)
    {
      // Конструктор класса.
      std::function<void()> action = this->subButtons[i]->action;
      this->subButtons[i]->action = [this, i, action]()
      {
        buttonNumber = i;
        // Конструктор класса.
        action();
      };
      // Конструктор класса.
      this->subButtons[i]->appearance(Resource::unfocusedColor);
    }
  }

  // Конструктор класса.
  ~SubButtons()
  {
    // Конструктор класса.
    while (subButtons.size())
    {
      // Конструктор класса.
      delete *(subButtons.rbegin());
      // Конструктор класса.
      subButtons.pop_back();
    }
  }

  // Обработка ввода/событий SFML (мышь/клавиатура/окно).
  void eventProcessing(sf::Event event)
  {
    // Конструктор класса.
    if (selected)
    {
      // Конструктор класса.
      for (auto &button : subButtons)
      {
        // Конструктор класса.
        button->eventProcessing(event);
      }
    }
    else
    {
      // Конструктор класса.
      subButtons[buttonNumber]->Activatable::eventProcessing(event);
    }

    // Конструктор класса.
    Clickable::eventProcessing(event);
  }

  // Обновление состояния/логики перед отрисовкой.
  void update()
  {
    // Конструктор класса.
    if (selected)
    {
      // Конструктор класса.
      for (auto &button : subButtons)
      {
        // Конструктор класса.
        button->update();
      }
    }
    else
    {
      // Конструктор класса.
      subButtons[buttonNumber]->Activatable::update();
    }

    // Конструктор класса.
    if (unselect)
    {
      // Конструктор класса.
      for (auto &button : subButtons)
      {
        // Конструктор класса.
        button->appearance(Resource::unfocusedColor);
      }
      unselect = false;
    }

    // Конструктор класса.
    if (activate)
    {
      selected = !selected;
      unselect = selected == false;
      activate = false;
    }
  }

  // Установка позиции/размера (границ) и раскладка дочерних элементов.
  void setBound(float x, float y, float width, float height, float indent)
  {
    float deltaY = 0;
    // Конструктор класса.
    if (selected)
    {
      // Конструктор класса.
      for (auto &button : subButtons)
      {
        // Конструктор класса.
        button->setBound(x, y + deltaY, width, height, indent);
        // Конструктор класса.
        deltaY += button->getBound().getSize().y + indent;
      }
      deltaY -= indent;
    }
    else
    {
      // Конструктор класса.
      subButtons[buttonNumber]->setBound(x, y, width, height, indent);
      // Конструктор класса.
      deltaY += subButtons[buttonNumber]->getBound().getSize().y;
    }

    // Конструктор класса.
    Clickable::setBound(x, y, width, std::max(deltaY, height), indent);
  }

  // Отрисовка объекта на целевой поверхности.
  void draw(sf::RenderTarget &target, sf::RenderStates states) const
  {
    // Конструктор класса.
    if (selected)
    {
      // Конструктор класса.
      for (auto &button : subButtons)
      {
        // Конструктор класса.
        target.draw(*button, states);
      }
    }
    else
    {
      // Конструктор класса.
      target.draw(*subButtons[buttonNumber], states);
    }
  }

protected:
  // Применение темы/цветов к элементам.
  void appearance(sf::Color color) {}
};