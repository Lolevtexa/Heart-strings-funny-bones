/**
 * @file subButtons.hpp
 * @brief Набор внутренних кнопок и логика выбора текущего индекса.
 *
 * Используется RadioButton. Переназначает action у каждого варианта, чтобы
 * обновлять выбранный индекс.
 */
#pragma once
#include "button.hpp"

/**
 * @brief Блок управления под-кнопками (вариантами) и текущим выбором.
 */
class SubButtons : virtual public Clickable {
protected:
  bool selected = false; ///< true — раскрыть список вариантов.
  bool unselect =
      true; ///< запрос вернуть варианты к unfocusedColor после закрытия.

  int buttonNumber;                 ///< Индекс выбранного варианта.
  std::vector<Button *> subButtons; ///< Варианты (владение — здесь).

public:
  /**
   * @brief Конструктор.
   * @param subButtons   Кнопки-варианты.
   * @param buttonNumber Индекс выбранного по умолчанию.
   *
   * Переопределяет action каждой кнопки: при клике обновляет buttonNumber,
   * затем вызывает исходный action.
   */
  SubButtons(std::vector<Button *> subButtons, int buttonNumber = 0)
      : subButtons(subButtons), buttonNumber(buttonNumber) {
    for (int i = 0; i < static_cast<int>(this->subButtons.size()); i++) {
      std::function<void()> action = this->subButtons[i]->action;
      this->subButtons[i]->action = [this, i, action]() {
        this->buttonNumber = i;
        action();
      };
      this->subButtons[i]->appearance(Resource::unfocusedColor);
    }
  }

  ~SubButtons() {
    while (!subButtons.empty()) {
      delete subButtons.back();
      subButtons.pop_back();
    }
  }

  /// Прокидываем событие либо всем вариантам (если раскрыто), либо только
  /// активному.
  void eventProcessing(std::optional<sf::Event> &event) {
    if (selected) {
      for (auto &button : subButtons) {
        button->eventProcessing(event);
      }
    } else {
      subButtons[buttonNumber]->Activatable::eventProcessing(event);
    }
    Clickable::eventProcessing(event);
  }

  /// Обновляем либо все варианты, либо только выбранный; сбрасываем цвета при
  /// закрытии.
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

    if (activate) {
      selected = !selected;           // открыть/закрыть список
      unselect = (selected == false); // при закрытии попросить сбросить цвета
      activate = false;
    }
  }

  /**
   * @brief Компоновка под-кнопок: вертикальный список при selected=true; иначе
   * — только выбранная.
   *
   * Высота итоговой области — максимум из (накопленной высоты) и параметра
   * height.
   */
  void setBound(float x, float y, float width, float height, float indent) {
    float deltaY = 0;
    if (selected) {
      for (auto &button : subButtons) {
        button->setBound(x, y + deltaY, width, height, indent);
        deltaY += button->getBound().size.y + indent;
      }
      deltaY -= indent;
    } else {
      subButtons[buttonNumber]->setBound(x, y, width, height, indent);
      deltaY += subButtons[buttonNumber]->getBound().size.y;
    }

    Clickable::setBound(x, y, width, std::max(deltaY, height), indent);
  }

  /// Рисуем либо все варианты, либо только выбранный.
  void draw(sf::RenderTarget &target, sf::RenderStates states) const {
    if (selected) {
      for (auto &button : subButtons) {
        target.draw(*button, states);
      }
    } else {
      target.draw(*subButtons[buttonNumber], states);
    }
  }

protected:
  /// RadioButton перекрашивает шапку; варианты по умолчанию остаются без явной
  /// перекраски здесь.
  void appearance(sf::Color color) { (void)color; }
};
