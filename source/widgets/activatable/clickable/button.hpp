// Button — составной виджет-кнопка с дочерними элементами (текст, слайдеры и т.п.) и action().
// ------------------------------------------------------------

// Заголовочный файл. pragma once — защита от множественного включения.
#pragma once
#include "../clickable.hpp"
#include "../outline.hpp"

// Класс Button — см. описание в заголовке файла.
class Button : virtual public Clickable, public AOutline
{
protected:
  // Конструктор класса.
  std::function<void()> action;

  std::vector<Activatable *> elements;

public:
  template <typename Action>
  // Конструктор класса.
  Button(Action action, std::vector<Activatable *> elements)
      // Конструктор класса.
      : action(action), elements(elements)
  {
    // Конструктор класса.
    appearance(Resource::unfocusedColor);
  }

  // Конструктор класса.
  ~Button()
  {
    // Конструктор класса.
    while (elements.size())
    {
      // Конструктор класса.
      delete *elements.rbegin();
      // Конструктор класса.
      elements.pop_back();
    }
  }

  // Обработка ввода/событий SFML (мышь/клавиатура/окно).
  void eventProcessing(sf::Event event)
  {
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
    for (auto &element : elements)
    {
      // Конструктор класса.
      element->update();
    }

    // Конструктор класса.
    if (activate)
    {
      // Конструктор класса.
      action();
      activate = false;
    }

    // Конструктор класса.
    Clickable::update();
  }

  // Установка позиции/размера (границ) и раскладка дочерних элементов.
  void setBound(float x, float y, float width, float height, float indent)
  {
    float deltaY = 0;
    float minWidth = width;

    // Конструктор класса.
    for (auto &element : elements)
    {
      // Конструктор класса.
      element->setBound(x, y + deltaY, width, height, indent);
      // Конструктор класса.
      deltaY += element->getBound().height + indent;
      // Конструктор класса.
      minWidth = std::max(minWidth, element->getBound().width);
    }
    deltaY -= deltaY == 0 ? 0 : indent;

    // Конструктор класса.
    for (auto &element : elements)
    {
      // Конструктор класса.
      element->setBound(element->getBound().left, element->getBound().top, minWidth, element->getBound().height, indent);
    }

    // Конструктор класса.
    AOutline::setBound(x, y, minWidth, std::max(deltaY, height), indent);
  }

  // Отрисовка объекта на целевой поверхности.
  void draw(sf::RenderTarget &target, sf::RenderStates states) const
  {
    // Конструктор класса.
    for (auto &element : elements)
    {
      // Конструктор класса.
      target.draw(*element, states);
    }

    // Конструктор класса.
    AOutline::draw(target, states);
  }

  friend class RadioButton;
  friend class SubButtons;

protected:
  // Применение темы/цветов к элементам.
  void appearance(sf::Color color)
  {
    // Конструктор класса.
    for (auto &element : elements)
    {
      // Конструктор класса.
      element->appearance(color);
    }

    // Конструктор класса.
    AOutline::appearance(color);
  }
};