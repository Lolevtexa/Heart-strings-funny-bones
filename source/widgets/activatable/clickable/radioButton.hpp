// RadioButton — кнопка с раскрывающимся набором под-кнопок (варианты), управляет выбранным состоянием.
// ------------------------------------------------------------

// Заголовочный файл. pragma once — защита от множественного включения.
#pragma once
#include "button.hpp"
#include "subButtons.hpp"

// Класс RadioButton — см. описание в заголовке файла.
class RadioButton : public Button, public SubButtons
{
public:
  template <typename Action>
  // Конструктор класса.
  RadioButton(Action action, std::vector<Activatable *> elements,
              std::vector<Button *> subButtons)
      // Конструктор класса.
      : Button(action, elements), SubButtons(subButtons)
  {
    // Конструктор класса.
    appearance(Resource::unfocusedColor);
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
    for (auto &element : elements)
    {
      // Конструктор класса.
      element->eventProcessing(event);
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
    for (auto &element : elements)
    {
      // Конструктор класса.
      element->update();
    }

    // Конструктор класса.
    if (activate)
    {
      selected = !selected;
      unselect = selected == false;
      activate = false;
      // Конструктор класса.
      action();
    }

    // Конструктор класса.
    Clickable::update();
  }

  // Установка позиции/размера (границ) и раскладка дочерних элементов.
  void setBound(float x, float y, float width, float height, float indent)
  {
    float deltaY = 0;

    // Конструктор класса.
    for (auto &element : elements)
    {
      // Конструктор класса.
      element->setBound(x, y + deltaY, width, height, indent);
      // Конструктор класса.
      deltaY += element->getBound().height + indent;
    }
    deltaY -= deltaY == 0 ? 0 : indent;

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
      subButtons[buttonNumber]->setBound(x, y + deltaY, width, height, indent);
      // Конструктор класса.
      deltaY += subButtons[buttonNumber]->getBound().getSize().y;
    }

    // Конструктор класса.
    AOutline::setBound(x, y, width, std::max(deltaY, height), indent);
  }

  // Отрисовка объекта на целевой поверхности.
  void draw(sf::RenderTarget &target, sf::RenderStates states) const
  {
    // Конструктор класса.
    SubButtons::draw(target, states);
    // Конструктор класса.
    Button::draw(target, states);
  }

private:
  // Применение темы/цветов к элементам.
  void appearance(sf::Color color) { Button::appearance(color); }
};