/**
 * @file constantable.hpp
 * @brief Базовый класс для «постоянных» элементов (имеют границы и умеют
 * рисоваться).
 */
#pragma once
#include "../bound.hpp"
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

/**
 * @brief Маркерный класс для отрисовываемых элементов с границами.
 *
 * Наследники должны реализовывать:
 *  - setBound(...): вычисление/установка body;
 *  - draw(...): отрисовку.
 */
class Constantable : public Bound, public sf::Drawable {};
