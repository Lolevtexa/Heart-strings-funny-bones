// RadioButton — кнопка с раскрывающимся набором под-кнопок (варианты),
// управляет выбранным состоянием.
// ------------------------------------------------------------

// Заголовочный файл. pragma once — защита от множественного включения.
#pragma once
#include "button.hpp"
#include "subButtons.hpp"

// Класс RadioButton — см. описание в заголовке файла.
class RadioButton : public Button, public SubButtons {
public:
  template <typename Action>
  // Конструктор: инициализация класса RadioButton.
  RadioButton(Action action, std::vector<Activatable *> elements,
              std::vector<Button *> subButtons, int buttonNumber = 0)
      : Button(action, elements), SubButtons(subButtons, buttonNumber) {
    appearance(Resource::unfocusedColor);
  }

  // Обработка ввода/событий SFML (мышь/клавиатура/окно).
  void eventProcessing(sf::Event event) {
    if (selected) {
      for (auto &button : subButtons) {
        button->eventProcessing(event);
      }
    } else {
      subButtons[buttonNumber]->Activatable::eventProcessing(event);
    }

    for (auto &element : elements) {
      element->eventProcessing(event);
    }

    Clickable::eventProcessing(event);
  }

  // Обновление состояния/логики перед отрисовкой.
  void update() {
    if (selected) {
      for (auto &button : subButtons) {
        button->update();
      }
    } else {
      subButtons[buttonNumber]->Activatable::update();
    }

    if (unselect) {
      for (auto &button : subButtons) {
        button->appearance(Resource::unfocusedColor);
      }
      unselect = false;
    }

    for (auto &element : elements) {
      element->update();
    }

    if (activate) {
      selected = !selected;
      unselect = selected == false;
      activate = false;
      action();
    }

    Clickable::update();
  }

  // Установка позиции/размера (границ) и раскладка дочерних элементов.
  void setBound(float x, float y, float width, float height, float indent) {
    float deltaY = 0;

    for (auto &element : elements) {
      element->setBound(x, y + deltaY, width, height, indent);
      deltaY += element->getBound().height + indent;
    }
    deltaY -= deltaY == 0 ? 0 : indent;

    if (selected) {
      for (auto &button : subButtons) {
        button->setBound(x, y + deltaY, width, height, indent);
        deltaY += button->getBound().getSize().y + indent;
      }
      deltaY -= indent;
    } else {
      subButtons[buttonNumber]->setBound(x, y + deltaY, width, height, indent);
      deltaY += subButtons[buttonNumber]->getBound().getSize().y;
    }

    AOutline::setBound(x, y, width, std::max(deltaY, height), indent);
  }

  // Отрисовка объекта на целевой поверхности.
  void draw(sf::RenderTarget &target, sf::RenderStates states) const {
    SubButtons::draw(target, states);
    Button::draw(target, states);
  }

private:
  // Применение темы/цветов к элементам.
  void appearance(sf::Color color) { Button::appearance(color); }
};