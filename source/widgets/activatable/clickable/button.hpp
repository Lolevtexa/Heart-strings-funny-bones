/**
 * @file button.hpp
 * @brief Составная кнопка: содержит произвольные дочерние элементы
 * (текст/слайдеры) и действие action().
 */
#pragma once
#include "../clickable.hpp"
#include "../outline.hpp"

/**
 * @brief Кнопка с набором дочерних элементов. Реагирует на клик (Clickable).
 *
 * Компоновка: setBound() раскладывает дочерние элементы вертикально с
 * отступами, подбирает minWidth.
 */
class Button : virtual public Clickable, public AOutline {
protected:
  std::function<void()> action; ///< Колбэк при активации.
  std::vector<Activatable *>
      elements; ///< Дочерние элементы (владение — здесь).

public:
  /**
   * @brief Конструктор.
   * @param action   Действие по клику.
   * @param elements Дочерние элементы (все будут освобождены в деструкторе).
   */
  template <typename Action>
  Button(Action action, std::vector<Activatable *> elements)
      : action(action), elements(elements) {
    appearance(Resource::unfocusedColor);
  }

  ~Button() {
    while (!elements.empty()) {
      delete elements.back();
      elements.pop_back();
    }
  }

  /// Прокинуть событие всем дочерним элементам + своя логика клика.
  void eventProcessing(std::optional<sf::Event> &event) {
    for (auto &element : elements) {
      element->eventProcessing(event);
    }
    Clickable::eventProcessing(event);
  }

  /// Обновить дочерние элементы; выполнить action() при activate=true; затем
  /// общий апдейт Clickable.
  void update() {
    for (auto &element : elements) {
      element->update();
    }

    if (activate) {
      action();
      activate = false;
    }

    Clickable::update();
  }

  /**
   * @brief Вертикальная компоновка дочерних элементов с равными отступами.
   *
   * Дополнительно подбирается минимальная ширина (minWidth) по максимальной
   * ширине из детей, и обновляется рамка AOutline.
   */
  void setBound(float x, float y, float width, float height, float indent) {
    float deltaY = 0;
    float minWidth = width;

    for (auto &element : elements) {
      element->setBound(x, y + deltaY, width, height, indent);
      deltaY += element->getBound().size.y + indent;
      minWidth = std::max(minWidth, element->getBound().size.x);
    }
    if (deltaY > 0)
      deltaY -= indent; // убрать лишний нижний отступ

    // Выровнять ширину всех детей по minWidth
    for (auto &element : elements) {
      element->setBound(element->getBound().position.x, element->getBound().position.y,
                        minWidth, element->getBound().size.y, indent);
    }

    AOutline::setBound(x, y, minWidth, std::max(deltaY, height), indent);
  }

  /// Отрисовать всех детей, затем рамку.
  void draw(sf::RenderTarget &target, sf::RenderStates states) const {
    for (auto &element : elements) {
      target.draw(*element, states);
    }
    AOutline::draw(target, states);
  }

  friend class RadioButton;
  friend class SubButtons;

protected:
  /// Применить цветовую схему ко всем дочерним элементам и рамке.
  void appearance(sf::Color color) {
    for (auto &element : elements) {
      element->appearance(color);
    }
    AOutline::appearance(color);
  }
};
