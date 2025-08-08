// Constantable — базовый класс для неизменяемых самих по себе элементов (рисуемые + границы).
// ------------------------------------------------------------

// Заголовочный файл. pragma once — защита от множественного включения.
#pragma once
#include "../bound.hpp"
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

// Класс Constantable — см. описание в заголовке файла.
class Constantable : public Bound, public sf::Drawable
{
};