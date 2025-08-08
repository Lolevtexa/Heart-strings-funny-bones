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
  std::function<void()> action;

  std::vector<Activatable *> elements;

public:
  template <typename Action>
  // Конструктор: инициализация класса Button.
  Button(Action action, std::vector<Activatable *> elements)
      : action(action), elements(elements)
  {
    appearance(Resource::unfocusedColor);
  }

  ~Button()
  {
    while (elements.size())
    {
      delete *elements.rbegin();
      elements.pop_back();
    }
  }

  // Обработка ввода/событий SFML (мышь/клавиатура/окно).
  void eventProcessing(sf::Event event)
  {
    for (auto &element : elements)
    {
      element->eventProcessing(event);
    }

    Clickable::eventProcessing(event);
  }

  // Обновление состояния/логики перед отрисовкой.
  void update()
  {
    for (auto &element : elements)
    {
      element->update();
    }

    if (activate)
    {
      action();
      activate = false;
    }

    Clickable::update();
  }

  // Установка позиции/размера (границ) и раскладка дочерних элементов.
  void setBound(float x, float y, float width, float height, float indent)
  {
    float deltaY = 0;
    float minWidth = width;

    for (auto &element : elements)
    {
      element->setBound(x, y + deltaY, width, height, indent);
      deltaY += element->getBound().height + indent;
      minWidth = std::max(minWidth, element->getBound().width);
    }
    deltaY -= deltaY == 0 ? 0 : indent;

    for (auto &element : elements)
    {
      element->setBound(element->getBound().left, element->getBound().top, minWidth, element->getBound().height, indent);
    }

    AOutline::setBound(x, y, minWidth, std::max(deltaY, height), indent);
  }

  // Отрисовка объекта на целевой поверхности.
  void draw(sf::RenderTarget &target, sf::RenderStates states) const
  {
    for (auto &element : elements)
    {
      target.draw(*element, states);
    }

    AOutline::draw(target, states);
  }

  friend class RadioButton;
  friend class SubButtons;

protected:
  // Применение темы/цветов к элементам.
  void appearance(sf::Color color)
  {
    for (auto &element : elements)
    {
      element->appearance(color);
    }

    AOutline::appearance(color);
  }
};