/**
 * @file radioButton.hpp
 * @brief Радио-кнопка: надпись + раскрывающийся набор под-кнопок (варианты).
 *
 * selected=true — раскрыто все под-кнопки; selected=false — отображается только
 * выбранная.
 */
#pragma once
#include "button.hpp"
#include "subButtons.hpp"

/**
 * @brief Радио-кнопка, сочетающая Button (шапка/надпись) и SubButtons
 * (варианты).
 */
class RadioButton : public Button, public SubButtons {
public:
  /**
   * @brief Конструктор.
   * @param action        Действие при клике по «шапке» (переключает selected).
   * @param elements      Визуальные элементы шапки (например, текст).
   * @param subButtons    Список вариантов (по одному Button на вариант).
   * @param buttonNumber  Индекс активного варианта по умолчанию.
   */
  template <typename Action>
  RadioButton(Action action, std::vector<Activatable *> elements,
              std::vector<Button *> subButtons, int buttonNumber = 0)
      : Button(action, elements), SubButtons(subButtons, buttonNumber) {
    appearance(Resource::unfocusedColor);
  }

  /// События: сначала варианты (если раскрыто/или только активный), затем
  /// элементы шапки, затем логика клика.
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

  /// Обновление: варианты (все или выбранный), сброс «unselect» в
  /// unfocusedColor, затем элементы шапки и клик.
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
      selected = !selected;           // разворачиваем/сворачиваем список
      unselect = (selected == false); // при сворачивании сбросить цвета
      activate = false;
      action(); // пользовательский колбэк
    }

    Clickable::update();
  }

  /**
   * @brief Компоновка:
   *  - элементы шапки вертикально,
   *  - затем либо все варианты (если раскрыто), либо только выбранный.
   */
  void setBound(float x, float y, float width, float height, float indent) {
    float deltaY = 0;

    for (auto &element : elements) {
      element->setBound(x, y + deltaY, width, height, indent);
      deltaY += element->getBound().height + indent;
    }
    if (deltaY > 0)
      deltaY -= indent;

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

  /// Рисуем варианты (через SubButtons) и шапку (через Button).
  void draw(sf::RenderTarget &target, sf::RenderStates states) const {
    SubButtons::draw(target, states);
    Button::draw(target, states);
  }

private:
  /// Окраска только шапки (варианты красит SubButtons).
  void appearance(sf::Color color) { Button::appearance(color); }
};
