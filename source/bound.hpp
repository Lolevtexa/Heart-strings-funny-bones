// Базовый класс Bound — хранит прямоугольную область (sf::FloatRect) и
// предоставляет общие границы для виджетов.
// ------------------------------------------------------------

// Заголовочный файл. pragma once — защита от множественного включения.
#pragma once
#include <SFML/Graphics/Rect.hpp>

// Класс Bound — см. описание в заголовке файла.
class Bound {
protected:
  sf::FloatRect body;

public:
  // Установка позиции/размера (границ) и раскладка дочерних элементов.
  virtual void setBound(float x, float y, float width, float height,
                        float indent) {
    body.height = height;
    body.width = width;
    body.top = y;
    body.left = x;
  }

  sf::FloatRect getBound() { return body; }
};