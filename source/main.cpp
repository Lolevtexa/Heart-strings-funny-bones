/**
 * @file main.cpp
 * @brief Точка входа приложения: окно SFML, MainScene, обработка событий, смена
 * режимов экрана.
 */

#include "mainScene.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <cstdlib>

/**
 * @brief Установить локализованный заголовок окна.
 */
void setTitle(sf::RenderWindow &window) {
  window.setTitle(utf8_to_wstring(Resource::localization["window name"]));
}

/**
 * @brief Переключить окно в полноэкранный режим.
 * @param isFullscreen Флаг-состояние (обновляется по месту).
 *
 * Побочный эффект: пишет "fullscreen" в Resource::userSettings["screen mode"].
 */
void setFullscreen(sf::RenderWindow &window, bool &isFullscreen) {
  if (!isFullscreen) {
    window.create(sf::VideoMode::getDesktopMode(), "", sf::State::Fullscreen);
    setTitle(window);
    window.setVerticalSyncEnabled(true);

    Resource::userSettings["screen mode"] = "fullscreen";
    isFullscreen = true;
  }
}

/**
 * @brief Переключить окно в оконный режим (800x600).
 * @param isFullscreen Флаг-состояние (обновляется по месту).
 *
 * Побочный эффект: пишет "windowed" в Resource::userSettings["screen mode"].
 */
void setWindowed(sf::RenderWindow &window, bool &isFullscreen) {
  if (isFullscreen) {
    window.create(sf::VideoMode({800, 600}),
                  utf8_to_wstring(Resource::localization["window name"]),
                  sf::Style::Default);
    setTitle(window);
    window.setVerticalSyncEnabled(true);

    Resource::userSettings["screen mode"] = "windowed";
    isFullscreen = false;
  }
}

int main() {
  // true, если сейчас режим НЕ fullscreen -> надо создать окно windowed
  bool isFullscreen = Resource::userSettings["screen mode"] != "fullscreen";
  sf::RenderWindow window;
  if (!isFullscreen) {
    setFullscreen(window, isFullscreen);
  } else {
    setWindowed(window, isFullscreen);
  }

  // Пробрасываем действия в сцену (замыкания на окно/флаг).
  MainScene mainScene(
      [&window]() { window.close(); },
      [&window, &isFullscreen]() { setFullscreen(window, isFullscreen); },
      [&window, &isFullscreen]() { setWindowed(window, isFullscreen); },
      [&window]() { setTitle(window); }, window.getSize());

  // Главный цикл
  while (window.isOpen()) {
    // Очередь событий
    while (auto ev = window.pollEvent()) {
      if (ev->is<sf::Event::Closed>()) {
        window.close();
      } else if (const auto* r = ev->getIf<sf::Event::Resized>()) {
        sf::FloatRect visible({0.f, 0.f}, {static_cast<float>(r->size.x),
                                           static_cast<float>(r->size.y)});
        window.setView(sf::View(visible));
      }

      mainScene.eventProcessing(*ev);
    }

    // Сохраняем настройки пользователя (например, громкость/язык)
    Resource::updateJson();

    // Логика интерфейса
    mainScene.update();

    // Кадр
    window.clear(sf::Color::White);
    window.draw(mainScene);
    window.display();
  }

  return 0;
}
