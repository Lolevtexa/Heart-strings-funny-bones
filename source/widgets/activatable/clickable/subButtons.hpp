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
  // Конструктор: инициализация класса SubButtons.
  SubButtons(std::vector<Button *> subButtons) : subButtons(subButtons)
  {
    for (int i = 0; i < this->subButtons.size(); i++)
    {
      std::function<void()> action = this->subButtons[i]->action;
      this->subButtons[i]->action = [this, i, action]()
      {
        buttonNumber = i;
        action();
      };
      this->subButtons[i]->appearance(Resource::unfocusedColor);
    }
  }

  ~SubButtons()
  {
    while (subButtons.size())
    {
      delete *(subButtons.rbegin());
      subButtons.pop_back();
    }
  }

  // Обработка ввода/событий SFML (мышь/клавиатура/окно).
  void eventProcessing(sf::Event event)
  {
    if (selected)
    {
      for (auto &button : subButtons)
      {
        button->eventProcessing(event);
      }
    }
    else
    {
      subButtons[buttonNumber]->Activatable::eventProcessing(event);
    }

    Clickable::eventProcessing(event);
  }

  // Обновление состояния/логики перед отрисовкой.
  void update()
  {
    if (selected)
    {
      for (auto &button : subButtons)
      {
        button->update();
      }
    }
    else
    {
      subButtons[buttonNumber]->Activatable::update();
    }

    if (unselect)
    {
      for (auto &button : subButtons)
      {
        button->appearance(Resource::unfocusedColor);
      }
      unselect = false;
    }

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
    if (selected)
    {
      for (auto &button : subButtons)
      {
        button->setBound(x, y + deltaY, width, height, indent);
        deltaY += button->getBound().getSize().y + indent;
      }
      deltaY -= indent;
    }
    else
    {
      subButtons[buttonNumber]->setBound(x, y, width, height, indent);
      deltaY += subButtons[buttonNumber]->getBound().getSize().y;
    }

    Clickable::setBound(x, y, width, std::max(deltaY, height), indent);
  }

  // Отрисовка объекта на целевой поверхности.
  void draw(sf::RenderTarget &target, sf::RenderStates states) const
  {
    if (selected)
    {
      for (auto &button : subButtons)
      {
        target.draw(*button, states);
      }
    }
    else
    {
      target.draw(*subButtons[buttonNumber], states);
    }
  }

protected:
  // Применение темы/цветов к элементам.
  void appearance(sf::Color color) {}
};